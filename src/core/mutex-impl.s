.section .text

.globl kthread_mutex_trylock
.type kthread_mutex_trylock, @function
kthread_mutex_trylock:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %edx
    mov $-1, %eax
    xchg (%edx), %eax
    not %eax
    leave
    ret
.size kthread_mutex_trylock, . - kthread_mutex_trylock

.globl kthread_mutex_lock
.type kthread_mutex_lock, @function
kthread_mutex_lock:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %edx
lock_retry:
    mov $-1, %eax
    xchg (%edx), %eax
    test %eax, %eax
    jnz lock_failed
    leave
    ret
lock_failed:
    int $0x81 # Yield
    jmp lock_retry
.size kthread_mutex_lock, . - kthread_mutex_lock

.globl kthread_mutex_unlock
.type kthread_mutex_unlock, @function
kthread_mutex_unlock:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %edx
    movl $0, (%edx)
    leave
    ret
.size kthread_mutex_unlock, . - kthread_mutex_unlock
