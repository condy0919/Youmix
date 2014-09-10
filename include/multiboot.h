#pragma once

#include <cstdint>

struct multiboot_t {
    std::uint32_t flags; // Multiboot 版本信息

    /*
     * 从BIOS获的可用内存, 单位K.
     */
    std::uint32_t mem_lower;
    std::uint32_t mem_upper;

    std::uint32_t boot_device; // 引导设备
    std::uint32_t cmdline;
    std::uint32_t mods_count; // 模块数
    std::uint32_t mods_addr;

    // elf
    std::uint32_t num;
    std::uint32_t size;
    std::uint32_t addr;
    std::uint32_t shndx; // section名字的映射，为了压缩而存在.

    // BIOS提供的缓冲区
    std::uint32_t mmap_length;
    std::uint32_t mmap_addr;

    std::uint32_t drives_length; // 第一个驱动器
    std::uint32_t drives_addr;
    std::uint32_t config_table; // ROM配置
    std::uint32_t boot_loader_name;
    std::uint32_t apm_table;
    std::uint32_t vbe_control_info;
    std::uint32_t vbe_mode_info;
    std::uint32_t vbe_mode;
    std::uint32_t vbe_interface_seg;
    std::uint32_t vbe_interface_off;
    std::uint32_t vbe_interface_len;
} __attribute__((packed));


struct mmap_entry_t {
    std::uint32_t size; // sizeof(Mmap_entry) - sizeof(size), 单位byte.
    std::uint32_t base_addr_low;
    std::uint32_t base_addr_high;
    std::uint32_t length_low;
    std::uint32_t length_high;
    std::uint32_t type; // 地址区间类型
} __attribute__((packed));

extern multiboot_t *glb_mboot_ptr;


