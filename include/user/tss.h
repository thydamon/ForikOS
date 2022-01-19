// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: tss.h
// Create Time: Sun 16 Jan 2022 03:07:04 PM CST
// This is ... 

#ifndef TSS_H_
#define TSS_H_

#include "thread/thread.h"

void update_tss_esp(struct task_struct* pthread);
void tss_init(void);

#endif
