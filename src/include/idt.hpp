#pragma once

#include <stdint.h>
#include <stddef.h>

/*
 * Following is about IDT
 */

struct InterruptDescriptor {
    uint16_t offset0_15;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t offset16_31;
} __attribute__((packed));

struct IDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct Register {
    uint32_t ds;  // data segment
    uint32_t edi; //
    uint32_t esi;
    uint32_t ebp;
    uint32_t useless_value; // esp originally
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax; // 由 pusha 压入
    uint32_t int_no; // 中断号
    uint32_t errno; // 错误号
    uint32_t eip; // 以下由CPU自动压入
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
};

#ifdef __cplusplus
extern "C"
#endif
void isr_handler(Register *);

void register_interrupt_handler(uint8_t id, decltype(isr_handler) *);

#ifdef __cplusplus
extern "C"
#endif
void init_idt();

// ISR中断服务程序
// 0~19属于CPU异常中断
void isr0() __asm__("_isr0");  // #DE 除0异常
void isr1() __asm__("_isr1");  // #DB 调试异常
void isr2() __asm__("_isr2");  // NMI
void isr3() __asm__("_isr3");  // BP断点异常
void isr4() __asm__("_isr4");  // #OF 溢出
void isr5() __asm__("_isr5");  // #BR 数组越界
void isr6() __asm__("_isr6");  // #UD 无效或未定义的操作码
void isr7() __asm__("_isr7");  // #NM 设备不可用/无数学协处理器
void isr8() __asm__("_isr8");  // #DF 双重故障, 设置errno
void isr9() __asm__("_isr9");  // 协处理器跨段操作
void isr10() __asm__("_isr10"); // #TS 无效TSS, 设置errno
void isr11() __asm__("_isr11"); // #NP 段不存在, 设置errno
void isr12() __asm__("_isr12"); // #SS 栈错误, 设置errno
void isr13() __asm__("_isr13"); // #GP 常规保护, 设置errno
void isr14() __asm__("_isr14"); // #PF 页故障, 设置errno
void isr15() __asm__("_isr15"); // CPU保留
void isr16() __asm__("_isr16"); // #MF 浮点处理单元错误
void isr17() __asm__("_isr17"); // #AC 对齐检查, 设置errno
void isr18() __asm__("_isr18"); // #MC 机器检查
void isr19() __asm__("_isr19"); // #XM SIMD单指令多数据

// 20~31 Intel保留
void isr20() __asm__("_isr20");
void isr21() __asm__("_isr21");
void isr22() __asm__("_isr22");
void isr23() __asm__("_isr23");
void isr24() __asm__("_isr24");
void isr25() __asm__("_isr25");
void isr26() __asm__("_isr26");
void isr27() __asm__("_isr27");
void isr28() __asm__("_isr28");
void isr29() __asm__("_isr29");
void isr30() __asm__("_isr30");
void isr31() __asm__("_isr31");

// 32~255用户自定义异常
void isr255() __asm__("_isr255");

#ifdef __cplusplus
extern "C"
#endif
void isr_common_stub() __asm__("_isr_common_stub");



/*
 * Following is about IRQ.
 */

#ifdef __cplusplus
extern "C"
#endif
void irq_handler(Register *);

enum {
    IRQ0 = 32,  // 计算机系统计时器
    IRQ1 = 33,  // 键盘
    IRQ2 = 34,  // 与 IRQ9 相接，MPU-401使用
    IRQ3 = 35,  // 串口设备
    IRQ4 = 36,  // 串口设备
    IRQ5 = 37,  // 建议声卡使用
    IRQ6 = 38,  // 软驱传输控制使用
    IRQ7 = 39,  // 打印机传输控制使用
    IRQ8 = 40,  // 即时时钟
    IRQ9 = 41,  // 与 IRQ2 相接，可设定给其他硬件
    IRQ10 = 42, // 建议网卡使用
    IRQ11 = 43, // 建议AGP显卡使用
    IRQ12 = 44, // 接PS/2鼠标，也可设定给其他硬件
    IRQ13 = 45, // 协处理器使用
    IRQ14 = 46, // IDE0传输控制使用
    IRQ15 = 47  // IDE1传输控制使用
};

void irq0() __asm__("_irq0");
void irq1() __asm__("_irq1");
void irq2() __asm__("_irq2");
void irq3() __asm__("_irq3");
void irq4() __asm__("_irq4");
void irq5() __asm__("_irq5");
void irq6() __asm__("_irq6");
void irq7() __asm__("_irq7");
void irq8() __asm__("_irq8");
void irq9() __asm__("_irq9");
void irq10() __asm__("_irq10");
void irq11() __asm__("_irq11");
void irq12() __asm__("_irq12");
void irq13() __asm__("_irq13");
void irq14() __asm__("_irq14");
void irq15() __asm__("_irq15");


#ifdef __cplusplus
extern "C"
#endif
void irq_common_stub() __asm__("_irq_common_stub");

