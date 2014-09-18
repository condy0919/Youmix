#pragma once

#include <stdint.h>
#include <stddef.h>

#include "idt.hpp"

void timer_callback(Register *);

void init_timer(uint32_t);
