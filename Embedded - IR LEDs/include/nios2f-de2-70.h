/**
 * $Id: nios2f-de2-70.h 678 2013-04-12 15:19:59Z klugeflo $
 * Addresses for NIOS2f on DE2-70 board
 */

#ifndef BOARD_INCLUDE
#error Do not directly include this file, use <board.h> instead!
#endif

#ifndef NIOS2_NIOS2F_DE2_70_H
#define NIOS2_NIOS2F_DE2_70_H

// RAMS
#define A_SDRAM       0x04000000
#define A_SSRAM       0x08200000
#define A_OCRAM       0x08408000

// uC Peripherals
#define A_JTAG_DBG    0x08410800
#define A_DM9000      0x08411000
#define A_PCC         0x08411400
#define A_S7CTL       0x08411480
#define A_RGB         0x08411500
#define A_TIMER       0x08411540
#define A_UART        0x08411560
#define A_PIO_LGREEN  0x08411580
#define A_PIO_LRED    0x08411590
#define A_PIO_BUTTON  0x084115a0
#define A_PIO_SW      0x084115b0
#define A_PS2         0x084115c0
#define A_LCD         0x084115d0
#define A_LCD_BL      0x084115f0
#define A_JTAG_UART   0x08411600
#define A_SYSID       0x08411608


#define HAVE_S7CTL       1
#define HAVE_PIO_LGREEN  1
#define HAVE_PIO_LRED    1
#define HAVE_PIO_LEDS    1 // summing up the above
#define HAVE_PIO_BUTTON  1
#define N_BUTTONS        4
#define HAVE_PIO_SW      1
#define N_SW            18
#define HAVE_UART        1
#define HAVE_JTAG_UART   1
#define HAVE_PCC         1
#define HAVE_SYSID       1

//#define A_RGB_MUX_CLK (A_VGA+0x40) /*!< VGA MUX clock address */


#define N_IRQS     20 /*!< Number of IRQs available */

#define IRQ_JTAG_UART_N  0
#define IRQ_TIMER_N      1
#define IRQ_PIO_BUTTON_N 2
#define IRQ_PIO_SW_N     3
#define IRQ_UART_N       4
#define IRQ_PS2_N        5
#define IRQ_JDM9000      6

#endif /* NIOS2_NIOS2F_DE2_70_H */

