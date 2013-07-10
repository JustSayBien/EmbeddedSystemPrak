/**
 * $Id: ju.h 676 2013-04-12 13:57:21Z klugeflo $
 * JTAG UART
 */

#ifndef _JU_H
#define _JU_H 1

#include <board.h>

#ifdef HAVE_JTAG_UART

#define JU_DATA 0x00
#define JU_CTRL 0x04

#define JU_DATA_RAVAIL 0xffff0000
#define JU_DATA_RVALID 0x00008000
#define JU_DATA_DATA   0x000000ff

#define JU_CTRL_RE     0x00000001
#define JU_CTRL_WE     0x00000002
#define JU_CTRL_RI     0x00000010
#define JU_CTRL_WI     0x00000020
#define JU_CTRL_AC     0x00000040
#define JU_CTRL_WSPACE 0xffff0000


void ju_puts(char *s);
void ju_putchar(int c);

#endif /* HAVE_JTAG_UART */
#endif /* _JU_H */
