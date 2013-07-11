/******************************************************************************

File: ir_remote.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel
        Universität Augsburg

Created: 28.06.2013


******************************************************************************/

/****************************************************************** Includes */

#include "ir_remote.h"
#include "program.h"
#include "roomba.h"
#include "workbench.h"
#include "button.h"
#include "mymath.h"

#include <asm/io.h>


bool_t roomba_remote_discrete_pressed[NUM_ROOMBA_REMOTE_BUTTONS] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false
};

millis_t roomba_remote_activation_times[NUM_ROOMBA_REMOTE_BUTTONS] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

uint8_t roomba_remote_default_repeat_times[NUM_ROOMBA_REMOTE_BUTTONS] = {
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_NORMAL_REPEAT,
	ROOMBA_REMOTE_NORMAL_REPEAT,
	ROOMBA_REMOTE_NORMAL_REPEAT,
	ROOMBA_REMOTE_NORMAL_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT
};

uint8_t roomba_remote_repeat_times[NUM_ROOMBA_REMOTE_BUTTONS] = {
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_NORMAL_REPEAT,
	ROOMBA_REMOTE_NORMAL_REPEAT,
	ROOMBA_REMOTE_NORMAL_REPEAT,
	ROOMBA_REMOTE_NORMAL_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT,
	ROOMBA_REMOTE_LONG_REPEAT
};

unsigned long roomba_remote_repeat_counters[NUM_ROOMBA_REMOTE_BUTTONS] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

uint16_t roomba_remote_repeat_time_values[] = {
	100,	// ROOMBA_REMOTE_MIN_REPEAT
	200,	// ROOMBA_REMOTE_SHORT_REPEAT
	400,	// ROOMBA_REMOTE_NORMAL_REPEAT
	650,	// ROOMBA_REMOTE_LONG_REPEAT
	2000,	// ROOMBA_REMOTE_MAX_REPEAT
};

uint8_t current_base = 0;


int led = 0;
int power = 0;
int intensity = 0;

// UP, DOWN:  CHANGE VALUE
// OK:	      NEXT_VALUE / FINISH
// NUM KEYS:  SELECT BASE TO CONFIGURE

// BASE_COORDINATE_X, BASE_COORDINATE_Y: METERS 1-99
// BASE_ROTATION: SAVE INCREMENTS OF 45 DEGREES

workbench* current_workbench;

enum programstate handleSubstateBaseSetup () {
	switch (base_config_state) {
		case BASE_SELECT:		
			roomba_sevenseg_digits[3] = 'B';
			roomba_sevenseg_digits[2] = 'A';
			roomba_sevenseg_digits[1] = 'S';
			roomba_sevenseg_digits[0] = 'E';
			write_sevenseg_digits();
			
			switch (ir_action) {
				/*case ROOMBA_REMOTE_CROSS_OK:
					//my_msleep(500);
					return DRIVE;
					break;*/
				case ROOMBA_REMOTE_NUM_2:
					handleBaseSelect(2);
					break;
				case ROOMBA_REMOTE_NUM_3:
					handleBaseSelect(3);
					break;
				case ROOMBA_REMOTE_NUM_4:
					handleBaseSelect(4);
					break;
				case ROOMBA_REMOTE_NUM_5:
					handleBaseSelect(5);
					break;
				/*case ROOMBA_REMOTE_NUM_6:
					handleBaseSelect(6);
					break;*/
				default:
					break;
			}
			break;

		case BASE_COORDINATE_X:					
			switch (ir_action) {
				case ROOMBA_REMOTE_BACK:
					base_config_state = BASE_SELECT;
					break;
				case ROOMBA_REMOTE_CROSS_OK:
					base_config_state = BASE_COORDINATE_Y;
					break;
				case ROOMBA_REMOTE_CROSS_UP:
					if (current_workbench->distance_to_base_x < 99)
						current_workbench->distance_to_base_x++;
					break;
				case ROOMBA_REMOTE_CROSS_DOWN:
					if (current_workbench->distance_to_base_x > -99)
						current_workbench->distance_to_base_x--;
					break;
				default:
					break;
			}
			printXcoordinate(current_workbench);
			break;


		case BASE_COORDINATE_Y:
			switch (ir_action) {
				case ROOMBA_REMOTE_BACK:
					base_config_state = BASE_COORDINATE_X;
					break;
				case ROOMBA_REMOTE_CROSS_OK:
					base_config_state = BASE_ROTATION;
					break;
				case ROOMBA_REMOTE_CROSS_UP:
					if (current_workbench->distance_to_base_y < 99)
						current_workbench->distance_to_base_y++;
					break;
				case ROOMBA_REMOTE_CROSS_DOWN:
					if (current_workbench->distance_to_base_y > -99)
						current_workbench->distance_to_base_y--;
					break;
				default:
					break;
			}
			
			printYcoordinate(current_workbench);
			break;

		case BASE_ROTATION:
			switch (ir_action) {
				case ROOMBA_REMOTE_BACK:
					base_config_state = BASE_COORDINATE_Y;
					break;
				case ROOMBA_REMOTE_CROSS_OK:
					if (current_workbench->distance_to_base_x != 0 && current_workbench->distance_to_base_y != 0) {
						current_workbench->id = current_base;
					}
					current_base = 0;
					base_config_state = BASE_SELECT;
					break;
				case ROOMBA_REMOTE_CROSS_UP:
					if (current_workbench->dock_angle_multiplier < 7)
						current_workbench->dock_angle_multiplier++;
					break;
				case ROOMBA_REMOTE_CROSS_DOWN:
					if (current_workbench->dock_angle_multiplier > 0)
						current_workbench->dock_angle_multiplier--;
					break;
				default:
					break;
			}
			printRotation(current_workbench);
			break;

		default:
			led &= ~LED_DOCK_GREEN;
			break;
	}
	return CALIBRATE;
}

