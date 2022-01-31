// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: shell/buildin_cmd.h
// Create Time: Mon 31 Jan 2022 11:14:49 AM CST
// This is ... 

#ifndef BUILDIN_CMD_H_
#define BUILDIN_CMD_H_

#include "lib/stdint.h"

void buildin_ls(uint32_t argc, char** argv);
char* buildin_cd(uint32_t argc, char** argv);
int32_t buildin_mkdir(uint32_t argc, char** argv);
int32_t buildin_rmdir(uint32_t argc, char** argv);
int32_t buildin_rm(uint32_t argc, char** argv);
void make_clear_abs_path(char* path, char* wash_buf);
void buildin_pwd(uint32_t argc, char** argv);
void buildin_ps(uint32_t argc, char** argv);
void buildin_clear(uint32_t argc, char** argv);

#endif
