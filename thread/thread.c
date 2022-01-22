// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: thread.c
// Create Time: Sun 02 Jan 2022 05:55:38 PM CST
// This is ... 

#include "thread/thread.h"
#include "lib/stdint.h"
#include "lib/string.h"
#include "global.h"
#include "lib/debug.h"
#include "kernel/interrupt.h"
#include "lib/print.h"
#include "mm/memory.h"
#include "user/process.h"
#include "thread/sync.h"
#include "device/console.h"

// 定义主线程PCB
struct task_struct* main_thread;
// 线程就绪队列
struct list thread_ready_list;
// 线程全量队列
struct list thread_all_list;
struct lock pid_lock;		        // 分配pid锁
static struct list_elem* thread_tag;

extern void switch_to(struct task_struct* cur, struct task_struct* next);

// 获取当前pcb指针
struct task_struct* running_thread()
{
    uint32_t esp;

    asm("mov %%esp, %0" : "=g"(esp));
    // 取esp整数部分即pcb起始地址
    return (struct task_struct*)(esp & 0xfffff000);
}

// 由kernel_thread去执行function(func_arg)
static void kernel_thread(thread_func* function, void* func_arg)
{
    intr_enable();
    function(func_arg);
}

// 分配pid
static pid_t allocate_pid(void) {
   static pid_t next_pid = 0;
   lock_acquire(&pid_lock);
   next_pid++;
   lock_release(&pid_lock);
   return next_pid;
}
// 初始化线程栈thread_stack,将待执行的函数和参数放到thread_stack中相应的位置
void thread_create(struct task_struct* pthread, thread_func function, void* func_arg)
{
    // 先预留中断使用栈的空间,可见thread.h中定义的结构
    pthread->self_kstack -= sizeof(struct intr_stack);
    
    // 再留出线程栈空间,可见thread.h中定义
    pthread->self_kstack -= sizeof(struct thread_stack);
    struct thread_stack* kthread_stack = (struct thread_stack*)pthread->self_kstack;
    kthread_stack->eip = kernel_thread;
    kthread_stack->function = function;
    kthread_stack->func_arg = func_arg;
    kthread_stack->ebp =  0;
    kthread_stack->ebx = 0;
    kthread_stack->esi = 0;
    kthread_stack->edi = 0;
}

// 初始化线程基本信息
void init_thread(struct task_struct* pthread, char* name, int prio)
{
    memset(pthread, 0, sizeof(*pthread));
    pthread->pid = allocate_pid();
    strcpy(pthread->name, name);

    if (pthread == main_thread)
    {
        pthread->status = TASK_RUNNING;
    }
    else
    {
        pthread->status = TASK_READY;
    }

    // self_kstack是线程自己在内核态下使用的栈顶地址
    pthread->self_kstack = (uint32_t*)((uint32_t)pthread + PG_SIZE);
    pthread->priority = prio;
    pthread->ticks = prio;
    pthread->elapsed_ticks = 0;
    pthread->pgdir = NULL;
    pthread->stack_magic = 0x19870916;
}

// 创建一优先级为prio的线程,线程名为name,线程所执行的函数是function(func_arg)
struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_arg)
{
    // pcb都位于内核空间,包括用户进程的pcb也是在内核空间
    struct task_struct* thread = get_kernel_pages(1);

    init_thread(thread, name, prio);
    // 初始化线程栈
    thread_create(thread, function, func_arg);

    // 确保之前不在队列中 
    ASSERT(!elem_find(&thread_ready_list, &thread->general_tag));
    // 加入就绪线程队列
    list_append(&thread_ready_list, &thread->general_tag);

    // 确保之前不在队列中
    ASSERT(!elem_find(&thread_all_list, &thread->all_list_tag));
    // 加入全部线程队列
    list_append(&thread_all_list, &thread->all_list_tag);

    return thread;
}

// 将kernel中的main函数完善为主线程
static void make_main_thread(void)
{
    main_thread = running_thread();
    init_thread(main_thread, "main", 31);

    // main函数是当前线程,当前线程不在thread_ready_list中,所以只将其加在thread_all_list中
    ASSERT(!elem_find(&thread_all_list, &main_thread->all_list_tag));
    list_append(&thread_all_list, &main_thread->all_list_tag);
}

// 实现任务调度
void schedule()
{
    ASSERT(intr_get_status() == INTR_OFF);

    struct task_struct* cur = running_thread();
    // 若此线程只是cpu时间片到了,将其加入到就绪队列尾
    if (cur->status == TASK_RUNNING)
    {
        ASSERT(!elem_find(&thread_ready_list, &cur->general_tag));
        list_append(&thread_ready_list, &cur->general_tag);
        cur->ticks = cur->priority;     // 重新将当前线程的ticks再重置为其priority;
        cur->status = TASK_READY;
    }
    else
    {
        // todo 若此线程需要某事件发生后才能继续上cpu运行,不需要将其加入队列,因为当前线程不在就绪队列中
    }

    ASSERT(!list_empty(&thread_ready_list));
    thread_tag = NULL;
    thread_tag = list_pop(&thread_ready_list);
    struct task_struct* next = elem2entry(struct task_struct, general_tag, thread_tag);
    next->status = TASK_RUNNING;

    // 激活任务页表
    process_activate(next);

    switch_to(cur, next);
}

// 当前线程将自己阻塞,将状态置为stat
void thread_block(enum task_status stat)
{
    // stat状态为TASK_BLOCKED,TASK_WAITING,TASK_HANGING
    ASSERT(((stat == TASK_BLOCKED) || (stat == TASK_WAITING) || (stat == TASK_HANGING)));
    enum intr_status old_status = intr_disable();
    struct task_struct* cur_thread = running_thread();
    cur_thread->status = stat;
    schedule();  // 将当前线程换下处理器
    // 待当前线程被解除阻塞后才继续运行下面的intr_set_status
    intr_set_status(old_status);
}

// 将线程pthread接触阻塞
void thread_unblock(struct task_struct* pthread)
{
    enum intr_status old_status = intr_disable();
    ASSERT(((pthread->status == TASK_BLOCKED) || (pthread->status == TASK_WAITING) || (pthread->status == TASK_HANGING)));
    if (pthread->status != TASK_READY)
    {
        ASSERT(!elem_find(&thread_ready_list, &pthread->general_tag));
        if (elem_find(&thread_ready_list, &pthread->general_tag))
        {
            PANIC("thread_unblock: blocked thread in ready_list\n");
        }
        list_push(&thread_ready_list, &pthread->general_tag);
        pthread->status = TASK_READY;
    }
    intr_set_status(old_status);
}

// 初始化线程环境
void thread_init(void)
{
    put_str("thread_init start\n");
    list_init(&thread_ready_list);
    list_init(&thread_all_list);
    lock_init(&pid_lock);
    // 将当前main函数创建为线程
    make_main_thread();
    put_str("thread_init done\n");
}


