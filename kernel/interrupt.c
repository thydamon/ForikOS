// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: interrupt.c
// Create Time: Mon 27 Dec 2021 08:12:29 PM CST
// This is ... 

#include "kernel/interrupt.h"
#include "stdint.h"
#include "global.h"
#include "kernel/io.h"
#include "lib/print.h"


#define PIC_M_CTRL 0x20     // 可编程中断控制器8259A,主片的控制端口是0x20
#define PIC_M_DATA 0x21     // 主片的数据端口0x21
#define PIC_S_CTRL 0xa0     // 从片的控制端口0xa0
#define PIC_S_DATA 0xa1     // 从片的数据端口0xa0

#define IDT_DESC_CNT 0x30   // 目前总共支持的中断数,33个中断

#define EFLAGS_IF 0x00000200  // efig寄存器中if位为1
#define GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" : "=g" (EFLAG_VAR))

struct gate_desc
{
    uint16_t  func_offset_low_word;
    uint16_t  selector;
    uint8_t   dcount;
    uint8_t   attribute;
    uint16_t  func_offset_high_word;
};

static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function);
// 中断描述符表
static struct gate_desc idt[IDT_DESC_CNT];

// 用于保存异常的名字
char* intr_name[IDT_DESC_CNT];
// 中断处理程序数组,在kernel.asm中定义的intrXXentry只是中断处理程序的入口,最终调用实在idt_table中的处理程序
intr_handler idt_table[IDT_DESC_CNT];
// 引用定义在kernel.asm中的中断处理函数入口数组
extern intr_handler intr_entry_table[IDT_DESC_CNT];

// 初始化中断控制器8259A
static void pic_init(void)
{
  // 初始化主片
  outb(PIC_M_CTRL, 0x11);   // ICW1: 边沿触发,级联8259, 需要ICW4
  outb(PIC_M_DATA, 0x20);   // ICW2: 起始中断向量号为0x20,也就是IR[0-7] 为 0x20 ~ 0x27
  outb(PIC_M_DATA, 0x04);   // ICW3: IR2接从片
  outb(PIC_M_DATA, 0x01);   // ICW4: 8086模式, 正常EOI

  // 初始化从片
  outb(PIC_S_CTRL, 0x11);   // ICW1: 边沿触发,级联8259, 需要ICW4
  outb(PIC_S_DATA, 0x28);   // ICW2: 起始中断向量号为0x28,也就是IR[8-15] 为 0x28 ~ 0x2F
  outb(PIC_S_DATA, 0x02);   // ICW3: 设置从片连接到主片的IR2引脚
  outb(PIC_S_DATA, 0x01);   // ICW4: 8086模式, 正常EOI

  // 打开主片上IR0,也就是目前只接受时钟产生的中断
  outb (PIC_M_DATA, 0xfe);
  outb(PIC_S_DATA, 0xff);

  put_str("  pic_init done.\n");
}

// 创建中断门描述符
static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function)
{
    p_gdesc->func_offset_low_word = (uint32_t)function & 0x0000FFFF;
    p_gdesc->selector = SELECTOR_K_CODE;
    p_gdesc->dcount = 0;
    p_gdesc->attribute = attr;
    p_gdesc->func_offset_high_word = ((uint32_t)function & 0xFFFF0000) >> 16; 
}

// 初始化中断描述符表
static void idt_desc_init(void)
{
  int i = 0;
  for (i = 0; i < IDT_DESC_CNT; i++)
  {
      make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_entry_table[i]); 
  }

  put_str("   idt_desc_init done.\n");
}

