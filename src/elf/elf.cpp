#include "elf.hpp"
#include "string.hpp"

elf_t::elf_t(multiboot_t *mb) {
    elf_section_header_t *sh = (elf_section_header_t *)mb->addr;
    std::uint32_t shstrtab = sh[mb->shndx].addr;

    for (std::size_t i = 0; i < mb->num; ++i) {
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

const char *elf_t::operator[](std::uint32_t addr) {
    return lookup_symbol(addr);
}

const char *elf_t::lookup_symbol(std::uint32_t addr) {
    for (std::size_t i = 0; i < symtabsize / sizeof(elf_symbol_t); ++i) {
        if (ELF32_ST_TYPE(symtab[i].info) != 0x2) // function
            continue;
        if (addr >= symtab[i].value && addr < symtab[i].value + symtab[i].size)
            return (const char *)((std::uint32_t)strtab + symtab[i].name);
    }
    return 0;
}

