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
/*
#include "thread/thread.h"

void k_thread_a(void* arg)
{
    char* para = (char*)arg;
    while (1)
    {
        put_str(para);
    }
}

void k_thread_b(void* arg)
{
    char* para = (char*)arg;
    while (1)
    {
        put_str(para);
    }
}
*/

int main(void)
{
    put_str("I am kernel\n");
    init_all();
    // thread_start("k_thread_a", 31, k_thread_a, "argA ");
    // thread_start("k_thread_b", 8, k_thread_b, "argB ");
    // intr_enable();
    // 使用sti指令才能开启中断,这样中断程序才能运行,
    // 它将标志位寄存器eflas的IF位置为1,这样来自中断代理8259A的中断信号便可以被处理器受理了
    // 外部设备都是接在8259A的引脚上,由于在8259A中已经通过IMR寄存器将除时钟之外的所有外部设备
    // 中断都屏蔽了,这样开启中断后,处理器之后接收时钟中断
    // asm volatile("sti");
    while(1);
   	return 0;
}

