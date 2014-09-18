#include <stdint.h>
#include <stddef.h>

#include "idt.hpp"
#include "ostream.hpp"

InterruptDescriptor idtdesc[256];

IDTPointer idt_ptr;

decltype(isr_handler) *interrupt_handlers[256];

void init_idt() {
    auto fn = [](uint8_t id, uint32_t base, uint16_t selector, uint8_t flags) {
        idtdesc[id].offset0_15 = (base & 0x0000ffff) >> 0;
        idtdesc[id].offset16_31 = (base & 0xffff0000) >> 16;
        idtdesc[id].selector = selector;
        idtdesc[id].always0 = 0;
        idtdesc[id].flags = flags;// | 0x60; // 用户态时设置中断门级别为3
    };

    idt_ptr.limit = sizeof(idtdesc);
    idt_ptr.base = (uint32_t)idtdesc;

    // 用于CPU的中断处理
    fn(0, (uint32_t)isr0, 0x08, 0x8e);
    fn(1, (uint32_t)isr1, 0x08, 0x8e);
    fn(2, (uint32_t)isr2, 0x08, 0x8e);
    fn(3, (uint32_t)isr3, 0x08, 0x8e);
    fn(4, (uint32_t)isr4, 0x08, 0x8e);
    fn(5, (uint32_t)isr5, 0x08, 0x8e);
    fn(6, (uint32_t)isr6, 0x08, 0x8e);
    fn(7, (uint32_t)isr7, 0x08, 0x8e);
    fn(8, (uint32_t)isr8, 0x08, 0x8e);
    fn(9, (uint32_t)isr9, 0x08, 0x8e);
    fn(10, (uint32_t)isr10, 0x08, 0x8e);
    fn(11, (uint32_t)isr11, 0x08, 0x8e);
    fn(12, (uint32_t)isr12, 0x08, 0x8e);
    fn(13, (uint32_t)isr13, 0x08, 0x8e);
    fn(14, (uint32_t)isr14, 0x08, 0x8e);
    fn(15, (uint32_t)isr15, 0x08, 0x8e);
    fn(16, (uint32_t)isr16, 0x08, 0x8e);
    fn(17, (uint32_t)isr17, 0x08, 0x8e);
    fn(18, (uint32_t)isr18, 0x08, 0x8e);
    fn(19, (uint32_t)isr19, 0x08, 0x8e);
    fn(20, (uint32_t)isr20, 0x08, 0x8e);
    fn(21, (uint32_t)isr21, 0x08, 0x8e);
    fn(22, (uint32_t)isr22, 0x08, 0x8e);
    fn(23, (uint32_t)isr23, 0x08, 0x8e);
    fn(24, (uint32_t)isr24, 0x08, 0x8e);
    fn(25, (uint32_t)isr25, 0x08, 0x8e);
    fn(26, (uint32_t)isr26, 0x08, 0x8e);
    fn(27, (uint32_t)isr27, 0x08, 0x8e);
    fn(28, (uint32_t)isr28, 0x08, 0x8e);
    fn(29, (uint32_t)isr29, 0x08, 0x8e);
    fn(30, (uint32_t)isr30, 0x08, 0x8e);
    fn(31, (uint32_t)isr31, 0x08, 0x8e);

    // 实现系统调用
    fn(255, (uint32_t)isr255, 0x08, 0x8e);

    __asm__ __volatile__("lidtl (idt_ptr)");
}

void isr_handler(Register *reg) {
    if (interrupt_handlers[reg->int_no])
        interrupt_handlers[reg->int_no](reg);
    else
        std::cout << "Unhandled interrupt " << reg->int_no << std::endl;
}


#define ISR_NOERRORCODE_MACRO(id) void isr##id() { \
    __asm__ __volatile__("pushl $0x0"); \
    __asm__ __volatile__("pushl $" #id); \
    __asm__ __volatile__("jmp isr_common_stub"); \
}

#define ISR_ERRORCODE_MACRO(id) void isr##id() { \
    __asm__ __volatile__("pushl $" #id); \
    __asm__ __volatile__("jmp isr_common_stub"); \
}

ISR_NOERRORCODE_MACRO(0)
ISR_NOERRORCODE_MACRO(1)
ISR_NOERRORCODE_MACRO(2)
ISR_NOERRORCODE_MACRO(3)
ISR_NOERRORCODE_MACRO(4)
ISR_NOERRORCODE_MACRO(5)
ISR_NOERRORCODE_MACRO(6)
ISR_NOERRORCODE_MACRO(7)
ISR_ERRORCODE_MACRO(8)
ISR_NOERRORCODE_MACRO(9)
ISR_ERRORCODE_MACRO(10)
ISR_ERRORCODE_MACRO(11)
ISR_ERRORCODE_MACRO(12)
ISR_ERRORCODE_MACRO(13)
ISR_ERRORCODE_MACRO(14)
ISR_NOERRORCODE_MACRO(15)
ISR_NOERRORCODE_MACRO(16)
ISR_ERRORCODE_MACRO(17)
ISR_NOERRORCODE_MACRO(18)
ISR_NOERRORCODE_MACRO(19)

// Intel 保留
ISR_NOERRORCODE_MACRO(20)
ISR_NOERRORCODE_MACRO(21)
ISR_NOERRORCODE_MACRO(22)
ISR_NOERRORCODE_MACRO(23)
ISR_NOERRORCODE_MACRO(24)
ISR_NOERRORCODE_MACRO(25)
ISR_NOERRORCODE_MACRO(26)
ISR_NOERRORCODE_MACRO(27)
ISR_NOERRORCODE_MACRO(28)
ISR_NOERRORCODE_MACRO(29)
ISR_NOERRORCODE_MACRO(30)
ISR_NOERRORCODE_MACRO(31)


// 用户自定义
ISR_NOERRORCODE_MACRO(255)

#undef ISR_NOERRORCODE_MACRO
#undef ISR_ERRORCODE_MACRO

void isr_common_stub() {
    __asm__ __volatile__("pusha");
    __asm__ __volatile__("movw %ds, %ax");
    __asm__ __volatile__("pushl %eax");
    __asm__ __volatile__("movw $0x10, %ax;");
    __asm__ __volatile__("movw %ax, %ds");
    __asm__ __volatile__("movw %ax, %es");
    __asm__ __volatile__("movw %ax, %fs");
    __asm__ __volatile__("movw %ax, %gs");
    __asm__ __volatile__("movw %ax, %ss");
    __asm__ __volatile__("pushl %esp");
    __asm__ __volatile__("call isr_handler");
    __asm__ __volatile__("addl $0x4, %esp");
    __asm__ __volatile__("popl %ebx");
    __asm__ __volatile__("movw %bx, %ds");
    __asm__ __volatile__("movw %bx, %es");
    __asm__ __volatile__("movw %bx, %fs");
    __asm__ __volatile__("movw %bx, %gs");
    __asm__ __volatile__("movw %bx, %ss");
    __asm__ __volatile__("popa");
    __asm__ __volatile__("addl $0x8, %esp");
    __asm__ __volatile__("iret");
}
