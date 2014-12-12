#include "../include/timer.hpp"
#include "../include/ostream.hpp"
#include "../include/asm.hpp"
#include "../include/sched.hpp"

void init_timer(uint32_t freq) {
    register_interrupt_handler(IDT::IRQ0, [](IDT::Register *reg) {
        //static uint32_t cnt = 0;
        (void)reg; // Ignore warnning.
        //io::cout << io::Color::CYAN << "Tick: " << ++cnt << io::endl;

        schedule();
    });

    uint32_t divisor = 1193180 / freq;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0x000000ff);
    outb(0x40, (divisor & 0x0000ff00) >> 8);
}
