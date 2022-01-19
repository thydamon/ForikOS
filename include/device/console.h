// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: console.h
// Create Time: Wed 12 Jan 2022 08:50:55 PM CST
// This is ... 

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "stdint.h"

void console_init(void);
void console_acquire(void);
void console_release(void);
void console_put_str(char* str);
void console_put_char(uint8_t char_asci);
void console_put_int(uint32_t num);

#endif
