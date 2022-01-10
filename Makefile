TOP = ${HOME}/src/boot/src/ForikOS
CC = gcc
AS = nasm
LD = ld
ENTRY_POINT = 0xc0001500

TARGET = kernel.bin
LOADER = loader.bin
BOOTER = booter.bin

IMPORT_INCLUDE += -I $(TOP)/include
CFLAGS = -Wall -c -fno-builtin -W -Wstrict-prototypes -Wmissing-prototypes
# LDFLAGS = -Ttext $(ENTRY_POINT) -e main -Map $(TOP)/kernel.map -m elf_i386
LDFLAGS = -m elf_i386 -Ttext 0xc0001500 -e main
ASFLAGS = -f elf
ASMKFLAGS = -I boot/include/

all:$(TARGET) $(LOADER) $(BOOTER)

SRC_PATH += $(TOP)/main
SRC_PATH += $(TOP)/kernel
SRC_PATH += $(TOP)/lib
SRC_PATH += $(TOP)/device
SRC_PATH += $(TOP)/thread


DIRS = $(shell find $(SRC_PATH) -maxdepth 3 -type d)
SRC_C += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
SRC_ASM += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.asm))

# C_SRC_=kernel/main.c kernel/memory.c
# C_OBJ=${C_SRC:.c=.o}
OBJS_C = $(patsubst %.c, %.o, $(SRC_C))
OBJS_ASM = $(patsubst %.asm, %.o, $(SRC_ASM))

# $(TARGET):$(OBJS_ASM) $(OBJS_C)
$(TARGET):$(OBJS_C) $(OBJS_ASM)  # 注意这里顺序很重要
	@echo "linking $^ ==> $@..."
	$(LD) $(LDFLAGS) -o $@ $^
	@echo

.c.o:
	@echo "compiling $< ==> $@..."
	$(CC) $(IMPORT_INCLUDE) -m32 -c -o $*.o $*.c
	@echo

# .asm.o:
$(OBJS_ASM): %.o: %.asm
	@echo "asming $< ==> $@..."
	$(AS) $(ASFLAGS) -o $@ $<
	@echo

############ boot汇编代码 ##############
# $(BOOTER) : boot/boot.asm boot/include/boot.inc
# bximage -hd -mode="flat" -size=60 -q hd60M.img
$(BOOTER) : boot/boot.asm 
	$(AS) $(ASMKFLAGS) -o $@ $<

$(LOADER) : boot/loader.asm
	$(AS) $(ASMKFLAGS) -o $@ $<

build:
	dd if=booter.bin of=hd60M.img bs=512 count=1 conv=notrunc
	dd if=loader.bin of=hd60M.img bs=512 count=3 seek=2 conv=notrunc
	dd if=kernel.bin of=hd60M.img bs=512 count=200 seek=9 conv=notrunc

clean:
	rm -rf $(OBJS_C)
	rm -rf $(OBJS_ASM)
	rm -rf $(TARGET)
	rm -rf $(LOADER)
	rm -rf $(BOOTER)
