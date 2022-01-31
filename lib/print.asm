TI_GDT equ 0
RPL0   equ 0
; 视频选择子
; 选择字位16位
; 0~2bit: RPL
; 3bit: 0-GDT,1-LDT
; 3~15bit: 2^8192个描述符
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0

section .data
put_int_buffer dq 0 ; 定义8字节缓冲区用于数字到字符的转换

[bits 32]
section .text

global put_str
put_str:
  ; 本函数只用到ebx和ecx寄存器,备份
  push ebx
  push ecx
  xor  ecx, ecx
  mov  ebx, [esp + 12]  ; 从栈中得到打印字符串的地址,备份两个寄存器4*2=8,函数返回地址4字节,总计12个字节
                        ; ebx获取到了字符串的首地址

; 遍历每个字符串调用put_char
.goon:
  mov  cl, [ebx]
  cmp  cl, 0  ; 遇到字符串结束符返回
  jz   .str_over
  push ecx    ; 为put_char传递参数
  call put_char
  add  esp, 4 ; 回收参数所占的栈空间
  inc ebx		      ; 使ebx指向下一个字符
  jmp .goon
.str_over:
  pop ecx
  pop ebx
  ret

global put_char
put_char:
  pushad  ; 备份32位寄存器环境
   ;需要保证gs中为正确的视频段选择子,为保险起见,每次打印时都为gs赋值
  mov ax, SELECTOR_VIDEO	       ; 不能直接把立即数送入段寄存器
  mov  gs, ax

  ; 获取当前光标位置,光标寄存器分为高8位和低8位
  ; 先获取高8位,高8位寄存器索引是0x0e
  mov  dx, 0x03d4  ; 首先保存端口索引0x03d4
  mov  al, 0x0e
  out  dx, al
  mov  dx, 0x03d5  ; 然后往端口地址为0x03d5写入索引
  in   al, dx      ; 得到光标位置的高8位,对于in指令,如果寄存器为8位,必须为低8位
  mov  ah, al

  ; 获取低8位,低8位寄存器索引是0x0f
  mov  dx, 0x03d4
  mov  al, 0x0f
  out  dx, al
  mov  dx, 0x03d5
  in   al, dx

  ; 将光标存入bx
  mov  bx, ax  ; 习惯性做法,使用bx作为基址寻址寄存器
  ; 在栈中取待打印的字符
  mov ecx, [esp + 36]  ; pushad压入4*8=32个字节,主调函数返回4字节,故esp+36,此时ecx寄存器保存了需要打印的字符
  cmp  cl, 0xd         ; CR(回车)是0x0d,LF(换行)是0x0a,ecx保存了打印的字符,一个字符只需要取cl一个字节8位即可
  jz  .is_carriage_return
  cmp  cl, 0xa
  jz  .is_line_feed

  cmp  cl, 0x8         ; BS(backspace)asci码是8
  jz  .is_backspace
  jmp .put_other

.is_backspace:
  ; 当为backspace时,本质上是将光标向前移动一个显存位置即可,后面输入的字符会覆盖此处的字符
  ; 但是如果后面没有输入字符,需要用空格或0覆盖
  dec  bx  ; bx-1,光标向前移动一个字符
  shl  bx, 1 ; shl左移1位相当于*2
  mov byte [gs:bx], 0x20 ; 显存低8位写入字符空格(0x20)
  inc  bx  ; inc指令bx+1, bx便指向了属性位置, 下面写入属性0x07(黑底白字)
  mov byte [gs:bx], 0x07 ; 显存高0位写入属性0x07
  shr  bx, 1 ; 与shl bx, 1 相对应,用于恢复光标位置,此时bx便指向新恢复的空格
  jmp .set_cursor

.put_other:
  shl  bx, 1  ; 光标位置用2字节表示,光标值乘2,表示对应显存中的偏移字节
  mov [gs:bx], cl ; 低8位打印字符的ASCI码
  inc  bx     ; bx指向属性位置
  mov byte [gs:bx], 0x07 ; 属性赋值
  shr  bx, 1  ; 恢复光标值
  inc  bx     ; 指向下一个光标值
  cmp  bx, 2000 ; 80(列)*25(行)=2000(字)
  jl  .set_cursor ; 若光标值小于2000表示未到最后,设置新的光标值,大于2000则需要换行

.is_line_feed: ; 如果是换行符\n(0x0a)
.is_carriage_return: ; 如果是回车符\r(0x0d),只需把光标移到行首a
  xor  dx, dx  ; dx是被除数的高16位,清零
  mov  ax, bx  ; ax是除数的低16位
  mov  si, 80  ; linux系统中\n和\r都处理成光标指向行首
  div  si
  sub  bx, dx  ; 光标值减去除以80后的余数

