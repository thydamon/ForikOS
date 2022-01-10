// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: print.h
// Create Time: Mon 27 Dec 2021 08:05:23 PM CST
// This is ... 

#ifndef PRINT_H_
#define PRINT_H_

#include "stdint.h"

void put_char(uint8_t char_asci);
void put_str(char* msg);
void put_int(uint32_t num);
void set_cursor(uint32_t cursor_pos);

#endif
