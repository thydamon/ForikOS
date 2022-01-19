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

#define syscall_nr 32
typedef void* syscall;
syscall syscall_table[syscall_nr];

// 返回当前任务的pid
uint32_t sys_getpid()
{
    return running_thread()->pid;
}

// 初始化系统调用
void syscall_init()
{
    put_str("syscall_init start\n");
    syscall_table[SYS_GETPID] = sys_getpid;
    put_str("syscall_init done\n");
}
