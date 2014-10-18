ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    /*. = 1M;*/
    . = 0xc0100000;
    .text ALIGN(4K) : AT(ADDR(.text) - 0xc0000000)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xc0000000)
    {
        *(.rodata)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - 0xc0000000)
    {
        *(.data)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - 0xc0000000)
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }
    .stab ALIGN(4K) : AT(ADDR(.stab) - 0xc0000000)
    {
        *(.stab)
    }
    .stabstr ALIGN(4K) : AT(ADDR(.stabstr) - 0xc0000000)
    {
        *(.stabstr)
    }

    /DISCARD/ :
    {
        *(.comment) 
    }
}
