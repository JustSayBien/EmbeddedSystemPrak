/**
 * $Id: irq.h 468 2011-02-21 15:53:05Z klugeflo $
 */

/******************************************************************************

File: irq.h

Project: Roomba Embedded Systems Training

Description: IRQ handling

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS Training


*/

#ifndef _IRQ_H
#define _IRQ_H 1

/****************************************************************** Includes */

#include <stdint.h>
#include <tools.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************* Defines */


/****************************************************************** Typedefs */

  typedef uint32_t (*irq_handler_t)(uint32_t);

/************************************************************** Global const */


/********************************************************** Global variables */

  //extern bool_t irq_state;

/************************************************ Global function prototypes */
  
/*!
  Initialise the IRQ system
*/
  void init_irq(void);


  /*!
    Enable all interrupts
  */
  void enable_all_interrupts(void);


  /*!
    Disable all interrupts
  */
  void disable_all_interrupts(void);


  /*!
    Enable a specific IRQ
    \param irq number of irq
    \return - 0 if no error
            - ENOENT no handler registered
	    - ENXIO no such irq number

   */
  uint32_t irq_enable(uint32_t irq);


  /*!
    Disable a specific IRQ
    \param irq number of irq
    \return - 0 if no error
            - ENXIO no such irq number
  */
  uint32_t irq_disable(uint32_t irq);


  /*!
    Request an interrupt / register handler
    \param irq number of irq
    \param handler the callback handler function
    \return - 0 if no error
            - ENXIO no such irq number
	    - EINVAL invalid handler
	    - EEXIST a handler is already registered, unregister using #irq_free
	      before!
  */
  uint32_t irq_request(uint32_t irq, irq_handler_t handler);

  /*!
    Unregister an interrupt handler
    \param irq number of irq
    \return - 0 if no error
            - ENXIO no such irq number
	    - EBUSY irq is enabled, disable using #irq_disable before!
  */
  uint32_t irq_free(uint32_t irq);

  /*!
    Check whether an interrupt is enabled
    \param irq number of irq
    \return true, if irq is enabled, false else
  */
  bool_t irq_enabled(uint32_t irq);



/*************************************************** Global inline functions */


/******************************************************************** Macros */


#ifdef __cplusplus
}
#endif

#endif /* !_IRQ_H */