bool_t intToAscii2placesSigned(int32_t value, int32_t out[]) {
	bool_t sign = mySign(value);
	value = myAbs(value);
	uint8_t hundreds = (value % 100) / 10;
	uint8_t sign_place = 2;
	if (hundreds > 0) {
		out[1] = hundreds + ASCII_NUMBER_START;
	} else {
		sign_place = 1;
	}
	out[0] = value % 10 + ASCII_NUMBER_START;
	
	out[sign_place] = sign ? '-' : ' ';
	
	
	// integer too large
	if(value >= 100)
		return false;
	else 
		return true;
}

bool_t intToAscii3placesUnsigned(uint32_t value, int32_t out[]) {
	value = myAbs(value);
	out[2] = (value % 1000) / 100 + ASCII_NUMBER_START;
	out[1] = (value % 100) / 10 + ASCII_NUMBER_START;
	out[0] = value % 10 + ASCII_NUMBER_START;
	
	// integer too large
	if(value >= 1000)
		return false;
	else 
		return true;
}

int getIRActionIndex (int character) {
	switch (character) {
		case ROOMBA_REMOTE_CROSS_OK:
			return ROOMBA_REMOTE_CROSS_OK_INDEX;
		case ROOMBA_REMOTE_CROSS_UP:
			return ROOMBA_REMOTE_CROSS_UP_INDEX;
		case ROOMBA_REMOTE_CROSS_DOWN:
			return ROOMBA_REMOTE_CROSS_DOWN_INDEX;
		case ROOMBA_REMOTE_CROSS_RIGHT:
			return ROOMBA_REMOTE_CROSS_RIGHT_INDEX;
		case ROOMBA_REMOTE_CROSS_LEFT:
			return ROOMBA_REMOTE_CROSS_LEFT_INDEX;
		case ROOMBA_REMOTE_BACK:
			return ROOMBA_REMOTE_BACK_INDEX;
		case ROOMBA_REMOTE_PAUSE:
			return ROOMBA_REMOTE_PAUSE_INDEX;
		case ROOMBA_REMOTE_POWER:
			return ROOMBA_REMOTE_POWER_INDEX;
		case ROOMBA_REMOTE_NUM_1:
			return ROOMBA_REMOTE_NUM_1_INDEX;
		case ROOMBA_REMOTE_NUM_2:
			return ROOMBA_REMOTE_NUM_2_INDEX;
		case ROOMBA_REMOTE_NUM_3:
			return ROOMBA_REMOTE_NUM_3_INDEX;
		case ROOMBA_REMOTE_NUM_4:
			return ROOMBA_REMOTE_NUM_4_INDEX;
		case ROOMBA_REMOTE_NUM_5:
			return ROOMBA_REMOTE_NUM_5_INDEX;
		case ROOMBA_REMOTE_NUM_6:
			return ROOMBA_REMOTE_NUM_6_INDEX;
		default:
			return -1;
	}
}

