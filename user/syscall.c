// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: syscall.c
// Create Time: Wed 19 Jan 2022 03:33:06 AM CST
// This is ... 

#include "user/syscall.h"
#include "thread/thread.h"

// 无参数的系统调用
// retval 返回值
// "int $0x80" 内联汇编代码,执行0x80中断
// "=a" (retval) 位于output部分,表示retval由寄存器eax赋值
// "a" (NUMBER) 位于input部分,表示将变量NUMBER约束到eax寄存器,用eax寄存器作为输入
// "memory" 位于被改变的资源列表,表示向gcc申明修改了内存,在编译器优化代码时会重新读取内存
#define _syscall0(NUMBER) ({               \
    int retval;                            \
    asm volatile(                          \
    "int $0x80"                            \
    : "=a" (retval)                        \
    : "a" (NUMBER)                         \
    : "memory"                             \
    );                                     \
    retval;                                \
})

// 一个参数的系统调用
#define _syscall1(NUMBER, ARG1) ({         \
    int retval;                            \
    asm volatile(                          \
    "int $0x80"                            \
    : "=a" (retval)                        \
    : "a" (NUMBER), "b" (ARG1)             \
    : "memory"                             \
    );                                     \
    retval;                                \
})

// 两个参数的系统调用
#define _syscall2(NUMBER, ARG1, ARG2) ({         \
    int retval;                            \
    asm volatile(                          \
    "int $0x80"                            \
    : "=a" (retval)                        \
    : "a" (NUMBER), "b" (ARG1), "c"  (ARG2)            \
    : "memory"                             \
    );                                     \
    retval;                                \
})

// 三个参数的系统调用
#define _syscall3(NUMBER, ARG1, ARG2, ARG3) ({         \
    int retval;                            \
    asm volatile(                          \
    "int $0x80"                            \
    : "=a" (retval)                        \
    : "a" (NUMBER), "b" (ARG1), "c"  (ARG2), "d" (ARG3)            \
    : "memory"                             \
    );                                     \
    retval;                                \
})

// 返回当前任务pid
uint32_t getpid()
{
    return _syscall0(SYS_GETPID);
}

// 打印字符串
uint32_t write(int32_t fd, const void* buf, uint32_t count) 
{
   return _syscall3(SYS_WRITE, fd, buf, count);
}

// 申请size字节大小的内存,并返回结果
void* malloc(uint32_t size)
{
    return (void*)_syscall1(SYS_MALLOC, size);
}

// 释放ptr指向的内存
void free(void* ptr)
{
    _syscall1(SYS_FREE, ptr);
}

// 派生子进程,返回子进程pid
pid_t fork(void)
{
    return _syscall0(SYS_FORK);
}

