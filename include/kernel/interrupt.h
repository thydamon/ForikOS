// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: interrupt.h
// Create Time: Mon 27 Dec 2021 08:08:14 PM CST
// This is ... 

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "stdint.h"

typedef void* intr_handler;
void idt_init(void);

// 定义中断状态的两种类型
enum intr_status
{
    INTR_OFF,  // 关闭
    INTR_ON    // 开启
};

enum intr_status intr_get_status(void);
enum intr_status intr_set_status(enum intr_status);
enum intr_status intr_enable(void);
enum intr_status intr_disable(void);
void register_handler(uint8_t vector_no, intr_handler function);

#endif
