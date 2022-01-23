// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: dir.h
// Create Time: Sun 23 Jan 2022 01:02:16 PM CST
// This is ... 

#ifndef DIR_H_
#define DIR_H_

#include "lib/stdint.h"
#include "fs/inode.h"
#include "fs/fs.h"
#include "device/ide.h"
#include "global.h"

#define MAX_FILE_NAME_LEN  16    // 最大文件名长度

// 目录结构
struct dir
{
    struct inode* inode;
    uint32_t dir_pos;      // 记录在目录内的偏移
    // 目录的数据缓存,如读取目录时,用来存储返回的目录项
    uint8_t dir_buf[512];  // 目录的数据缓存
};

// 目录项结构,它是连接文件名与inode的纽带,成员filename是文件名
struct dir_entry
{
    char filename[MAX_FILE_NAME_LEN];  // 普通文件或目录名称
    uint32_t i_no;                     // 普通文件或目录对应的inode编号
    enum file_types f_type;            // 文件类型
};

#endif
