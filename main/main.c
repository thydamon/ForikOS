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

// 注意main函数之上只能给出函数声明,不能有函数地址
// 是因为定义了函数会改变函数的入口地址,汇编中函数的入口地址变成了k_thread_a
void k_thread_a(void *arg);
void k_thread_b(void *arg);

int main(void)
{
    put_str("I am kernel\n");
    init_all();
    // thread_start("k_thread_a", 31, k_thread_a, "argA ");
    // thread_start("k_thread_b", 8, k_thread_b, "argB ");
    intr_enable(); // 开启中断
    while(1);
   	return 0;
}

void k_thread_a(void* arg)
{
    char* para = (char*)arg;
    while (1)
    {
        // put_str(para);
        console_put_str(para);
    }
}

void k_thread_b(void* arg)
{
    char* para = (char*)arg;
    while (1)
    {
        // put_str(para);
        console_put_str(para);
    }
}

