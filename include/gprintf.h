/**
 * $Id: gprintf.h 468 2011-02-21 15:53:05Z klugeflo $
 */

/******************************************************************************

File: gprintf.h

Project: Roomba Embedded Systems Training

Description: Generic printf output (only u,d,x,c,s)

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS Training

*/

#ifndef _PRINTF_H
#define _PRINTF_H 1


/****************************************************************** Includes */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************* Defines */


/****************************************************************** Typedefs */


/************************************************************** Global const */


/********************************************************** Global variables */


/************************************************ Global function prototypes */

  int32_t gprintf(int32_t (*putchar_fn)(int32_t), const char *format, ...);
  int32_t gputs(int32_t (*putchar_fn)(int32_t), const char *p);


/*************************************************** Global inline functions */


/******************************************************************** Macros */


#ifdef __cplusplus
}
#endif

#endif /* !_PRINTF_H */
