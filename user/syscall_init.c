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
#include "fs/fs.h"
#include "user/fork.h"

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
    syscall_table[SYS_WRITE] = sys_write;
    syscall_table[SYS_MALLOC] = sys_malloc;
    syscall_table[SYS_FREE] = sys_free;
    syscall_table[SYS_FORK]   = sys_fork;
    syscall_table[SYS_READ]    = sys_read;
    syscall_table[SYS_PUTCHAR] = sys_putchar;
    syscall_table[SYS_CLEAR]   = cls_screen;
    syscall_table[SYS_GETCWD]     = sys_getcwd;
    syscall_table[SYS_OPEN]       = sys_open;
    syscall_table[SYS_CLOSE]      = sys_close;
    syscall_table[SYS_LSEEK]	 = sys_lseek;
    syscall_table[SYS_UNLINK]	 = sys_unlink;
    syscall_table[SYS_MKDIR]	 = sys_mkdir;
    syscall_table[SYS_OPENDIR]	 = sys_opendir;
    syscall_table[SYS_CLOSEDIR]   = sys_closedir;
    syscall_table[SYS_CHDIR]	 = sys_chdir;
    syscall_table[SYS_RMDIR]	 = sys_rmdir;
    syscall_table[SYS_READDIR]	 = sys_readdir;
    syscall_table[SYS_REWINDDIR]	 = sys_rewinddir;
    syscall_table[SYS_STAT]	 = sys_stat;
    syscall_table[SYS_PS]	 = sys_ps;
    put_str("syscall_init done\n");
}
