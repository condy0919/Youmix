#ifndef _YOUMIX_TASK_STRUCT_HPP_
#define _YOUMIX_TASK_STRUCT_HPP_

#include "context.hpp"
#include "mm_struct.hpp"

enum class TaskState { UNINITIALIZED, WAITING, READY, RUNNING };

struct task_struct {
    task_struct()
        : state(TaskState::RUNNING), stack(0), mm(0), cntx({0, 0, 0, 0, 0, 0}),
          id(++__global_id) {}

    volatile TaskState state;
    char *stack;
    mm_struct *mm;
    TaskContext cntx;
    int id;

    static int __global_id;
};

#endif
