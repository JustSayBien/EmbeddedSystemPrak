/**
 * $Id: timer.c 672 2013-04-12 10:30:44Z klugeflo $
 */

#if 0

/******************************************************************************

File: timer.c

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


/****************************************************************** Includes */

#include <timer.h>

#include <stddef.h>
#include <errno.h>
#include <spr-defs.h>
#include <or32intrinsics.h>


/******************************************************************* Defines */


/******************************************************* Function prototypes */


/************************************************************** Global const */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */

static void (*my_callback)(void) = NULL;

/******************************************************************** Macros */


/********************************************************** Global functions */

uint32_t register_timer_cb(void (*cb_fn)(void)) {
  uint32_t err = 0;
  if (cb_fn == NULL) {
    err = EINVAL;
  }
  else {
    my_callback = cb_fn;
  }
  return err;
}


uint32_t enable_timer(void) {
  uint32_t err = 0;
  if (my_callback == NULL) {
    err = ENOENT;
  }
  else {
    __mtspr(SPR_SR, __mfspr(SPR_SR) | SPR_SR_TEE);
  }
  return err;
}


void disable_timer(void) {
  __mtspr(SPR_SR, __mfspr(SPR_SR) & ~SPR_SR_TEE);
}



/******************************************************* Handler for XCP vec */

void do_timer(void) {
  if (my_callback != NULL) {
    my_callback();
  }
}


/*********************************************************** Local functions */


#endif /* 0 */
