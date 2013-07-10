/**
 * $Id: output.h 672 2013-04-12 10:30:44Z klugeflo $
 */

/******************************************************************************

File: output.h

Project: Roomba Embedded Systems Training

Description: Printf Makros

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 28.09.2009

Last changed: 28.09.2009

*******************************************************************************

Modification history:
---------------------


*/

#ifndef OUTPUT_H_
#define OUTPUT_H_ 1


/****************************************************************** Includes */

#include <gprintf.h>
#include <uart.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************* Defines */


/****************************************************************** Typedefs */


/************************************************************** Global const */


/********************************************************** Global variables */


/************************************************ Global function prototypes */


/*************************************************** Global inline functions */


/******************************************************************** Macros */

//#define lcd_printf(fmt, args...)  gprintf(lcd_putchar, fmt, args)
//#define tty_printf(fmt, args...)  gprintf(tty_putchar, fmt, args)
//#define uart_printf(fmt, args...) gprintf(uart_putchar, fmt, args)

#define lcd_printf(args...)  gprintf(lcd_putchar, args)
#define tty_printf(args...)  gprintf(tty_putchar, args)
#define uart_printf(args...) gprintf(uart_putchar, args)

#define log_printf uart_printf
#define log_puts uart_puts
#define log_putchar uart_putchar


#ifdef __cplusplus
}
#endif

#endif /* OUTPUT_H_ */
