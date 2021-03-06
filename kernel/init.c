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
#include "device/console.h"
#include "thread/thread.h"
#include "mm/memory.h"
#include "device/keyboard.h"
#include "user/tss.h"
#include "user/syscall_init.h"
#include "device/ide.h"
#include "fs/fs.h"

void init_all()
{
    put_str("init_all\n");
    idt_init();
    mem_init();	  // 初始化内存管理系统
    thread_init(); // 初始化线程相关结构
    timer_init();
    console_init();
    keyboard_init();  // 键盘初始化
    tss_init();       // tss初始化
    syscall_init();   // 初始化系统调用
    intr_enable();    // 后面的ide_init需要打开中断
    ide_init();       // 初始化硬盘
    filesys_init();   // 初始化文件系统
}

