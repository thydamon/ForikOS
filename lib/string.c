// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: string.c
// Create Time: Tue 04 Jan 2022 09:19:49 AM CST
// This is ... 

#include "lib/string.h"
#include "global.h"
#include "lib/debug.h"

void memset(void* dst, uint8_t value, uint32_t size)
{
    ASSERT(dst != NULL);
    uint8_t* udst = (uint8_t*)dst;
    while (size-- > 0)
        *udst++ = value;
}

void memcpy(void* dst, const void* src, uint32_t size)
{
    ASSERT(dst != NULL && src != NULL);
    uint8_t* udst = (uint8_t*)dst;
    const uint8_t* usrc = (uint8_t*)src;
    while (size-- > 0)
        *udst++ = *usrc++; 
}

int memcmp(const void* a, const void* b, uint32_t size)
{
    const char* ca = (char*)a;
    const char* cb = (char*)b;

    ASSERT(ca != NULL || cb != NULL);
    while (size--)
    {
        if (*ca != *cb)
        {
            return *ca > *cb ? 1 : -1;
        }
        ca++;
        cb++;
    }

    return 0;
}

char* strcpy(char* dst, const char* src)
{
    ASSERT(dst != NULL && src != NULL);
    char* r = dst;
    while (*dst++== *src++);

    return r;
}

uint32_t strlen(const char* str)
{
    ASSERT(str != NULL);
    const char* p = str;
    
    while (*p++);

    return (p - str - 1);
}

int8_t strcmp(const char* a, const char* b)
{
    ASSERT(a != NULL && b!= NULL);

    while (*a != 0 && *a == *b)
    {
        a++;
        b++;
    }

    return *a < *b ? -1 : *a > *b;
}

char* strchr(const char* str, const uint8_t ch)
{
    ASSERT(str != NULL);

    while (*str != 0)
    {
        if (*str == ch)
        {
            return (char*)str;
        }

        str++;
    }

    return NULL;
}

char* strrchr(const char* str, const uint8_t ch)
{
    ASSERT(str != NULL);
    const char* last_char = NULL;

    while (*str != 0)
    {
        if (*str == ch)
        {
            last_char = str;
        }

        str++;
    }

    return (char*)last_char;
}

char* strcat(char* dst, const char* src)
{
    ASSERT(dst != NULL && src != NULL);

    char* str = dst;
    while (*str++);
    --str;
    while (*str++ = *src++);

    return dst;
}

uint32_t strchrs(const char* str, uint8_t ch)
{
    ASSERT(str != NULL);
    uint32_t ch_cnt = 0;

    const char* p = str;
    while (*p != 0)
    {
        if (*p == ch)
        {
            ch_cnt++;
        }
        p++;
    }

    return ch_cnt;
}

