// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: printk.c
// Create Time: Sat 22 Jan 2022 07:53:48 PM CST
// This is ... 

#include "kernel/printk.h"
#include "lib/print.h"
#include "lib/stdio.h"
#include "lib/string.h"
#include "device/console.h"
#include "global.h"

#define va_start(args, first_fix) args = (va_list)&first_fix
#define va_end(args) args = NULL

// 供内核使用的格式化输出函数
void printk(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char buf[1024];

    memset(buf, 0x00, sizeof(buf));
    vsprintf(buf, format, args);
    va_end(args);
    console_put_str(buf);
}
