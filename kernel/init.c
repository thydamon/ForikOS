// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: init.c
// Create Time: Wed 29 Dec 2021 09:56:08 PM CST
// This is ... 

#include "kernel/init.h"
#include "lib/print.h"
#include "kernel/interrupt.h"
#include "device/timer.h"

void init_all()
{
    put_str("init_all\n");
    // 初始化中断
    idt_init();
    // 初始化PIT
    timer_init();
    mem_init();	  // 初始化内存管理系统
}

