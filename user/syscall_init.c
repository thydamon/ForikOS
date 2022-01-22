// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: syscall_init.c
// Create Time: Wed 19 Jan 2022 04:40:44 AM CST
// This is ... 

#include "user/syscall_init.h"
#include "user/syscall.h"
#include "lib/stdint.h"
#include "lib/print.h"
#include "thread/thread.h"
#include "device/console.h"
#include "lib/string.h"
#include "mm/memory.h"

#define syscall_nr 32
typedef void* syscall;
syscall syscall_table[syscall_nr];

// 返回当前任务的pid
uint32_t sys_getpid()
{
    return running_thread()->pid;
}

// 打印字符串str
uint32_t sys_write(char* str)
{
    console_put_str(str);
    return strlen(str);
}

// 初始化系统调用
void syscall_init()
{
    put_str("syscall_init start\n");
    syscall_table[SYS_GETPID] = sys_getpid;
    syscall_table[SYS_WRITE] = sys_write;
    syscall_table[SYS_MALLOC] = sys_malloc;
    syscall_table[SYS_FREE] = sys_free;
    put_str("syscall_init done\n");
}