; 到行尾需要换行
.is_carriage_return_end:   ; 回车符CR处理结束
  add  bx, 80
  cmp  bx, 2000

.is_line_feed_end:
  jl  .set_cursor

; 屏幕行范围是0~24,滚屏的原理是将屏幕的第1~24行搬运到0~23行,再将24行用空格填充
.roll_screen:
  cld               ; 清除方向键,就是把eflags寄存器中方向标志位DF清0
  mov  ecx, 960     ; 用来控制rep重复执行的次数
                    ; 1~24行的字符整体往上提,复制到0~23行,要复制的字符数是2000-50=1920个,每个字符是2字节,共3840字节
                    ; movsd指令每次复制4字节数据,3840/4=960
  mov esi, 0xc00b80a0			  ; 第1行行首
  mov edi, 0xc00b8000			  ; 第0行行首
  rep  movsd
  
  ; 将最后一行用空格填充
  mov  ebx, 3840    ; 最后一行的首字符的第一个字节偏移=1920*2=2840
  mov  ecx, 80      ; 一行是80字符(160字节),每次清空1字符(2字节),一行需要移动80次
; 开始循环清空最后一行
.cls:
  mov  word [gs:ebx], 0x0720 ; 0x07-属性黑底白字,0x20空格字符
  add  ebx, 2       ; 每次处理2个字节
  loop .cls
  mov  bx,  1920    ; 将光标值重置为1920,最后一行的首字符

; 将光标寄存器设置为bx中的值
.set_cursor:
  ; 先设置高8位
  mov  dx, 0x03d4   ; 0x03d4端口写入待操作寄存器索引
  mov  al, 0x0e
  out  dx, al
  mov  dx, 0x03d5
  mov  al, bh
  out  dx, al

  ; 再设置低8位
  mov  dx, 0x03d4
  mov  al, 0x0f
  out  dx, al
  mov  dx, 0x03d5
  mov  al, bl
  out  dx, al

.put_char_done:
  popad
  ret

global cls_screen
cls_screen:
    pushad

    mov ax, SELECTOR_VIDEO          ; 不能直接把立即数送入gs,须由ax中转
    mov gs, ax

    mov ebx, 0
    mov ecx, 80*25
.cls:
    mov word [gs:ebx], 0x0720          ;0x0720是黑底白字的空格键
    add ebx, 2
    loop .cls 
    mov ebx, 0

.set_cursor:                 ;直接把set_cursor搬过来用,省事
    ; 1 先设置高8位
    mov dx, 0x03d4             ;索引寄存器
    mov al, 0x0e               ;用于提供光标位置的高8位
    out dx, al
    mov dx, 0x03d5             ;通过读写数据端口0x3d5来获得或设置光标位置 
    mov al, bh
    out dx, al

    ; 2 再设置低8位
    mov dx, 0x03d4
    mov al, 0x0f
    out dx, al
    mov dx, 0x03d5 
    mov al, bl
    out dx, al
    popad
    ret

global put_int
put_int:
  pushad
  mov  ebp, esp
  mov  eax, [ebp+4*9]  ; call的返回地址占4字节+pushad的8个4字节
  mov  edx, eax
  mov  edi, 7
  mov  ecx, 8
  mov  ebx, put_int_buffer

.16based_4bits:
  and  edx, 0x0000000F
  cmp  edx, 9
  jg   .is_A2F
  add  edx, '0'
  jmp  .store

.is_A2F:
  sub  edx, 10
  add  edx, 'A'

.store:
  mov  [ebx+edi], dl
  dec  edi
  shr  eax, 4
  mov  edx, eax
  loop .16based_4bits

.ready_to_print:
  inc  edi

.skip_prefix_0:
  cmp  edi, 8
  je   .full0

.go_on_skip:
  mov  cl, [put_int_buffer+edi]
  inc  edi
  cmp  cl, '0'
  je   .skip_prefix_0
  dec  edi
  jmp  .put_each_num

.full0:
  mov  cl, '0'

.put_each_num:
  push  ecx
  call  put_char
  add   esp, 4
  inc   edi
  mov   cl, [put_int_buffer+edi]
  cmp   edi, 8
  jl    .put_each_num
  popad
  ret

global set_cursor
set_cursor:
   pushad
   mov bx, [esp+36]
;;;;;;; 1 先设置高8位 ;;;;;;;;
   mov dx, 0x03d4			  ;索引寄存器
   mov al, 0x0e				  ;用于提供光标位置的高8位
   out dx, al
   mov dx, 0x03d5			  ;通过读写数据端口0x3d5来获得或设置光标位置 
   mov al, bh
   out dx, al

;;;;;;; 2 再设置低8位 ;;;;;;;;;
   mov dx, 0x03d4
   mov al, 0x0f
   out dx, al
   mov dx, 0x03d5 
   mov al, bl
   out dx, al
   popad
   ret
