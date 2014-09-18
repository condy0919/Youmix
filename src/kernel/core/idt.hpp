#pragma once

#include <stdint.h>
#include <stddef.h>

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
    uint32_t useresp;
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
void isr0();  // #DE 除0异常
void isr1();  // #DB 调试异常
void isr2();  // NMI
void isr3();  // BP断点异常
void isr4();  // #OF 溢出
void isr5();  // #BR 数组越界
void isr6();  // #UD 无效或未定义的操作码
void isr7();  // #NM 设备不可用/无数学协处理器
void isr8();  // #DF 双重故障, 设置errno
void isr9();  // 协处理器跨段操作
void isr10(); // #TS 无效TSS, 设置errno
void isr11(); // #NP 段不存在, 设置errno
void isr12(); // #SS 栈错误, 设置errno
void isr13(); // #GP 常规保护, 设置errno
void isr14(); // #PF 页故障, 设置errno
void isr15(); // CPU保留
void isr16(); // #MF 浮点处理单元错误
void isr17(); // #AC 对齐检查, 设置errno
void isr18(); // #MC 机器检查
void isr19(); // #XM SIMD单指令多数据

// 20~31 Intel保留
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

// 32~255用户自定义异常
void isr255();

#ifdef __cplusplus
extern "C"
#endif
void isr_common_stub();

