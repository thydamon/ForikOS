// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: syscall.h
// Create Time: Wed 19 Jan 2022 03:31:03 AM CST
// This is ... 

#ifndef SYSCALL_H_
#define SYSCALL_H_

#include "lib/stdint.h"

enum SYSCALL_NR
{
    SYS_GETPID,
    SYS_WRITE,
    SYS_MALLOC,
    SYS_FREE
};

uint32_t getpid(void);
uint32_t write(char* str);
void* malloc(uint32_t size);
void free(void* ptr);

#endif
