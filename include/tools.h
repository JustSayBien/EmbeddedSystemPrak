/**
 * $Id: tools.h 672 2013-04-12 10:30:44Z klugeflo $
 */

/******************************************************************************

File: tools.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2012 (FAK) Created from RTOS Training

*/

#ifndef _TOOLS_H
#define _TOOLS_H 1


/****************************************************************** Includes */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************* Defines */


/****************************************************************** Typedefs */

  typedef enum {false, true} bool_t;

/************************************************************** Global const */


/********************************************************** Global variables */


/************************************************ Global function prototypes */

/*! sleep for approx. 10 us (our boards are running at 40 MHz) */
void my_sleep(uint32_t counter);


/*! invert the byte order of a 32 bit integer */
uint32_t invert_byte_order32(uint32_t in32);


/*! invert the byte order of a 16 bit integer */
uint16_t invert_byte_order16(uint16_t in16);


/*************************************************** Global inline functions */


/******************************************************************** Macros */

#define my_usleep(us)   my_sleep((us)/10) /*!< sleep for #us microseconds */
#define my_msleep(ms)   my_sleep((ms)*100) /*!< sleep for #ms milliseconds */



#ifdef __cplusplus
}
#endif

#endif /* !_TOOLS_H */
