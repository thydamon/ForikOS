// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: include/user/assert.h
// Create Time: Sat 29 Jan 2022 11:51:39 PM CST
// This is ... 

#ifndef ASSERT_H_
#define ASSERT_H_

#define NULL ((void*)0)

void user_spin(char* filename, int line, const char* func, const char* condition);
#define panic(...) user_spin(__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef NDEBUG
    #define assert(CONDITION) ((void)0)
#else
    #define assert(CONDITION) if (!(CONDITION)) { panic(#CONDITION); }
#endif

#endif
