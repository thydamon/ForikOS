// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: include/user/wait_exit.h
// Create Time: Tue 01 Feb 2022 05:51:11 AM CST
// This is ... 

#ifndef WAIT_EXIT_H_
#define WAIT_EXIT_H_

#include "thread/thread.h"

pid_t sys_wait(int32_t* status);
void sys_exit(int32_t status);


#endif

