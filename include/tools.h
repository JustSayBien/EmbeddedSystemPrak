/**
 * $Id: tools.h 468 2011-02-21 15:53:05Z klugeflo $
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
26.04.2011 (MG) Modified to be used in embedded practical training
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

/*! sleep for approx. 1 us (our boards are running at 40 MHz) */
void wait(uint32_t us);

void msleep(uint32_t ms);

/*! invert the byte order of a 32 bit integer */
uint32_t invert_byte_order32(uint32_t in32);


/*! invert the byte order of a 16 bit integer */
uint16_t invert_byte_order16(uint16_t in16);
/*************************************************** Global inline functions */


/******************************************************************** Macros */


#ifdef __cplusplus
}
#endif

#endif /* !_TOOLS_H */
