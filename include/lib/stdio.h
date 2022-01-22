// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: stdio.h
// Create Time: Wed 19 Jan 2022 08:02:06 AM CST
// This is ... 

#ifndef STDIO_H_
#define STDIO_H_

#include "lib/stdint.h"

typedef char* va_list;
uint32_t printf(const char* str, ...);
uint32_t vsprintf(char* str, const char* format, va_list ap);
uint32_t sprintf(char* buf, const char* format, ...);

#endif
