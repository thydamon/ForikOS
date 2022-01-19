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

// 注意main函数之上只能给出函数声明,不能有函数地址
// 是因为定义了函数会改变函数的入口地址,汇编中函数的入口地址变成了k_thread_a
void k_thread_a(void*);
void k_thread_b(void*);
void u_prog_a(void);
void u_prog_b(void);
int test_var_a = 0;
int test_var_b = 0;
int prog_a_pid = 0;
int prog_b_pid = 0;


int main(void) {
   put_str("I am kernel\n");
   init_all();

   process_execute(u_prog_a, "user_prog_a");
   process_execute(u_prog_b, "user_prog_b");

   intr_enable();
   console_put_str(" main_pid:0x");
   console_put_int(sys_getpid());
   console_put_char('\n');
   thread_start("k_thread_a", 31, k_thread_a, "argA ");
   thread_start("k_thread_b", 31, k_thread_b, "argB ");
   while(1);
   return 0;
}

/* 在线程中运行的函数 */
void k_thread_a(void* arg) {     
   char* para = arg;
   console_put_str(" thread_a_pid:0x");
   console_put_int(sys_getpid());
   console_put_char('\n');
   console_put_str(" prog_a_pid:0x");
   console_put_int(prog_a_pid);
   console_put_char('\n');
   while(1); 
}

/* 在线程中运行的函数 */
void k_thread_b(void* arg) {     
   char* para = arg;
   console_put_str(" thread_b_pid:0x");
   console_put_int(sys_getpid());
   console_put_char('\n');
   console_put_str(" prog_b_pid:0x");
   console_put_int(prog_b_pid);
   console_put_char('\n');
   while(1); 
}

/* 测试用户进程 */
void u_prog_a(void) 
{
    prog_a_pid = getpid();
    while(1);
}

/* 测试用户进程 */
void u_prog_b(void) 
{
    prog_b_pid = getpid();
    while(1); 
}
