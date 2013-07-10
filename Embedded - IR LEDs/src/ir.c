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
#include <roomba.h>

#include <asm/io.h>


/******************************************************************* Defines */


/******************************************************* Function prototypes */


/************************************************************** Global const */


/********************************************************** Global variables */

uint8_t ir_roomba_id, ir_roomba_leds[IR_NUM_SENSORS] = {0,0,0};
uint8_t ir_remote_command = 0;

/*************************************************************** Local const */


/*********************************************************** Local variables */

/* has to be n*m elements long with n being the number of IR sensors (3) and
 * m being the number of IR leds to be received.
 */
static volatile uint16_t ir_timer_leds[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

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

void ir_receive() {
	uint8_t s, sensor_id, i, ir_raw, ir_leds, ir_id, ir_remote;

	/* check all sensors */
	for(s=0; s<IR_NUM_SENSORS; s++) {
	
		/* select appropriate sensor id */
		switch(s) {
			case IR_RECEIVER_OMNI:	sensor_id = SENSOR_IR_OMNI; break;
			case IR_RECEIVER_LEFT: sensor_id = SENSOR_IR_LEFT; break;
			case IR_RECEIVER_RIGHT: sensor_id = SENSOR_IR_RIGHT; break;
			default: return;
		}

		/* read from roomba */
		uart_write_byte(CMD_SENSORS);
		uart_write_byte(sensor_id);
		ir_raw = uart_read_byte();

		/* per default, nothing is received */
		ir_leds = 0;
		ir_id = 0;
		ir_remote = 0;
		
		if(ir_raw != 0) {
			/* something received */
			
			if((ir_raw & IR_SENDER_MASK) == IR_SENDER_ROOMBA) {
				/* roomba ir leds on back */
				ir_id = (ir_raw & IR_ROOMBA_MASK)>>3;
				ir_leds = ir_raw & IR_DATA_MASK;
			} else if((ir_raw & IR_SENDER_MASK) == IR_SENDER_REMOTE) {
				/* remote ir command */
				ir_remote = ir_raw & IR_DATA_MASK;
			}
		}
	
		/* update state of all IR leds */
		for(i=0; i<IR_NUM_LEDS; i++) {
			if(ir_leds & (1<<i)) {
				/* led is on now */
				ir_roomba_leds[s] |= (1<<i);
				/* reset timer */
				ir_timer_leds[s*IR_NUM_LEDS+i] = 0;
			} else if(ir_roomba_leds[s] & (1<<i)) {
				/* led is off */
				if(ir_timer_leds[s*IR_NUM_LEDS+i] > IR_TIMER_MAX_VAL) {
					/* led is really of if timed out */
					/* set led state to OFF */
					ir_roomba_leds[s] &= ~(1<<i);
				}
			}
		}
		
		/* set remote command if received */
		if(ir_remote != 0)
			ir_remote_command = ir_remote;
	}

	if(ir_roomba_leds[0] & (IR_LED_LEFT|IR_LED_CENTER|IR_LED_RIGHT)) { // at least one led active
		ir_roomba_id = ir_id;
	} else {
		ir_roomba_id = 0;
	}

}

/*********************************************************** Local functions */

#endif // HAVE_IR

