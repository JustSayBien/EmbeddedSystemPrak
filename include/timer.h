/**
 * $Id: timer.h 468 2011-02-21 15:53:05Z klugeflo $
 */

/******************************************************************************

File: timer.h

Project: Roomba Embedded Systems Training

Description: Handling of tick timer

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS Training

*/

#ifndef _TIMER_H
#define _TIMER_H 1


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

/*!
  Register callback function for timer exceptions
  \param cb_fn callback
  \return - 0 if no error
          - EINVAL if cb_fn is invalid, i.e. NULL
*/
  uint32_t register_timer_cb(void (*cb_fn)(void));

  /*!
    Enable timer exception
    \return - 0 if no error
            - ENOENT if no handler was registered
  */
  uint32_t enable_timer(void);

  /*!
    Disable timer exception
  */
  void disable_timer(void);


/*************************************************** Global inline functions */


/******************************************************************** Macros */



#ifdef __cplusplus
}
#endif

#endif /* !_TIMER_H */
