// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: include/user/exec.h
// Create Time: Mon 31 Jan 2022 03:02:29 PM CST
// This is ... 

#ifndef EXEC_H_
#define EXEC_H_

#include "lib/stdint.h"

int32_t sys_execv(const char* path, const char*  argv[]);

#endif
