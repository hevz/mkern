/* uart.c
 * Heiher <r@hev.cc>
 */

#include "uart.h"
#include "spinlock.h"

void
prom_putchar (char c)
{
    int timeout;
    unsigned char *uart_base;

    uart_base = (unsigned char *)0x800000001fe001e0ul;
    timeout = 1024;

    while (((serial_in (uart_base, UART_LSR) & UART_LSR_THRE) == 0) &&
           (timeout-- > 0))
        ;

    serial_out (uart_base, UART_TX, c);
}

int
puts (const char *s)
{
    static int lock;

    spin_lock (&lock);
    while ('\0' != *s)
        putchar (*s++);
    spin_unlock (&lock);

    return 0;
}
