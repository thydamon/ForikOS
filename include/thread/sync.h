// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: sync.h
// Create Time: Tue 11 Jan 2022 07:37:20 PM CST
// This is ... 

#ifndef SYNC_H_
#define SYNC_H_

#include "lib/list.h"
#include "stdint.h"
#include "thread/thread.h"

// 信号量结构体
struct semaphore
{
    uint8_t value;
    struct  list waiters;
};

// 锁结构
struct lock
{
    struct task_struct* holder;   // 锁的持有者
    struct semaphore semaphore;   // 用二元信号量实现锁
    uint32_t holder_repeat_nr;    // 锁的持有者重复申请锁的次数
};

void sema_init(struct semaphore* psema, uint8_t value);
void sema_down(struct semaphore* psema);
void sema_up(struct semaphore* psema);
void lock_init(struct lock* plock);
void lock_acquire(struct lock* plock);
void lock_release(struct lock* plock);

#endif
