/**
 * $Id: led.h 478 2011-04-19 08:06:54Z bradatch $
 */

/******************************************************************************

File: led.h

Project: Roomba Embedded Systems Training

Description: Programming tools for LEDs

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universit�t Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------

21.02.2011 (FAK) Created from RTOS Training
26.04.2011 (MG) Modified for embedded practical training
*/

#ifndef _LED_H
#define _LED_H 1

/****************************************************************** Includes */

#include <stdint.h>

#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************* Defines */


/****************************************************************** Typedefs */


/************************************************************** Global const */

#if __BOARD==de2-70
extern const uint32_t led_vals[];

#define A_LED   0xf00000c0 /*!< Green LED address */
#define LED (*((uint32_t volatile *) A_LED))
#elif __BOARD==db_4ce10

#endif /* __BOARD==? */



/********************************************************** Global variables */


/************************************************ Global function prototypes */


/*************************************************** Global inline functions */


/******************************************************************** Macros */


#ifdef __cplusplus
}
#endif

#endif /* !_LED_H */
