/**
 * $Id: uart.h 478 2011-04-19 08:06:54Z bradatch $
 */

/******************************************************************************

File: uart.h

Project: Roomba Embedded Systems Training

Description: ALTERA DE2-70 on-board UART

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS Training


*/

#ifndef DRIVER_UART_H
#define DRIVER_UART_H 1


/****************************************************************** Includes */

#include <board.h>

#ifdef HAVE_UART

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************* Defines */

#define UART_ST_PE   0x00000001 /*!< Parity Error */
#define UART_ST_FE   0x00000002 /*!< Framing Error */
#define UART_ST_BRK  0x00000004 /*!< Break Detect */
#define UART_ST_ROE  0x00000008 /*!< Receive Overrun Error */
#define UART_ST_TOE  0x00000010 /*!< Transmit Overrun Error */
#define UART_ST_TMT  0x00000020 /*!< Transmit Empty */
#define UART_ST_TRDY 0x00000040 /*!< Transmit Ready */
#define UART_ST_RRDY 0x00000080 /*!< Receive Character Ready */
#define UART_ST_E    0x00000100 /*!< Exception */
#define UART_ST_DCTS 0x00000400 /*!< Change in Clear To Send (CTS) Signal (optional) */
#define UART_ST_CTS  0x00000800 /*!< Clear-To-Send (CTS) Signal (optional) */
#define UART_ST_EOP  0x00001000 /*!< End Of Packet encountered (optgional) */


#define UART_CT_IPE   0x00000001 /*!< Enable Interrupt for a Parity Error */
#define UART_CT_IFE   0x00000002 /*!< Enable Interrupt for a Framing Error */
#define UART_CT_IBRK  0x00000004 /*!< Enable Interrupt for a Break Detect */
#define UART_CT_IROE  0x00000008 /*!< Enable Interrupt for a Receiver Overrun Error */
#define UART_CT_ITOE  0x00000010 /*!< Enable Interrupt for a Transmitter Overrung Error */
#define UART_CT_ITMT  0x00000020 /*!< Enable Interrupt for a Transmitter Shift Register Empty */
#define UART_CT_ITRDY 0x00000040 /*!< Enable Interrupt for a Transmission Ready */
#define UART_CT_IRRDY 0x00000080 /*!< Enable Interrupt for a Read Ready */
#define UART_CT_IE    0x00000100 /*!< Enable Interrupt for an Exception */
#define UART_CT_TBRK  0x00000200 /*!< Transmit Break */
#define UART_CT_IDCTS 0x00000400 /*!< Enable Interrupt for a change in CTS signal */
#define UART_CT_RTS   0x00000800 /*!< Request To Send (RTS) Signal (optional) */
#define UART_CT_IEOP  0x00001000 /*!< Enable Interrupt for End-Of-Packet condition */


/****************************************************************** Typedefs */


/************************************************************** Global const */


/********************************************************** Global variables */


/************************************************ Global function prototypes */

/*! Initialise the UART device */
  void uart_init(void);

  /*! Write one char to the UART device */
  int32_t uart_putchar(int32_t c);

  /*! Output a string over UART */
  int32_t uart_puts(char *p);

  void uart_write_byte(char);
  int32_t uart_read_byte(void);

/*************************************************** Global inline functions */



/******************************************************************** Macros */



#ifdef __cplusplus
}
#endif

#endif /* HAVE_UART */

#endif /* UART_H */
