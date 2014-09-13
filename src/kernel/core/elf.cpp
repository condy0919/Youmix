#include "elf.hpp"
#include "../libs/string.h"

Elf_info::Elf_info(multiboot_info_t *mb) {
    //elf_section_header_t *sh = (elf_section_header_t *)mb->u.elf_sec.addr;
    elf_section_header_t *sh = (elf_section_header_t *)mb->addr;
    uint32_t shstrtab = sh[mb->shndx].addr;
    for (size_t i = 0; i < mb->num; ++i) {
        const char *name = (const char *)(shstrtab + sh[i].name);
        if (strcmp(name, ".strtab") == 0) {
            strtab = (const char *)sh[i].addr;
            strtabsize = sh[i].size;
        } else if (strcmp(name, ".symtab") == 0) {
            symtab = (elf_symbol_t *)sh[i].addr;
            symtabsize = sh[i].size;
        }
    }
}

const char *Elf_info::lookup_symbol(uint32_t addr) {
    for (size_t i = 0; i < symtabsize / sizeof(elf_symbol_t); ++i) {
        if (ELF32_ST_TYPE(symtab[i].info) != 0x2) // function
            continue;
        if (addr >= symtab[i].value && addr < symtab[i].value + symtab[i].size)
            return (const char *)((uint32_t)strtab + symtab[i].name);
    }
    return 0;
}
