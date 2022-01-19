// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: console.c
// Create Time: Wed 12 Jan 2022 08:54:01 PM CST
// This is ... 

#include "device/console.h"
#include "lib/print.h"
#include "stdint.h"
#include "thread/sync.h"
#include "thread/thread.h"

static struct lock console_lock;

// 初始化锁
void console_init()
{
    lock_init(&console_lock);
}

// 获取终端
void console_acquire()
{
    lock_acquire(&console_lock);
}

// 释放终端
void console_release()
{
    lock_release(&console_lock);
}

// 终端中输出字符串
void console_put_str(char* str)
{
    console_acquire();
    put_str(str);
    console_release();
}

// 终端输出字符
void console_put_char(uint8_t ch)
{
    console_acquire();
    put_char(ch);
    console_release();
}

// 终端中输出16进制整数
void console_put_int(uint32_t num)
{
    console_acquire();
    put_int(num); 
    console_release();
}

