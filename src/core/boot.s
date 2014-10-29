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
.local start
.type start, @function
.global _start
.set _start, (start - 0xc0000000)
.type _start, @function
start:
    leal _ZN6Memory14page_directoryE, %ecx # Memory::page_directory
    #leal page_directory, %ecx
    subl $0xc0000000, %ecx # Physical address
    movl %ecx, %cr3
    movl $0x83, (%ecx) # 4MB PAGESIZE | PRESENT | WRITABLE
    movl $0x300, %edx
    movl $0x83, (%ecx, %edx, 4)

    movl %cr4, %ecx
    or $0x10, %ecx # Set PSE bit to enable 4MB pages.
    movl %ecx, %cr4 

    movl %cr0, %ecx
    or $0x80000000, %ecx # Set PG bit to enable paging.
    movl %ecx, %cr0

    # Start fetching instructions in kernel space.
    # Since eip at this point holds the Physical address of the command(approximately 0x00100000)
    # we need to do a long jump to the correct virtual address of StartInHighHalf 
    # which is approximately 0xc0100000.
    leal StartInHighHalf, %ecx
    jmp *%ecx

StartInHighHalf:
    # unmap the identify-mapped first 4MB of Physical address space.
    # It should not be needed anymore.
    # FIXME: It makes the global constructor fails.
    #leal page_directory, %ecx
    #movl $0, (%ecx)
    #invlpg 0

    # From now on, paging should be enable. The first 4MB of Physical address space is
    # mapped starting at KERNEL_VIRTUAL_BASE. Everything is linked to this, so no more
    # position-independent code or funny business with virtual-to-physical address
    # translation should be necessary. we now have a higher-half kernel.

    movl $stack_top, %esp
    movl $0x0, %ebp
    andl $0xFFFFFFF0, %esp

    leal glb_mboot_ptr, %ecx
    movl %ebx, (%ecx)

    call init_page_dir

    call _init # call global constructors

    call init_gdt
    call init_idt


    #popl %eax
    #pushl %ebx # ptr to multiboot_info_t
    #pushl %eax # magic
    call kernel_main

    call _fini # call global destructors

    cli # clear interrupts
    hlt # halt
.Lhang:
    jmp .Lhang

.size _start, . - start
