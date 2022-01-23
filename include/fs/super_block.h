// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: super_block.h
// Create Time: Sun 23 Jan 2022 12:45:45 PM CST
// This is ... 

#ifndef SUPER_BLOCK_H_
#define SUPER_BLOCK_H_

#include "lib/stdint.h"

struct super_block
{
    uint32_t magic;               // 用来标识文件系统类型,支持多文件系统的操作系统通过此标志来识别文件系统类型
    uint32_t sec_cnt;             // 本分区总共的扇区数
    uint32_t inode_cnt;           // 本分区中inode数量
    uint32_t part_lba_base;       // 本分区的起始lba地址

    uint32_t block_bitmap_lba;    // 块位图本身起始扇区地址
    uint32_t block_bitmap_sects;  // 扇区位图本身占用的扇区数量

    uint32_t inode_bitmap_lba;    // i结点位图起始扇区lba地址
    uint32_t inode_bitmap_sects;  // i结点位图占用的扇区数量

    uint32_t inode_table_lba;     // i结点表起始扇区lba地址
    uint32_t inode_table_sects;   // i结点表占用的扇区数量

    uint32_t data_start_lba;      // 数据区开始的第一个扇区号
    uint32_t root_inode_no;       // 根目录所在的I结点号
    uint32_t dir_entry_size;      // 目录项大小
    
    // 为方便些程序,数据块大小与扇区大小一致,即1块等于1扇区
    // 磁盘操作要以扇区为单位,咱们交给硬盘的数据必须是扇区大小的整数倍
    uint8_t  pad[460];            // 加上460字节,凑够512字节1扇区大小
} __attribute__ ((packed));

#endif
