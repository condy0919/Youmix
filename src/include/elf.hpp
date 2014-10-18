#pragma once

#include <stdint.h>
#include <stddef.h>

#include "multiboot.h"

#define ELF32_ST_TYPE(i) ((i)&0xf)

struct elf_section_header_t {
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    uint32_t addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
} __attribute__((packed));

struct elf_symbol_t {
    uint32_t name;
    uint32_t value;
    uint32_t size;
    uint8_t info;
    uint8_t other;
    uint16_t shndx;
} __attribute__((packed));

class Elf_info {
public:
    Elf_info(multiboot_info_t *);
    const char *lookup_symbol(uint32_t);

private:
    elf_symbol_t *symtab;
    uint32_t symtabsize;
    const char *strtab;
    uint32_t strtabsize;
};
