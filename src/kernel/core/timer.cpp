#include "timer.hpp"
#include "ostream.hpp"
#include "asm_port.hpp"

void timer_callback(Register *reg) {
    static uint32_t cnt = 0;
    std::cout << std::RED << "Tick: " << ++cnt << std::endl;
}

void init_timer(uint32_t freq) {
    register_interrupt_handler(IRQ0, timer_callback);

    uint32_t divisor = 1193180 / freq;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0x000000ff);
    outb(0x40, (divisor & 0x0000ff00) >> 8);
}
