// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: ioqueue.h
// Create Time: Sat 15 Jan 2022 05:41:25 PM CST
// This is ... 

#ifndef IOQUEUE_H_
#define IOQUEUE_H_

#include "stdint.h"
#include "thread/thread.h"
#include "thread/sync.h"

#define bufsize 64

// 环形队列
struct ioqueue
{
    struct lock lock;
    // 记录缓冲区满时阻塞的生产者
    struct task_struct* producer;
    // 记录缓冲区空时阻塞的消费者
    struct task_struct* consumer;
    char buf[bufsize];
    int32_t head;
    int32_t tail;
};

void ioqueue_init(struct ioqueue* ioq);
bool ioq_full(struct ioqueue* ioq);
char ioq_getchar(struct ioqueue* ioq);
void ioq_putchar(struct ioqueue* ioq, char byte);

#endif
