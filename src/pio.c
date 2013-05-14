/**
 * $Id: pio.c 478 2011-04-19 08:06:54Z bradatch $
 */

/******************************************************************************

File: pio.c

Project: Roomba Embedded Systems Training

Description: OR32 simulator special functions

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universit�t Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------

21.02.2011 (FAK) Created from RTOS Training
26.04.2011 (MG) Modified for embedded practical training
*/


/****************************************************************** Includes */

#include <board.h>

#ifdef HAVE_BUTTONS

#include <pio.h>


/******************************************************************* Defines */


/******************************************************* Function prototypes */


/************************************************************** Global const */

/*! Bits for buttons */
#if __BOARD==de2-70
const uint8_t button_vals[] = {
  0x01, 0x02, 0x04, 0x08
};
#elif __BOARD==db_4ce10

#endif /* __BOARD==? */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */


/*********************************************************** Local functions */


#endif /* HAVE_BUTTONS */
