/**
 * $Id: led.c 478 2011-04-19 08:06:54Z bradatch $
 */

/******************************************************************************

File: ir.c

Project: Roomba Embedded Systems Training

Description: Driver for IR sendedr

Author: Florian Haas <florian.haas@student.uni-augsburg.de>
        Universitaet Augsburg

Created: 23.03.2012

*******************************************************************************

Modification history:
---------------------

23.03.2012 Created from led.c
10.06.2013 (MG) Modified for NIOSII
*/


/****************************************************************** Includes */

#include <ir.h>
#include <board.h>
#include <tools.h>
#include <asm/io.h>


/******************************************************************* Defines */


/******************************************************* Function prototypes */


/************************************************************** Global const */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */



/******************************************************************** Macros */


/********************************************************** Global functions */

#ifdef HAVE_IR

void ir_sender_set(unsigned char left, unsigned char center, unsigned char right, unsigned char near) {
	IOWR32(A_IR_SENDER, IR_SENDER_DATA,near << 24 | right << 16 | center << 8 | left);
}

void ir_sender_on() {
	IOWR32(A_IR_SENDER, IR_SENDER_CTRL, 0x0f);
}

void ir_sender_off() {
	IOWR32(A_IR_SENDER, IR_SENDER_CTRL, 0x0);
}

/*********************************************************** Local functions */

#endif // HAVE_IR

