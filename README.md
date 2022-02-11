# ForikOS:一个简易的操作系统实现

## 项目介绍
ForikOS是类Linux的操作系统，实现了操作系统的主要功能，有助于理解操作系统原理。

ForikOS是基于nasm汇编和c语言编写的操作系统，实现了操作系统的完整功能。

其主要功能包括：
* 主引导器Boot
* 内核加载器Loader
* 中断机制
* 内存管理
* 进程管理
* IO管理(键盘驱动、硬盘驱动)
* 系统调用
* 文件系统
* 实现shell

## 使用文档
ForikOS使用bochs作为内核调试工具。
## bochs安装步骤如下：
```
安装依赖
yum install kernel-headers kernel-devel qt-devel gtk2-devel textinfo tcl-devel tk-devel
下载bochs
https://nchc.dl.sourceforge.net/project/bochs/bochs/2.6.8/bochs-2.6.8.tar.gz
使用--with-x11编译选项需要安装x window yum groupinstall GNOME Desktop Environment -y; yum groupinstall 'X Window System' -y
sudo ./configure \
        --prefix=/opt/bochs \
        --enable-debugger \
        --enable-disasm \
        --enable-iodebug \
        --enable-x86-debugger \
        --with-x \
        --with-x11
make
sudo make install
导入环境变量
export $PATH:/opt/bochs/bin
```
## bochs常见使用问题
```
出现错误 Cannot Connect to X display
yum groupinstall Desktop //Centos 6.x
yum groups install “GNOME Desktop" //Centos 7.x
yum install xorg*
yum install libX*
```

## 制作虚拟硬盘
```
# 执行生成硬盘镜像命令
========================================================================
                                bximage
  Disk Image Creation / Conversion / Resize and Commit Tool for Bochs
         $Id: bximage.cc 12690 2015-03-20 18:01:52Z vruppert $
========================================================================

1. Create new floppy or hard disk image
2. Convert hard disk image to other format (mode)
3. Resize hard disk image
4. Commit 'undoable' redolog to base image
5. Disk image info

0. Quit

Please choose one [0] 1

Create image

Do you want to create a floppy disk image or a hard disk image?
Please type hd or fd. [hd]

What kind of image should I create?
Please type flat, sparse, growing, vpc or vmware4. [flat]

Enter the hard disk size in megabytes, between 10 and 8257535
[10] 80

What should be the name of the image?
[c.img] hd80M.img

Creating hard disk image 'hd80M.img' with CHS=162/16/63

The following line should appear in your bochsrc:
  ata0-master: type=disk, path="hd80M.img", mode=flat
```

