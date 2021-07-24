/* string.h
 * Heiher <r@hev.cc>
 */

#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

static inline void
spin_lock (volatile int *lock)
{
    int tmp;

    asm volatile("1:                      \n\t"
                 "ll.w %[tmp], %[lock], 0 \n\t"
                 "bnez %[tmp], 1b         \n\t"
                 "ori  %[tmp], $zero, 1   \n\t"
                 "sc.w %[tmp], %[lock], 0 \n\t"
                 "beqz %[tmp], 1b         \n\t"
                 : [tmp] "=&r"(tmp)
                 : [lock] "r"(lock)
                 : "memory");
}

static inline void
spin_unlock (volatile int *lock)
{
    asm volatile("dbar 0" ::: "memory");
    *lock = 0;
    asm volatile("dbar 0" ::: "memory");
}

#endif /* __SPINLOCK_H__ */
