/*! \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * etc...
 */
/******************************************************************************

File: main.c

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel
        Universität Augsburg

Created: 08.06.2013


*******************************************************************************

Modification history:
---------------------

*/


/****************************************************************** Includes */

#include "program.h"
#include "roomba.h"
#include "workbench.h"
#include "button.h"

#ifdef HAVE_IR
	#include <asm/io.h>
	#include "ir.h"
#endif

/******************************************************************* Defines */

/******************************************************* Function prototypes */


/*************************************************************** Local const */

/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */


/*********************************************************** Local functions */



uint8_t base_id_codes[MAX_COUNT_WORKBENCHES] = {
	0xB0,
	0xC0,
	0xE0,
	0x70,
	0x30,
	0x10
};

uint8_t base_id = 5;



int main(int argc, char *argv[]) {
	led_set_blue(ledb_vals[0] | ledb_vals[1]);

	//button_wait(0);
	
	//led_set_blue(ledb_vals[2] | ledb_vals[3]);
	
	//ir_sender_setup();
	//ir_sender_set((ROOMBA_ID&0x03)<<4 | IR_SENDER_ID | IR_LED_LEFT, (ROOMBA_ID&0x03)<<4 | IR_SENDER_ID | IR_LED_CENTER, (ROOMBA_ID&0x03)<<4 | IR_SENDER_ID | IR_LED_RIGHT, (ROOMBA_ID&0x03)<<4 | ir_next_base_id | IR_LED_NEAR);
		
	IOWR32(A_IR_SENDER, 0x4, base_id_codes[base_id-1] | 0x5);
	//IOWR32(A_IR_SENDER, 0x4, 0xF8F4F2F1);
	//ir_sender_set(0x80 | IR_LED_RIGHT, 0x80 | IR_LED_CENTER, 0x80 | IR_LED_LEFT, 0x80 | IR_LED_NEAR);
	ir_sender_on();
	while(true) {
		ir_sender_off();
		my_msleep(500);
		ir_sender_on();
		my_msleep(50);
	}
	
  	return 0;
}

