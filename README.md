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
* IO管理
* 系统调用

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