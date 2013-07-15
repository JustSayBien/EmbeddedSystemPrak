/******************************************************************************

File: ir_base.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel
        Universität Augsburg

Created: 28.06.2013


******************************************************************************/

/************************************************************* Include Guard */

#ifndef _IR_BASE_H
#define _IR_BASE_H

/****************************************************************** Includes */

#include "workbench.h"
#include "tools.h"
#include <stdint.h>


/******************************************************************* Defines */


/******************************************************* Function prototypes */

/**
 * \brief	Check the currently visible base id
 *
 *         	this function checks if any base id is currently visible
 * \return 	the id of the visible base
 */
uint8_t check_base_id ();

/**
 * \brief	Check the currently visible base id
 *
 *         	this function checks if any base id is currently visible and has been visible for some time
 * \return 	the id of the base which is definitively in the direct vicinity
 */
uint8_t check_discrete_base_id ();



#endif  // _IR_BASE_H
