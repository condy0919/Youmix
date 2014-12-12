#ifndef _YOUMIX_SCHED_HPP_
#define _YOUMIX_SCHED_HPP_

#include "thread.hpp"
#include "task_struct.hpp"
#include "../libs/list"

extern "C" void init_sched();
void schedule() __attribute__((noinline));

#endif