## 磁盘分区
```
# 执行分区命令
fdisk ./hd80M.img
Welcome to fdisk (util-linux 2.23.2).

Changes will remain in memory only, until you decide to write them.
Be careful before using the write command.

Device does not contain a recognized partition table
Building a new DOS disklabel with disk identifier 0x18f97d13.

Command (m for help): m  # 显示帮助信息
Command action
   a   toggle a bootable flag
   b   edit bsd disklabel
   c   toggle the dos compatibility flag
   d   delete a partition
   g   create a new empty GPT partition table
   G   create an IRIX (SGI) partition table
   l   list known partition types
   m   print this menu
   n   add a new partition
   o   create a new empty DOS partition table
   p   print the partition table
   q   quit without saving changes
   s   create a new empty Sun disklabel
   t   change a partition's system id
   u   change display/entry units
   v   verify the partition table
   w   write table to disk and exit
   x   extra functionality (experts only)

Command (m for help): x  # 键入x进入extra functions菜单

Expert command (m for help): m  # 键入m显示子功能菜单
Command action
   b   move beginning of data in a partition
   c   change number of cylinders
   d   print the raw data in the first sector
   e   list extended partitions
   f   fix partition order
   g   create an IRIX (SGI) partition table
   h   change number of heads
   i   change the disk identifier
   m   print this menu
   p   print the partition table
   q   quit without saving changes
   r   return to main menu
   s   change number of sectors/track
   v   verify the partition table
   w   write table to disk and exit

Expert command (m for help): c  # 键入c设置柱面数为162
Number of cylinders (1-1048576, default 10): 162

Expert command (m for help): h  # 键入h设置磁头数为16
Number of heads (1-256, default 255): 16

Expert command (m for help): r  # 键入r返回到上一级菜单

Command (m for help): n  # 键入n创建分区
Partition type:
   p   primary (0 primary, 0 extended, 4 free)
   e   extended
Select (default p): p  # 键入p创建主分区
Partition number (1-4, default 1): 1  # 指定分区号为1
First sector (2048-163295, default 2048): 
Using default value 2048
Last sector, +sectors or +size{K,M,G} (2048-163295, default 163295): 32249
Partition 1 of type Linux and of size 14.8 MiB is set

Command (m for help): n  # 输入n创建分区
Partition type:
   p   primary (1 primary, 0 extended, 3 free)
   e   extended
Select (default p): e  # 键入e创建扩展分区
Partition number (2-4, default 2): 4  # 指定扩展分区号为4
First sector (32250-163295, default 32768): 
Using default value 32768
Last sector, +sectors or +size{K,M,G} (32768-163295, default 163295): 
Using default value 163295
Partition 4 of type Extended and of size 63.8 MiB is set

Command (m for help): p  # 键入p显示分区信息

Disk ./hd80M.img: 83 MB, 83607552 bytes, 163296 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk label type: dos
Disk identifier: 0x5a9653b4

      Device Boot      Start         End      Blocks   Id  System
./hd80M.img1            2048       32249       15101   83  Linux
./hd80M.img4           32768      163295       65264    5  Extended

Command (m for help): n  # 键入n继续创建分区
Partition type:
   p   primary (1 primary, 1 extended, 2 free)
   l   logical (numbered from 5)
Select (default p): l  # 键入l创建逻辑分区
Adding logical partition 5
First sector (34816-163295, default 34816): 
Using default value 34816
Last sector, +sectors or +size{K,M,G} (34816-163295, default 163295): 64498
Partition 5 of type Linux and of size 14.5 MiB is set

Command (m for help): n  # 键入n继续创建分区
Partition type:
   p   primary (1 primary, 1 extended, 2 free)
   l   logical (numbered from 5)
Select (default p): l  # 键入l创建逻辑分区
Adding logical partition 6
First sector (66547-163295, default 67584): 
Using default value 67584
Last sector, +sectors or +size{K,M,G} (67584-163295, default 163295): 96747
Partition 6 of type Linux and of size 14.2 MiB is set

Command (m for help): p  # 键入p显示分区信息

Disk ./hd80M.img: 83 MB, 83607552 bytes, 163296 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk label type: dos
Disk identifier: 0xbcfc08f9

      Device Boot      Start         End      Blocks   Id  System
./hd80M.img1            2048       32249       15101   83  Linux
./hd80M.img4           32768      163295       65264    5  Extended
./hd80M.img5           34816       64498       14841+  83  Linux
./hd80M.img6           67584       96747       14582   83  Linux

Command (m for help): t  # 键入t设置分区id
Partition number (1,4-6, default 6): 5 # 把5号分区id设置为0x66
Hex code (type L to list all codes): 66
Changed type of partition 'Linux' to 'unknown'

Command (m for help): t  # 键入t设置分区id
Partition number (1,4-6, default 6): 6  # 把6号分区id设置为0x66
Hex code (type L to list all codes): 66
Changed type of partition 'Linux' to 'unknown'

Command (m for help): p  # 键入p显示分区信息

Disk ./hd80M.img: 83 MB, 83607552 bytes, 163296 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk label type: dos
Disk identifier: 0xbcfc08f9

      Device Boot      Start         End      Blocks   Id  System
./hd80M.img1            2048       32249       15101   83  Linux
./hd80M.img4           32768      163295       65264    5  Extended
./hd80M.img5           34816       64498       14841+  66  Unknown
./hd80M.img6           67584       96747       14582   66  Unknown

Command (m for help): w  # 键入w将分区表写入磁盘并退出fdisk
The partition table has been altered!

Syncing disks.

```

## 内核编译
```
cd ForikOS
make
make build
```

## 运行内核
```
bochs -f bochsrc
```