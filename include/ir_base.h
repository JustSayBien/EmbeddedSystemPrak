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

uint8_t check_base_id ();

uint8_t check_discrete_base_id ();



#endif  // _IR_BASE_H
