// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: bitmap.c
// Create Time: Thu 30 Dec 2021 03:01:22 AM CST
// This is ... 

#include "lib/bitmap.h"
#include "lib/stdint.h"
#include "lib/string.h"
#include "lib/print.h"
#include "kernel/interrupt.h"
#include "lib/debug.h"

// 位图初始化
void bitmap_init(struct bitmap* btmp)
{
    memset(btmp->bits, 0, btmp->btmp_bytes_len);   
}

// 判断bit_idx位是否为1,若为1则返回true,否则返回false
bool bitmap_scan_test(struct bitmap* btmp, uint32_t bit_idx)
{
    // 取数组下标
    uint32_t byte_idx = bit_idx / 8;
    // 数组内的位
    uint32_t bit_odd = bit_idx % 8;
    
    // 将1左移bit_idx位取与
    return (btmp->bits[byte_idx] & (BITMAP_MASK << bit_odd));
}

// 在位图中申请连续cnt个位,成功则返回其起始位下标,失败返回-1
int bitmap_scan(struct bitmap* btmp, uint32_t cnt)
{
    uint32_t idx_byte = 0;

    // 逐字比较
    while ((0xff == btmp->bits[idx_byte]) && (idx_byte < btmp->btmp_bytes_len))
    {
        // 1表示该位已分配,为0xff表示该字节已无空闲位,跳向下一字节
        idx_byte++;
    }

    ASSERT(idx_byte < btmp->btmp_bytes_len);
    if (idx_byte == btmp->btmp_bytes_len) // 该内存池找不到可用空间
    {
        return -1;
    }

    // 若在位图数组范围内的某字节内找到空闲位,在该字节内逐位对比,返回空闲位的索引
    int idx_bit = 0;
    // 和btmp->bits[idx_byte]这个字逐位对比
    while ((uint8_t)(BITMAP_MASK << idx_bit) & btmp->bits[idx_byte])
    {
        idx_bit++;
    }
    
    // 空闲位在位图的下标
    int bit_idx_start = idx_byte * 8 + idx_bit;
    if (cnt == 1)
    {
        return bit_idx_start;
    }

    // 记录还有多少位可用判断
    uint32_t bit_left = (btmp->btmp_bytes_len * 8 - bit_idx_start);
    uint32_t next_bit = bit_idx_start + 1;
    uint32_t count = 1;  // 用于记录找到的空闲位的个数

    bit_idx_start = -1;  // 先将其位置为-1
    while (bit_left-- > 0)
    {
        if (!(bitmap_scan_test(btmp, next_bit)))  // 若next_bit为0
        {
            count++;
        }
        else
        {
            count = 0;
        }

        if (count == cnt)  // 找到连续的cnt个空位
        {
            bit_idx_start = next_bit - cnt + 1;
            break;
        }
        next_bit++;
    }

    return bit_idx_start;
}

void bitmap_set(struct bitmap* btmp, uint32_t bit_idx, int8_t value) 
{ 
    ASSERT((value == 0) || (value == 1));
    uint32_t byte_idx  = bit_idx / 8;
    uint32_t bit_odd = bit_idx % 8;

    if (value)
    {
        btmp->bits[byte_idx] |= (BITMAP_MASK << bit_odd);
    }
    else
    {
        btmp->bits[byte_idx] &= ~(BITMAP_MASK << bit_odd);
    }
}

