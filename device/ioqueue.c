// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: ioqueue.c
// Create Time: Sat 15 Jan 2022 05:47:24 PM CST
// This is ... 

#include "device/ioqueue.h"
#include "kernel/interrupt.h"
#include "global.h"
#include "lib/debug.h"

// 初始化io队列
void ioqueue_init(struct ioqueue* ioq)
{
    lock_init(&ioq->lock);
    ioq->producer = NULL;
    ioq->consumer = NULL;
    ioq->tail = 0;
    ioq->head = 0;
}

// 返回pos在缓冲区中下一个位置的值
static int32_t next_pos(int32_t pos)
{
    return (pos + 1) % bufsize;
}

// 判断队列是否已满
bool ioq_full(struct ioqueue* ioq)
{
    ASSERT(intr_get_status() == INTR_OFF);
    return next_pos(ioq->head) == ioq->tail;
}

// 判断队列是否已空
static bool ioq_empty(struct ioqueue* ioq)
{
    ASSERT(intr_get_status() == INTR_OFF);
    return ioq->head == ioq->tail;
}

// 使当前生产者或消费者在此缓冲区上等待
static void ioq_wait(struct task_struct** waiter)
{
    ASSERT(*waiter == NULL && waiter != NULL);
    *waiter = running_thread();
    thread_block(TASK_BLOCKED);
}

// 唤醒waiter
static void wakeup(struct task_struct** waiter)
{
    ASSERT(*waiter != NULL);
    thread_unblock(*waiter);
    *waiter = NULL;
}

// 消费者从ioq队列中获取一个字符
char ioq_getchar(struct ioqueue* ioq)
{
    ASSERT(intr_get_status() == INTR_OFF);

    // 若缓冲区(队列)为空,把消费者ioq->consumer记为当前线程自己,
    // 目的是将来生产者往缓冲区里装商品后,生产者知道唤醒哪个消费者,
    // 也就是唤醒当前线程自己
    while (ioq_empty(ioq))
    {
        lock_acquire(&ioq->lock);
        ioq_wait(&ioq->consumer);
        lock_release(&ioq->lock);
    }

    char byte = ioq->buf[ioq->tail];   // 从缓冲区中取出
    ioq->tail = next_pos(ioq->tail);   // 把读游标移到下一位置

    if (ioq->producer != NULL)
    {
        wakeup(&ioq->producer);         // 唤醒生产者
    }

    return byte;
}

// 生产者往ioq队列中写入一个字符byte
void ioq_putchar(struct ioqueue* ioq, char byte)
{
    ASSERT(intr_get_status() == INTR_OFF);

    // 若缓冲区(队列)已经满了,把生产者ioq->producer记为自己,
    // 为的是当缓冲区里的东西被消费者取完后让消费者知道唤醒哪个生产者,
    // 也就是唤醒当前线程自己
    while (ioq_full(ioq))
    {
        lock_acquire(&ioq->lock);
        ioq_wait(&ioq->producer);
        lock_release(&ioq->lock);
    }

    ioq->buf[ioq->head] = byte;      // 把字节放入缓冲区中
    ioq->head = next_pos(ioq->head); // 把写游标移到下一位置

    if (ioq->consumer != NULL)
    {
        wakeup(&ioq->consumer);          // 唤醒消费者
    }
}
