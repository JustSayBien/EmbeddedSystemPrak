/**
 * $Id: nios2s-db_4ce10.h 678 2013-04-12 15:19:59Z klugeflo $
 * Addresses for NIOS2s on DB_4CD10 board
 */

#ifndef BOARD_INCLUDE
#error Do not directly include this file, use <board.h> instead!
#endif

#ifndef NIOS2_NIOS2F_DE2_70_H
#define NIOS2_NIOS2F_DE2_70_H

// RAMS
#define A_SDRAM       0x01000000
#define A_OCRAM       0x02000000

// uC Peripherals
#define A_JTAG_DBG    0x02009000
#define A_EPCS        0x02009800
#define A_PCC         0x0200a000
#define A_TIMER       0x0200a040
#define A_HR_TIMER    0x0200a060
#define A_UART        0x0200a080
#define A_PIO_LBLUE   0x0200a0a0
#define A_PIO_BUTTON  0x0200a0b0
#define A_JTAG_UART   0x0200a0e0
#define A_SYSID       0x0200a0e8
#define A_UART_INT    0x0200a100
#define A_SPI	      0x0200a200
#define A_SPI_INT     0x0200a220
#define A_SPI_WAKE    0x0200a230
#define A_I2C	      0x0200a240
#define A_IR_SENDER   0x0200a100

#define HAVE_PIO_LBLUE   1
#define HAVE_PIO_BUTTON  1
#define N_BUTTONS        4
#define HAVE_UART        1
#define HAVE_JTAG_UART   1
#define HAVE_PCC         1
#define HAVE_SYSID       1
#define HAVE_TIMER	 1
//#define HAVE_RADIO	 1
#define HAVE_SPI	 1
#define HAVE_IR		 1


#define N_IRQS     20 /*!< Number of IRQs available */

#define IRQ_EPCS_N       0
#define IRQ_HR_TIMER_N   3 // old 1
#define IRQ_TIMER_N      2
#define IRQ_JTAG_UART_N  1 // old 3
#define IRQ_PIO_BUTTON_N 4 // old 4
#define IRQ_UART_N       5 // old 6
#define IRQ_UART_INT_N	 6 // new
#define IRQ_SPI_N	 7 // new
#define IRQ_SPI_INT_N	 8 // new
#define IRQ_I2C_N	 9 // new

#endif /* NIOS2_NIOS2F_DE2_70_H */

