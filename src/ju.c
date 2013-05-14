/**
 * $Id: ju.c 676 2013-04-12 13:57:21Z klugeflo $
 * JTAG UART
 */

#include <board.h>
#include <ju.h>
#include <asm/io.h>

#ifdef HAVE_JTAG_UART

void ju_puts(char *s) {
  while (*s != 0) {
    IOWR8(A_JTAG_UART, JU_DATA, *s);
    s++;
  }
}

void ju_putchar(int c) {
  IOWR8(A_JTAG_UART, JU_DATA, c);
}

#endif /* HAVE_JTAG_UART */
