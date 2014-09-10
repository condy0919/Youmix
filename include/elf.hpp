#pragma once

#include <cstdint>

#include "multiboot.h"

#define ELF32_ST_TYPE(i) ((i)&0xf)

struct elf_section_header_t {
    std::uint32_t name;
    std::uint32_t type;
    std::uint32_t flags;
    std::uint32_t addr;
    std::uint32_t offset;
    std::uint32_t size;
    std::uint32_t link;
    std::uint32_t info;
    std::uint32_t addralign;
    std::uint32_t entsize;
} __attribute__((packed));

struct elf_symbol_t {
    std::uint32_t name;
    std::uint32_t value;
    std::uint32_t size;
    std::uint8_t info;
    std::uint8_t other;
    std::uint16_t shndx;
} __attribute__((packed));

class elf_t {
public:
    elf_t(multiboot_t *);
    const char *operator[](std::uint32_t);

private:
    const char *lookup_symbol(std::uint32_t);

private:
    elf_symbol_t *symtab;
    std::uint32_t symtabsize;
    const char *strtab;
    std::uint32_t strtabsize;
};
