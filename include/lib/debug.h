// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: debug.h
// Create Time: Mon 27 Dec 2021 07:58:10 PM CST
// This is ... 

#ifndef DEBUG_H_
#define DEBUG_H_

void panic_spin(char* file_name, int line, const char* func, const char* condition);
#define PANIC(...) panic_spin(__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef NDEBUG
    #define ASSERT(CONDITION) ((void)0)
#else
    #define ASSERT(CONDITION)      \
        if (CONDITION) {} else {   \
            PANIC(#CONDITION);      \ 
        }
#endif

#endif
