/**
 * $Id: irq.c 468 2011-02-21 15:53:05Z klugeflo $
 */

/******************************************************************************

File: irq.c

Project: Roomba Embedded Systems Training

Description: IRQ handling

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2012 (FAK) Created from RTOS Training

*/

/****************************************************************** Includes */

#include <irq.h>

#include <or32intrinsics.h>
#include <spr-defs.h>
#include <board.h>

#include <stddef.h>
#include <errno.h>

/******************************************************************* Defines */


/******************************************************* Function prototypes */

int32_t pic_irq(void);
bool_t irq_enabled(uint32_t irq);


/************************************************************** Global const */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */

static irq_handler_t irq_list[N_IRQS];


/******************************************************************** Macros */


static inline bool_t is_irq_enabled(uint32_t irq) {
  return (__mfspr(SPR_PICMR) & (1<<irq)) != 0;
}

/********************************************************** Global functions */


void init_irq(void) {
  uint32_t i;
  for (i=0; i<N_IRQS; ++i) {
    irq_list[i] = NULL;
  }
}


void enable_all_interrupts(void) {
  __mtspr(SPR_SR, __mfspr(SPR_SR) | SPR_SR_IEE);
  __mtspr(SPR_PICMR, 0xffffffff);
}


void disable_all_interrupts(void) {
  __mtspr(SPR_SR, __mfspr(SPR_SR) & ~SPR_SR_IEE);
  __mtspr(SPR_PICMR, 0x0);
}


uint32_t irq_enable(uint32_t irq) {
  uint32_t err = 0;
  if (irq >= N_IRQS)
    err = ENXIO;
  else if (irq_list[irq] == NULL) {
    err = ENOENT;
  }
  else {
    __mtspr(SPR_PICMR, __mfspr(SPR_PICMR) | (1<<irq));
    err = 0;
  }
  return err;
}


uint32_t irq_disable(uint32_t irq) {
  uint32_t err = 0;
  if (irq >= N_IRQS)
    err = ENXIO;
  else {
    __mtspr(SPR_PICMR, __mfspr(SPR_PICMR) & ~(1<<irq));
    err = 0;
  }
  return err;
}


uint32_t irq_request(uint32_t irq, irq_handler_t handler) {
  uint32_t err = 0;
  if (irq >= N_IRQS)
    err = ENXIO;
  else if (handler == NULL)
    err = EINVAL;
  else if (irq_list[irq] != NULL)
    err = EEXIST;
  else {
    irq_list[irq] = handler;
    err = 0;
  }
  return err;
}


uint32_t irq_free(uint32_t irq) {
  uint32_t err = 0;
  if (irq >= N_IRQS)
    err = ENXIO;
  else if ((irq>1) && is_irq_enabled(irq))
    err = EBUSY;
  else {
    irq_list[irq] = NULL;
    err = 0;
  }
  return err;
}


bool_t irq_enabled(uint32_t irq) {
  return is_irq_enabled(irq);
}




/******************************************************* Handler for XCP vec */

void do_irq(void) {
  int32_t irq;
  while ((irq = pic_irq()) >= 0) {
    if (irq_list[irq] != NULL) {
      irq_list[irq](irq);
    }
    else {

    }
    __mtspr(SPR_PICSR, __mfspr(SPR_PICSR) & ~(1 << irq));
  }
}


/*********************************************************** Local functions */


//! plucked from linux-2.6.24
int32_t pic_irq(void) {
  uint32_t irq;
  uint32_t mask;
  
  uint32_t pend = __mfspr(SPR_PICSR) & 0xfffffffc;
  
  if (pend & 0x0000ffff) {
    if (pend & 0x000000ff) {
      if (pend & 0x0000000f) {
	mask = 0x00000001;
	irq = 0;
      } else {
	mask = 0x00000010;
	irq = 4;
      }
    } else {
      if (pend & 0x00000f00) {
	mask = 0x00000100;
	irq = 8;
      } else {
	mask = 0x00001000;
	irq = 12;
      }
    }
  } else if(pend & 0xffff0000) {
    if (pend & 0x00ff0000) {
      if (pend & 0x000f0000) {
	mask = 0x00010000;
	irq = 16;
      } else {
	mask = 0x00100000;
	irq = 20;
      }
    } else {
      if (pend & 0x0f000000) {
	mask = 0x01000000;
	irq = 24;
      } else {
	mask = 0x10000000;
	irq = 28;
      }
    }
  } else {
    return -1;
  }
  
  while (! (mask & pend)) {
    mask <<=1;
    irq++;
  }
  return irq;
}

