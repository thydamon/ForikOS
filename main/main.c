// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: main.c
// Create Time: Wed 29 Dec 2021 09:59:21 PM CST
// This is ... 

#include "kernel/init.h"
#include "lib/print.h"
#include "kernel/interrupt.h"
#include "thread/thread.h"
#include "device/console.h"
#include "user/process.h"
#include "user/syscall.h"
#include "user/syscall_init.h"
#include "lib/stdio.h"
#include "mm/memory.h"
#include "fs/dir.h"
#include "fs/fs.h"
#include "user/assert.h"
#include "shell/shell.h"
#include "kernel/printk.h"

// 注意main函数之上只能给出函数声明,不能有函数地址
// 是因为定义了函数会改变函数的入口地址,汇编中函数的入口地址变成了k_thread_a
void init(void);

int main(void) 
{
   put_str("I am kernel\n");
   init_all();
   uint32_t file_size = 6184;
   uint32_t sec_cnt = DIV_ROUND_UP(file_size, 512);
   struct disk* sda = &channels[0].devices[0];
   void* prog_buf = sys_malloc(file_size);
   ide_read(sda, 300, prog_buf, sec_cnt);
   int32_t fd = sys_open("/prog_no_arg", O_CREAT|O_RDWR);
   if (fd != -1) {
      if(sys_write(fd, prog_buf, file_size) == -1) {
	 printk("file write error!\n");
   while(1);
      }
   }
   cls_screen();
   console_put_str("[ForikOS@localhost /]$ ");
   while(1);
   return 0;
}

// init进程
void init(void)
{
   uint32_t ret_pid = fork();
   if(ret_pid) {  // 父进程
      while(1);
   } else {	  // 子进程
      my_shell();
   }
   panic("init: should not be here");
}
