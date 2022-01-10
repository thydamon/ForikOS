// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: string.h
// Create Time: Sat 25 Dec 2021 04:06:13 PM CST
// This is ... 

#ifndef STRING_H_
#define STRING_H_

#include "stdint.h"

void memset(void* dst, uint8_t value, uint32_t size);
void memcpy(void* dst, const void* src, uint32_t size);
int memcmp(const void* dst, const void* src, uint32_t size);
char* strcpy(char* dst, const char* src);
uint32_t strlen(const char* str);
int8_t strcmp(const char* a, const char* b);
char* strchr(const char* str, const uint8_t ch);
char* strrchr(const char* str, const uint8_t ch);
char* strcat(char* dst, const char* src);
uint32_t strchrs(const char* filename, uint8_t ch);

#endif
