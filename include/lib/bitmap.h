// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: bitmap.h
// Create Time: Sat 25 Dec 2021 09:21:26 PM CST
// This is ... 

#ifndef BITMAP_H_
#define BITMAP_H_

#include "global.h"

#define BITMAP_MASK 1

struct bitmap 
{
    uint32_t btmp_bytes_len;
    // 遍历位图时,整体上以字节为单位,细节上是以位为单位,所以此处位图的指针必须为单字节
    uint8_t* bits; 
};

void bitmap_init(struct bitmap* btmp);
bool bitmap_scan_test(struct bitmap* btmp, uint32_t bit_idx);
int bitmap_scan(struct bitmap* btmp, uint32_t cnt);
void bitmap_set(struct bitmap* btmp, uint32_t bit_idx, int8_t value);

#endif