bool_t checkDiscreteRoombaButtonArray (int index) {
	int i;
	for (i=0; i<NUM_ROOMBA_REMOTE_BUTTONS; i++) {
		if (i != index) {
			roomba_remote_discrete_pressed[i] = 0;
			roomba_remote_activation_times[i] = 0;
			roomba_remote_repeat_times[i] = roomba_remote_default_repeat_times[i];
		} else {
			//led = 0;
			millis_t difference = global_clock - roomba_remote_activation_times[i];
			if (roomba_remote_activation_times[i] == 0 || difference >= roomba_remote_repeat_time_values[roomba_remote_repeat_times[i]]) {
				roomba_remote_discrete_pressed[i] = 1;
				roomba_remote_activation_times[i] = global_clock;
				roomba_remote_repeat_counters[i]++;
				led_set_blue(roomba_remote_repeat_counters[i]);
				
				led = 0;
				if (roomba_remote_repeat_counters[i] >= 10 && roomba_remote_repeat_times[i] > 0) {
					roomba_remote_repeat_times[i]--;
					roomba_remote_repeat_counters[i] = 0;
					led = LED_DIRT_DETECT_BLUE;
				}
			} else {
				roomba_remote_discrete_pressed[i] = 0;
			}
		}
	}
	return roomba_remote_discrete_pressed[index];
}

int getIRAction () {
	int character = query_sensor(PACKET_INFRARED_CHARACTER_OMNI);
	int index = getIRActionIndex(character);
	
	int power = 0;
	int intensity = 0;
	
	if (roomba_remote_discrete_pressed[index]) {
		power = 255;
		intensity = 255;
	}

	uint32_t pressed = checkDiscreteRoombaButtonArray(index);
	
	if (index != -1 && pressed == 1)
		return character;
	return -1;
}

void printXcoordinate (workbench *toPrint) {
	roomba_sevenseg_digits[3] = 'H';
	roomba_sevenseg_digits[2] = ' ';
	roomba_sevenseg_digits[1] = ' ';
	roomba_sevenseg_digits[0] = ' ';
	printCoordinate(toPrint->distance_to_base_x);
}

void printYcoordinate (workbench *toPrint) {
	roomba_sevenseg_digits[3] = 'Y';
	roomba_sevenseg_digits[2] = ' ';
	roomba_sevenseg_digits[1] = ' ';
	roomba_sevenseg_digits[0] = ' ';
	printCoordinate(toPrint->distance_to_base_y);
}

void printRotation (workbench *toPrint) {
	roomba_sevenseg_digits[3] = (char) 94;
	if (intToAscii3placesUnsigned(toPrint->dock_angle_multiplier * 45, roomba_sevenseg_digits))
		write_sevenseg_digits();
}

void printCoordinate(int8_t coordinate_value) {
	if (intToAscii2placesSigned(coordinate_value, roomba_sevenseg_digits))
		write_sevenseg_digits();
}

void printString (char string[]) {
	int i;
	for (i=0; i<4; i++) {
		roomba_sevenseg_digits[i] = string[i];
	}
	write_sevenseg_digits();
}

void handleBaseSelect (uint8_t base_num) {
	current_base = base_num;
	base_config_state = BASE_COORDINATE_X;
	
	current_workbench = (&workbenches[base_num-1]);
	
	roomba_sevenseg_digits[3] = 'B';
	roomba_sevenseg_digits[2] = 'A';
	roomba_sevenseg_digits[1] = ' ';
	roomba_sevenseg_digits[0] = (base_num + ASCII_NUMBER_START);
	
	write_sevenseg_digits();
	
	my_msleep(2000);
}
