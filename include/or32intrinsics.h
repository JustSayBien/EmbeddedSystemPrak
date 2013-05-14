/**
 * $Id: or32intrinsics.h 468 2011-02-21 15:53:05Z klugeflo $
 */

/******************************************************************************

File: or32intrinsics.h

Project: Roomba Embedded Systems Training

Description: Intrinsic macros

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS Training

*/

#ifndef _OR32INTRINSICS_H
#define _OR32INTRINSICS_H 1


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


/*************************************************** Global inline functions */


/******************************************************************** Macros */

#define _mfspr(regaddr) __mfspr(regaddr)

#define __mfspr(regaddr)						\
  ({ uint32_t val;							\
    asm volatile ("l.mfspr %0, %1, 0" : "=r"(val) : "r"(regaddr));	\
    val; })


#define _mtspr(regaddr, val) __mtspr(regaddr, val)

#define __mtspr(regaddr, val)						\
  { asm volatile ("l.mtspr %0, %1, 0" : : "r"(regaddr), "r"(val)); }


#ifdef __cplusplus
}
#endif

#endif /* !_OR32INTRINSICS_H */
