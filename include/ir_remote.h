/******************************************************************************

File: ir_remote.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel
        Universität Augsburg

Created: 28.06.2013


******************************************************************************/

/************************************************************* Include Guard */

#ifndef _IR_REMOTE_H
#define _IR_REMOTE_H

/****************************************************************** Includes */

#include "program.h"
#include "workbench.h"
#include "tools.h"
#include <stdint.h>


/******************************************************************* Defines */

#define NUM_ROOMBA_REMOTE_BUTTONS		14

#define ROOMBA_REMOTE_MIN_REPEAT		0
#define ROOMBA_REMOTE_SHORT_REPEAT		1
#define ROOMBA_REMOTE_NORMAL_REPEAT		2
#define ROOMBA_REMOTE_LONG_REPEAT		3
#define ROOMBA_REMOTE_MAX_REPEAT		4

enum roomba_remote_actions {
	ROOMBA_REMOTE_CROSS_UP = 		0x85,
	ROOMBA_REMOTE_CROSS_DOWN = 		0x86,
	ROOMBA_REMOTE_CROSS_RIGHT = 		0x83,
	ROOMBA_REMOTE_CROSS_LEFT = 		0x81,
	ROOMBA_REMOTE_CROSS_OK = 		0x82,
	
	ROOMBA_REMOTE_BACK = 			0x8D,
	ROOMBA_REMOTE_PAUSE = 			0x89,
	ROOMBA_REMOTE_POWER = 			0x8A,

	ROOMBA_REMOTE_NUM_1 = 			0x8C,
	ROOMBA_REMOTE_NUM_2 = 			0x8B,
	ROOMBA_REMOTE_NUM_3 = 			0x84,
	ROOMBA_REMOTE_NUM_4 = 			0x87,
	ROOMBA_REMOTE_NUM_5 = 			0x88,
	ROOMBA_REMOTE_NUM_6 = 			0x8F
};

enum roomba_action_indices {
	ROOMBA_REMOTE_CROSS_UP_INDEX =		0,
	ROOMBA_REMOTE_CROSS_DOWN_INDEX,
	ROOMBA_REMOTE_CROSS_RIGHT_INDEX,
	ROOMBA_REMOTE_CROSS_LEFT_INDEX,
	ROOMBA_REMOTE_CROSS_OK_INDEX,

	ROOMBA_REMOTE_BACK_INDEX,
	ROOMBA_REMOTE_PAUSE_INDEX,
	ROOMBA_REMOTE_POWER_INDEX,

	ROOMBA_REMOTE_NUM_1_INDEX,
	ROOMBA_REMOTE_NUM_2_INDEX,
	ROOMBA_REMOTE_NUM_3_INDEX,
	ROOMBA_REMOTE_NUM_4_INDEX,
	ROOMBA_REMOTE_NUM_5_INDEX,
	ROOMBA_REMOTE_NUM_6_INDEX
};

//extern volatile bool_t roomba_remote_discrete_pressed[NUM_ROOMBA_REMOTE_BUTTONS];

extern millis_t roomba_remote_activation_times[NUM_ROOMBA_REMOTE_BUTTONS];

extern uint8_t raw_ir_data;


/******************************************************* Function prototypes */

enum programstate handleSubstateBaseSetup ();

bool_t intToAscii2placesSigned (int32_t value, int32_t out[]);

bool_t intToAscii3placesUnsigned (uint32_t value, int32_t out[]);

int getIRAction ();

int getIRActionIndex (int character);

void printXcoordinate (workbench *toPrint);

void printYcoordinate (workbench *toPrint);

void printRotation (workbench *toPrint);

void printCoordinate(int8_t coordinate_value);

void printString (char string[]);

void handleBaseSelect (uint8_t base_num);

bool_t checkDiscreteRoombaButtonArray (int index);

#endif  // _IR_REMOTE_H
