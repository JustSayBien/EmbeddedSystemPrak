/**
 * $Id: uart.c 478 2011-04-19 08:06:54Z bradatch $
 */

/******************************************************************************

File: uart.c

Project: Roomba Embedded Systems Training

Description: UART output

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS Training

*/


/****************************************************************** Includes */

#include <board.h>

#ifdef HAVE_UART

#include <stddef.h>
#include <uart.h>
#include <tools.h>


/******************************************************************* Defines */


/******************************************************* Function prototypes */


/************************************************************** Global const */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */

void uart_init(void) {
  // nothing to do
}


int32_t uart_putchar(int32_t c) {
  char vc = (char) (c & 0xff);
  while ( !(invert_byte_order32(UART_ST) & UART_ST_TRDY) );
  UART_TX = vc;
  return (int32_t) vc;
}


int32_t uart_puts(char *p) {
  uint32_t ctr = 0;
  if (p == NULL) {
    return -1;
  }
  do {
    // need to wait until UART gets ready to transmit
    if (invert_byte_order32(UART_ST) & UART_ST_TRDY) {
      UART_TX = *p;
      ++p;
      ++ctr;
    }
  } while (*p);
  
  return ctr;
}


void uart_write_byte(char b){
  while ( !(invert_byte_order32(UART_ST) & UART_ST_TRDY) );
      UART_TX = b;
}


int32_t uart_read_byte(void){

   char b = -1;

  while ( !(invert_byte_order32(UART_ST) & UART_ST_RRDY) );
      b = UART_RX;


   return (int32_t)b;

}


/*********************************************************** Local functions */

#endif /* HAVE_UART */
