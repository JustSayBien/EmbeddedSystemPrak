/**
 * $Id: pio.h 478 2011-04-19 08:06:54Z bradatch $
 */

/******************************************************************************

File: pio.h

Project: Roomba Embedded Systems Training

Description: Programming tools for PIOs

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universit�t Augsburg

Created: 02.02.2011

*******************************************************************************

Modification history:
---------------------
02.02.2011 (FAK) Created from RTOS Training
26.04.2011 (MG) Modified for embedded practical training
*/

#ifndef _PIO_H
#define _PIO_H 1


/****************************************************************** Includes */

#include <board.h>

#ifdef HAVE_BUTTONS

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************* Defines */


/****************************************************************** Typedefs */


/************************************************************** Global const */

#if __BOARD==de2-70
/*! Bit maps for the DE2-70 buttons */
  extern const uint8_t button_vals[];
#elif __BOARD==db_4ce10
/*! Bit maps for the DB4CE10 buttons */

#endif


/********************************************************** Global variables */


/************************************************ Global function prototypes */


/*************************************************** Global inline functions */


/******************************************************************** Macros */

#define BUTTON_PRESSED(button) ( ~PIO_BT_DATA & button_vals[button] )

#define BUTTON_WAIT(button) {			\
    while (!BUTTON_PRESSED(button));		\
    while (BUTTON_PRESSED(button));			\
  }


#ifdef __cplusplus
}
#endif

#endif /* HAVE_BUTTONS */

#endif /* !_PIO_H */
