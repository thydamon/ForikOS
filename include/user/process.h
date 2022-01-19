// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: process.h
// Create Time: Mon 17 Jan 2022 07:10:00 PM CST
// This is ... 

#ifndef PROCESS_H_
#define PROCESS_H_

#include "thread/thread.h"
#include "lib/stdint.h"

#define default_prio 31
#define USER_STACK3_VADDR  (0xc0000000 - 0x1000)
#define USER_VADDR_START 0x8048000

void process_execute(void* filename, char* name);
void start_process(void* filename);
void process_activate(struct task_struct* p_thread);
void page_dir_activate(struct task_struct* p_thread);
uint32_t* create_page_dir(void);
void create_user_vaddr_bitmap(struct task_struct* user_prog);

#endif
