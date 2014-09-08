; GO!

; Multiboot magic number
MBOOT_HEADER_MAGIC          equ 0x1BADB002

; 0号位表示所有的引导模块，将按页(4K)边界对齐
MBOOT_PAGE_ALIGN            equ 1 << 0

; 1号位通过 Multiboot 信息结构的mem_*域包括可用内存的信息
; 告诉(把内存空间的信息包含在信息结构中GRUBMultiboot)
MBOOT_MEM_INFO              equ 1 << 1

; 定义我们使用的 Multiboot 标记
MBOOT_HEADER_FLAGS          equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

; 校验. magic + flags + checksum = 0;
MBOOT_CHECKSUM              equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

; 符合规范的 Multiboot OS映像需要这样一个 magic Multiboot 头
; 
; 0 u32 magic
; 4 u32 flags
; 8 u32 checksum
;

[BITS 32]
section .text

; 设置符合 Multiboot 规范的标记
dd MBOOT_HEADER_MAGIC
dd MBOOT_HEADER_FLAGS
dd MBOOT_CHECKSUM

[GLOBAL start] ; 内核入口函数
[GLOBAL glb_mboot_ptr] ; 声明 struct multiboot* 变量
[EXTERN kern_entry] ; 声明内核C代码的入口函数

start:
    cli ; 此时还没有设置好保护模式的中断处理，要关闭中断

    mov esp, STACK_TOP
    mov ebp, 0
    and esp, 0FFFFFFF0H
    mov [glb_mboot_ptr], ebx ; GRUB把相关信息的地址放在了ebx中
    call kern_entry

stop:
    hlt
    jmp stop

section .bss
stack:
    resb 32768 ; 内核栈
glb_mboot_ptr: ; 全局 multiboot 结构体指针
    resb 4

STACK_TOP equ $-stack-1
