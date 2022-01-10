// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: debug.c
// Create Time: Thu 30 Dec 2021 02:54:06 AM CST
// This is ... 

#include "lib/debug.h"
#include "lib/print.h"
#include "kernel/interrupt.h"

// 打印文件名、行号、函数名、条件并使程序挂起
void panic_spin(char* filename, int line, const char* func, const char* condition)
{
    intr_disable();
    put_str("\n\n\n!!!!!! error !!!!!!!!\n");
    put_str("filename:");
    put_str(filename);
    put_str("\n");
    put_str("line:");
    put_int(line);
    put_str("\n");
    put_str("function:");
    put_str((char*)func);
    put_str("\n");
    put_str("condition:");
    put_str((char*)condition);
    put_str("\n");
}
