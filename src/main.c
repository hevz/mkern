/* main.c
 * Heiher <r@hev.cc>
 */

#include "uart.h"
#include "string.h"

static unsigned int mem;

void
cpu_main (unsigned int cpuid)
{
    if (cpuid == 0) {
        puts ("CPU 0\r\n");

        for (;;) {
            unsigned int t1, t2, ct = 0;

            asm volatile(
#ifdef USE_CMPXCHG
                "1:                             \n\t"
                "addi.w    %[ct], %[ct], 1      \n\t"
                "ld.w      %[t1], %[mem], 0     \n\t"
                "bstrins.w %[t1], $zero, 31, 16 \n\t"
                "ll.w      %[t2], %[mem], 0     \n\t"
                "bne       %[t1], %[t2], 1b     \n\t"
                "sc.w      %[t1], %[mem], 0     \n\t"
                "beqz      %[t1], 1b            \n\t"
#else
                "1:                             \n\t"
                "addi.w    %[ct], %[ct], 1      \n\t"
                "ll.w      %[t1], %[mem], 0     \n\t"
                "bstrins.w %[t1], $zero, 31, 16 \n\t"
                "sc.w      %[t1], %[mem], 0     \n\t"
                "beqz      %[t1], 1b            \n\t"
#endif
                : [t1] "=&r"(t1), [t2] "=&r"(t2), [ct] "+r"(ct)
                : [mem] "r"(&mem)
                : "memory");

            if (ct > 0) {
                char buf[32];

                ultostr (buf, ct);
                buf[16] = '\r';
                buf[17] = '\n';
                buf[18] = '\0';
                puts (buf);
            }
        }
    } else {
        unsigned short i = 0;
        volatile unsigned short *mp = (void *)&mem;

        puts ("CPU 1-3\r\n");

        for (;;) {
            *mp = i++;
            *mp = i++;
            *mp = i++;
            *mp = i++;

            *mp = i++;
            *mp = i++;
            *mp = i++;
            *mp = i++;
        }
    }
}
