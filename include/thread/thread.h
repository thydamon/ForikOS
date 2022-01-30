// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: thread.h
// Create Time: Sun 02 Jan 2022 05:44:52 PM CST
// This is ... 

#ifndef THREAD_H_
#define THREAD_H_

#include "stdint.h"
#include "lib/list.h"
#include "lib/bitmap.h"
#include "mm/memory.h"

#define MAX_FILES_OPEN_PER_PROC 8

// 自定义通用函数类型,它将在很多线程函数中做为形参类型
typedef void thread_func(void*);
typedef int16_t pid_t;

enum task_status
{
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_WAITING,
    TASK_HANGING,
    TASK_DIED
};

// 中断栈
struct intr_stack
{
    uint32_t vec_no;     // kernel.S 宏VECTOR中push %1压入的中断号
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    // 以下由cpu从低特权级进入高特权级时压入
    uint32_t err_code;
    void (*eip) (void);
    uint32_t cs;
    uint32_t eflags;
    void* esp;
    uint32_t ss;
};

// 线程栈thread_stack,用于存储线程中待执行的函数
// 此结构在线程自己的内核栈中位置不固定
// 仅用在switch_to是保存线程环境,实际位置取决于实际运行情况
struct thread_stack
{
    // 应用程序二进制接口ABI
    // 规定参数如何传递，返回值如何存储，系统调用的实现方式，目标文件格式或数据类型等
    // switch_to函数切换时,先在线程栈thread_stack中压入这4个寄存器
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edi;
    uint32_t esi;

    // 线程第一次执行时,eip指向待调用的函数kernel_thread
    // 其它时候,eip是指向switch_to的返回地址
    void (*eip) (thread_func* func, void* func_arg);
    // 占位函数返回地址
    void (*unused_retaddr);
    thread_func* function;
    void* func_arg;
};

// 线程的控制块
struct task_struct
{
    uint32_t* self_kstack;    // 各内核线程都用自己的内核栈
    pid_t pid;
    enum task_status status;
    char name[16];
    uint8_t priority;
    uint8_t ticks;      // 每次在处理器上执行的时间嘀嗒数

    uint32_t elapsed_ticks;

    struct list_elem general_tag;
    struct list_elem all_list_tag;
    
    uint32_t* pgdir; 
    struct virtual_addr user_vaddr;   // 用户进程的虚拟地址
    struct mem_block_desc u_block_desc[DESC_CNT];   // 用户进程内存块描述符
    int32_t fd_table[MAX_FILES_OPEN_PER_PROC];   // 文件描述符数组
    uint32_t cwd_inode_nr;    // 进程所在的工作目录的inode编号
    int16_t parent_pid;       // 父进程pid
    uint32_t stack_magic;
};

extern struct list thread_ready_list;
extern struct list thread_all_list;

void thread_create(struct task_struct* pthread, thread_func function, void* func_arg);
void init_thread(struct task_struct* pthread, char* name, int prio);
struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_arg);
struct task_struct* running_thread(void);
void schedule(void);
void thread_init(void);
void thread_block(enum task_status stat);
void thread_unblock(struct task_struct* pthread);
pid_t fork_pid(void);

#endif
