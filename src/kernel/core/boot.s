.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    movl $stack_top, %esp
    movl $0x0, %ebp
    andl $0xFFFFFFF0, %esp
    pushl %eax

    call _init

    popl %eax
    pushl %ebx
    pushl %eax
    call kernel_main

    call _fini

    cli # 清除中断
    hlt # 关闭中断
.Lhang:
    jmp .Lhang

.size _start, . - _start
