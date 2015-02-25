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

    .rodata ALIGN(4K) :
    {
        *(.rodata)
    }

    .data ALIGN(4K) :
    {
        *(.data)
    }

    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }
    .stab ALIGN(4K) :
    {
        *(.stab)
    }
    .stabstr ALIGN(4K) :
    {
        *(.stabstr)
    }

    /DISCARD/ :
    {
        *(.comment) 
    }
}
