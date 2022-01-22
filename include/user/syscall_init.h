// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: syscall_init.h
// Create Time: Wed 19 Jan 2022 04:39:05 AM CST
// This is ... 

#ifndef SYSCALL_INIT_H_
#define SYSCALL_INIT_H_

#include "lib/stdint.h"

void syscall_init(void);
uint32_t sys_getpid(void);
uint32_t sys_write(char* str);
#endif


