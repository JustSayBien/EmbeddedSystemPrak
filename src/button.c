/**
 * $Id: button.c 676 2013-04-12 13:57:21Z klugeflo $
 * PIO Buttons (Keys)
 */


/**
 * $Id: uart.c 672 2013-04-12 10:30:44Z klugeflo $
 */

/******************************************************************************

File: button.c

Project: Roomba Embedded Systems Training

Description: PIO Buttons functions

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 12.04.2013

*******************************************************************************

Modification history:
---------------------
12.04.2013 (FAK) Created from RTOS Training
06.05.2013 (MG) Changed for Embedded Systems Training

*/


/****************************************************************** Includes */

#include <button.h>
#include <pio.h>
#include <asm/io.h>

/******************************************************************* Defines */

#ifdef HAVE_PIO_BUTTON

/******************************************************* Function prototypes */


/************************************************************** Global const */

/*! Bits for buttons */
#if __BOARD==de2-70
const uint8_t button_vals[] = {
	0x01, 0x02, 0x04, 0x08
};
#elif __BOARD==db_4ce10
const uint8_t button_vals[] = {
	0x01, 0x02
};
#endif /* __BOARD==? */

/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */


/*********************************************************** Local functions */


#define BUTTON_PRESSED(button) ( ~IORD8(A_PIO_BUTTON, PIO_DATA) & button_vals[button] )

int button_pressed(uint8_t button) {
  if (button >= N_BUTTONS) return -1;
  return BUTTON_PRESSED(button);
}

int button_wait(uint8_t button) {
  if (button >= N_BUTTONS) return -1;
  while (!BUTTON_PRESSED(button));
  while (BUTTON_PRESSED(button));
  return 0;
}

/******************************************************************** Macros */


/********************************************************** Global functions */




#endif /* HAVE_PIO_BUTTON */