// 通用的中断处理函数
static void general_intr_handler(uint8_t vec_nr)
{
  if (vec_nr == 0x27 || vec_nr == 0x2f)
  {
      return;
  }

  set_cursor(0);
  int cursor_pos = 0;

  while (cursor_pos < 320)
  {
      put_char(' ');
      cursor_pos++;
  }

  set_cursor(0);
  put_str("!!!!!!!!!   excetion message begin!!!\n");
  set_cursor(88);
  put_str(intr_name[vec_nr]);
  if (vec_nr == 14)
  {
      int page_fault_vaddr = 0;
      asm volatile("movl %%cr2, %0" : "=r" (page_fault_vaddr));
      put_str("\npage fault addr is ");
      put_int(page_fault_vaddr);
  }

  put_str("!!!!!!!!!   excetion message end!!!\n");

  // 不会出现调度进程的情况。故下面的死循环不会再被中断。
  while (1);
}

// 完成一般中断处理程序注册及异常名称注册
static void exception_init(void)
{
    int i = 0;
    for (i = 0; i < IDT_DESC_CNT; i++)
    {
      idt_table[i] = general_intr_handler;		    // 默认为general_intr_handler。

      intr_name[i] = "unknown";				    // 先统一赋值为unknown 
    }

    intr_name[0] = "#DE Divide Error";
    intr_name[1] = "#DB Debug Exception";
    intr_name[2] = "NMI Interrupt";
    intr_name[3] = "#BP Breakpoint Exception";
    intr_name[4] = "#OF Overflow Exception";
    intr_name[5] = "#BR BOUND Range Exceeded Exception";
    intr_name[6] = "#UD Invalid Opcode Exception";
    intr_name[7] = "#NM Device Not Available Exception";
    intr_name[8] = "#DF Double Fault Exception";
    intr_name[9] = "Coprocessor Segment Overrun";
    intr_name[10] = "#TS Invalid TSS Exception";
    intr_name[11] = "#NP Segment Not Present";
    intr_name[12] = "#SS Stack Fault Exception";
    intr_name[13] = "#GP General Protection Exception";
    intr_name[14] = "#PF Page-Fault Exception";

    intr_name[16] = "#MF x87 FPU Floating-Point Error";
    intr_name[17] = "#AC Alignment Check Exception";
    intr_name[18] = "#MC Machine-Check Exception";
    intr_name[19] = "#XF SIMD Floating-Point Exception";
}

// 开中断并返回开中断状态
enum intr_status intr_enable() 
{
    enum intr_status old_status;

    if (INTR_ON == intr_get_status())
    {
        old_status = INTR_ON;

        return old_status;
    }

    else
    {
        old_status = INTR_OFF;
        asm volatile("sti");

        return old_status;
    }
}

// 关中断,并且返回关中断前的状态
enum intr_status intr_disable()
{
    enum intr_status old_status;

    if (INTR_ON == intr_get_status())
    {
        old_status = INTR_ON;
        asm volatile("cli" : : : "memory");  // 关中断,cli指令将IF位置0

        return old_status;
    }
    else
    {
        old_status = INTR_OFF;

        return old_status;
    }
}

// 设置中断状态
enum intr_status intr_set_status(enum intr_status status)
{
    return status & INTR_ON ? intr_enable() : intr_disable();
}

// 获取当前中断状态
enum intr_status intr_get_status()
{
    uint32_t eflags = 0;

    GET_EFLAGS(eflags);

    return (EFLAGS_IF & eflags) ? INTR_ON : INTR_OFF;
}

// 中断处理程序数组第 vector_no 个元素中
// 注册安装中断处理程序 function
void register_handler(uint8_t vector_no, intr_handler function)
{
    // idt _table 数组中的函数是在进入中断后根据中断向量号调用的
    idt_table[vector_no] = function;
}

void idt_init()
{
    put_str("  idt_init start\n");
    idt_desc_init();
    exception_init();
    pic_init();

    // 加载idt
    // IDTR为48位寄存器,低16位为IDT数组的大小,高32位是IDT的线性基地址
    uint64_t idt_operand = ((sizeof(idt) - 1) | ((uint64_t)(uint32_t)idt << 16));
    // %0表示&idt_oprand
    asm volatile("lidt %0" : : "m" (idt_operand));
    put_str(" idt_init done\n");
}


