// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: main.c
// Create Time: Wed 29 Dec 2021 09:59:21 PM CST
// This is ... 

#include "kernel/init.h"
#include "lib/print.h"
#include "kernel/interrupt.h"
#include "thread/thread.h"
#include "device/console.h"
#include "user/process.h"
#include "user/syscall.h"
#include "user/syscall_init.h"
#include "lib/stdio.h"
#include "mm/memory.h"
#include "fs/dir.h"
#include "fs/fs.h"

// 注意main函数之上只能给出函数声明,不能有函数地址
// 是因为定义了函数会改变函数的入口地址,汇编中函数的入口地址变成了k_thread_a
void k_thread_a(void*);
void k_thread_b(void*);
void u_prog_a(void);
void u_prog_b(void);
void init(void);


int main(void) 
{
   put_str("I am kernel\n");
   init_all();
   while(1);
   return 0;
}

/* 在线程中运行的函数 */
void k_thread_a(void* arg) 
{     
   void* addr1 = sys_malloc(256);
   void* addr2 = sys_malloc(255);
   void* addr3 = sys_malloc(254);
   console_put_str(" thread_a malloc addr:0x");
   console_put_int((int)addr1);
   console_put_char(',');
   console_put_int((int)addr2);
   console_put_char(',');
   console_put_int((int)addr3);
   console_put_char('\n');

   int cpu_delay = 100000;
   while(cpu_delay-- > 0);
   sys_free(addr1);
   sys_free(addr2);
   sys_free(addr3);
   while(1); 
}

/* 在线程中运行的函数 */
void k_thread_b(void* arg) 
{     
   void* addr1 = sys_malloc(256);
   void* addr2 = sys_malloc(255);
   void* addr3 = sys_malloc(254);
   console_put_str(" thread_b malloc addr:0x");
   console_put_int((int)addr1);
   console_put_char(',');
   console_put_int((int)addr2);
   console_put_char(',');
   console_put_int((int)addr3);
   console_put_char('\n');

   int cpu_delay = 100000;
   while(cpu_delay-- > 0);
   sys_free(addr1);
   sys_free(addr2);
   sys_free(addr3);
   while(1); 
}

/* 测试用户进程 */
void u_prog_a(void) 
{
   void* addr1 = malloc(256);
   void* addr2 = malloc(255);
   void* addr3 = malloc(254);
   printf(" prog_a malloc addr:0x%x,0x%x,0x%x\n", (int)addr1, (int)addr2, (int)addr3);

   int cpu_delay = 100000;

   while(cpu_delay-- > 0);
   free(addr1);
   free(addr2);
   free(addr3);
    while(1);
}

/* 测试用户进程 */
void u_prog_b(void) 
{
   void* addr1 = malloc(256);
   void* addr2 = malloc(255);
   void* addr3 = malloc(254);
   printf(" prog_b malloc addr:0x%x,0x%x,0x%x\n", (int)addr1, (int)addr2, (int)addr3);

   int cpu_delay = 100000;
   while(cpu_delay-- > 0);
   free(addr1);
   free(addr2);
   free(addr3);
    while(1); 
}

// init进程
void init(void)
{
    uint32_t ret_pid = fork();
    if (ret_pid)
    {
        printf("i am father, my pid is %d, child pid is %d\n", getpid(), ret_pid);
    }
    else
    {
        printf("i am child, my pid is %d, ret pid is %d\n", getpid(), ret_pid);
    }

    while(1);
}
