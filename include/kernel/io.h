// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: io.h
// Create Time: Sat 25 Dec 2021 09:52:53 PM CST
// This is ... 

#ifndef IO_H_
#define IO_H_

#include "stdint.h"

// 向端口port写入一个字节
static inline void outb(uint16_t port, uint8_t data)
{
    // "a"-ax、al、eax寄存器, "Nd"-dx寄存器, %b0-al寄存器, %w1-dx寄存器
    // "a"(data)-data放入ax寄存器, "Nd"(port)-port放入dx寄存器
    asm volatile("outb %b0, %w1" : : "a" (data), "Nd" (port));
}

// 将addr处起始的word_cnt个字写入端口port
static inline void outsw(uint16_t port, const void* addr, uint32_t word_cnt)
{
    // "+"-表示即做输入又做输出
    // outsw-表示把ds:esi处的内容写入port端口
    asm volatile("cld; rep outsw" : "+S" (addr), "+c" (word_cnt) : "d" (port));
}

// 从头端口port读入一个字节返回
static inline uint8_t inb(uint16_t port)
{
    uint8_t data;
    // "=a"-表示会将处理结果存入eax,然后写到变量data
    asm volatile("inb %w1, %b0" : "=a"(data) : "Nd" (port));

    return data;
}

// 将从端口port读入的word_cnt个字写入addr
static inline void insw(uint16_t port, void* addr, uint32_t word_cnt)
{
    // insw是从端口port处读入16位的内容写入es:edi指向的内存
    asm volatile("cld; rep insw": "+D" (addr), "+c" (word_cnt) : "d" (port) : "memory");
}


#endif
