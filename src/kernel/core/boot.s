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

    leal glb_mboot_ptr, %eax
    movl %ebx, (%eax)

    call _init # run global constructors

    call init_gdt
    call init_idt

    popl %eax
    pushl %ebx # ptr to multiboot_info_t
    pushl %eax # magic
    call kernel_main

    call _fini # run global destructors

    cli # clear interrupts
    hlt # halt
.Lhang:
    jmp .Lhang

.size _start, . - _start
