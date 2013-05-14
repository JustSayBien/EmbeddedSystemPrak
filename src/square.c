/******************************************************************************

File: square.c

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel

Created: 09.05.2011

********************************************************************************/


/****************************************************************** Includes */

#include "led.h"
#include "pio.h"
#include "tools.h"
#include "uart.h"

#include "mymath.h"
#include "square.h"
#include "util.h"
#include "rutil.h"
#include "de270util.h"

/******************************************************************* Defines */

enum states {
	INIT_MODE,
	DRIVE_MODE,
	STOP_MODE,
	ERROR_MODE
};

enum drive_states {
	DRIVE_STRAIGHT_MODE,
	DRIVE_TURN_MODE
};

enum init_states {
	INIT_ANGLE_MODE,
	INIT_SQUARESIDE_MODE,
	INIT_VELOCITY_MODE
};


/******************************************************* Function prototypes */


/************************************************************** Global const */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */

/*
 * query_IDs is set to:
 *  [0]  PACKET_DISTANCE
 *  [1]  PACKET_ANGLE
 *  [2]  PACKET_BUMPS_WHEELDROPS
 * in initRoomba()
 * 
*/
const packet *query_IDs[3];
int query_response[3];

// VARIABLES FOR ALL LEVELS OF STATEMACHINE
enum states state = INIT_MODE;
enum drive_states drive_state = DRIVE_STRAIGHT_MODE;
enum init_states init_state = INIT_ANGLE_MODE;

// VARIABLES FOR DRIVEING PARAMETERS
int driven_distance = 0;
int driven_angle = 0;
int total_distance = 0;
uint8_t quadrant = 0;
int square_side = 0;
int velocity = 300;
int calibrated_angle = 0;

// STATUS VARIABLES TO PREVENT REEXECUTION OF ONCE ONLY CODE IN STATES
bool_t isDriving = false;
bool_t isTurning = false;
bool_t inited = false;
bool_t stopped = false;
bool_t angle_calibrated = false;

// VARIABLES FOR BLINKING RED LED ON ERROR
uint8_t error = 0;
uint8_t error_blink_counter = 0;
bool_t error_blink_on = true;

/******************************************************************** Macros */

/*#define BUTTON_PRESSED_PRE (NUM) 											\
	if (roomba_button_pressed[NUM] && BUTTON_WAIT_RELEASE[NUM] == 0) {	\
		BUTTON_WAIT_RELEASE[NUM] = 1;

#define BUTTON_PRESSED_POST_DISCRETE (NUM)									\
	} else if (!roomba_button_pressed[NUM]) {								\
		BUTTON_WAIT_RELEASE[NUM] = 0;										\
	}

#define BUTTON_PRESSED_POST_ITERATIVE }*/

/********************************************************** Global functions */


/*********************************************************** Local functions */

/*
 *		BUTTON 0 (DAY):		DECREASE VALUE
 * 		BUTTON 1 (HOUR):	INCREASE VALUE
 * 		BUTTON 2 (MINUTE):	NEXT VALUE / START DRIVING
 * 		BUTTON 3 (CLEAN):	
 * 		BUTTON 4 (SPOT):	
 * 		BUTTON 5 (DOCK):	RESET
 */

void square_main () {
	BUTTON_WAIT(0);
	LED = led_vals[0];
	initRoomba();
	setupQueryIDs();
	msleep(1000);
	LED = led_vals[0] | led_vals[1];
	BUTTON_WAIT(1);
	
	while (true) {
		// Query status of the roomba's buttons, which are needed in most substates
		Roomba_refreshButtons();
		
		switch (state) {
			case INIT_MODE:
				initProgram();
				switch (init_state) {
					case INIT_ANGLE_MODE:
						handleAngleCalibration();
						break;
						
					case INIT_SQUARESIDE_MODE:
						handleSquaresideSetup();
						break;
				
					case INIT_VELOCITY_MODE:
						handleVelocitySetup();
						break;
				}
				break;
			
			case DRIVE_MODE:
				retrieveDriveData();
				if (error != 0) {
					state = ERROR_MODE;
					break;
				}
				
				switch (drive_state) {
					case DRIVE_STRAIGHT_MODE:
						handleDriveStraightMode();
						break;
					case DRIVE_TURN_MODE:
						handleTurnMode();
						break;
				}
				break;
				
			case STOP_MODE:
				handleStopMode();
				break;
			
			case ERROR_MODE:
				handleCollision();
				break;
		}
		msleep(50);
	}
}

void handleAngleCalibration () {
	calibrated_angle = 900;
	
	angle_calibrated = true;
	init_state = INIT_SQUARESIDE_MODE;
	
	intToASCII(calibrated_angle / 10, roomba_digits);
	Roomba_commitDigits();
	msleep(1000);
}

void handleSquaresideSetup () {
	if (roomba_button_pressed[ROOMBA_BUTTON_DAY] && !BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_DAY]) {
		BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_DAY] = true;
		if (square_side > -200)
			square_side -= 10;
	} else if (!roomba_button_pressed[ROOMBA_BUTTON_DAY]) {
		BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_DAY] = false;
	}
				
	if (roomba_button_pressed[ROOMBA_BUTTON_HOUR] && !BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_HOUR]) {
		BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_HOUR] = true;
		if (square_side < 200)
			square_side += 10;
	} else if (!roomba_button_pressed[ROOMBA_BUTTON_HOUR]) {
		BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_HOUR] = false;
	}
	
	if (roomba_button_pressed[ROOMBA_BUTTON_MINUTE] && !BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_MINUTE]) {
		BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_MINUTE] = true;
		init_state = INIT_VELOCITY_MODE;
	} else if (!roomba_button_pressed[ROOMBA_BUTTON_MINUTE]) {
		BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_MINUTE] = false;
	}
	
	intToASCII(square_side, roomba_digits);
	Roomba_commitDigits();
}

void handleVelocitySetup () {
	if (roomba_button_pressed[ROOMBA_BUTTON_DAY] && !BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_DAY]) {
		BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_DAY] = true;
		if (velocity > 0)
			velocity -= 10;
	} else if (!roomba_button_pressed[ROOMBA_BUTTON_DAY]) {
		BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_DAY] = false;
	}
				
	if (roomba_button_pressed[ROOMBA_BUTTON_HOUR] && !BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_HOUR]) {
		BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_HOUR] = true;
		if (velocity < 500)
			velocity += 10;
	} else if (!roomba_button_pressed[ROOMBA_BUTTON_HOUR]) {
		BUTTON_WAIT_RELEASE[ROOMBA_BUTTON_HOUR] = false;
	}
	
	if (roomba_button_pressed[ROOMBA_BUTTON_MINUTE]) {
		velocity = square_side < 0 ? velocity : -velocity;
		state = DRIVE_MODE;
		drive_state = DRIVE_STRAIGHT_MODE;
		Roomba_clearDigits();
		return;
	}
	
	intToASCII(velocity, roomba_digits);
	Roomba_commitDigits();
}

void retrieveDriveData () {
	Roomba_queryList(3, query_IDs, query_response);
	driven_distance += query_response[0];
	driven_angle += query_response[1];
	error = query_response[2];
}

void handleDriveStraightMode () {
	SEVEN_SEG = myAbs(driven_distance);
	LED = (de270_led_status & ~led_vals[0]) | led_vals[1];
	if (!isDriving) {
		isDriving = true;
		Roomba_LEDCounter(++quadrant);
		de270_LEDCounter(quadrant);
		Roomba_commitWeekdayLEDs(quadrant);
		Roomba_drive(velocity);
	}
	if (!checkDriveProgress()) {
		Roomba_stop();
		isDriving = false;
		total_distance += driven_distance;
		intToASCII(total_distance/10, roomba_digits);
		Roomba_commitDigits();
		driven_distance = 0;
		drive_state = DRIVE_TURN_MODE;
	}
}

void handleTurnMode () {
	SEVEN_SEG = myAbs(driven_angle);
	LED = (de270_led_status & ~led_vals[1]) | led_vals[0];
	if (!isTurning) {
		isTurning = true;
		Roomba_turn(velocity, 1);
	}
	if (!checkTurnProgress()) {
		Roomba_stop();
		isTurning = false;
		driven_angle = 0;
		if (quadrant < 4) {
			drive_state = DRIVE_STRAIGHT_MODE;
		} else {
			state = STOP_MODE;
		}
	}
}

void handleStopMode () {
	if (!stopped) {
		stopped = true;
		intToASCII(total_distance/10, roomba_digits);
		Roomba_commitDigits();
		LED = (de270_led_status & ~(led_vals[0] | led_vals[1]));
		roomba_power_led_color_status = 0;
		roomba_power_led_intensity_status = 255;
		Roomba_commitLEDs();
	}
	if (roomba_button_pressed[ROOMBA_BUTTON_DOCK]) {
		inited = false;
		state = INIT_MODE;
	}
}

void handleCollision () {
	SEVEN_SEG = error_blink_counter;
	if (error_blink_counter < 20) {
		error_blink_counter++;
	} else {
		roomba_power_led_color_status = 255;
		roomba_power_led_intensity_status = error_blink_on ? 255 : 0;
		Roomba_commitLEDs();
		error_blink_on = !error_blink_on;
		error_blink_counter = 0;
	}
	if (roomba_button_pressed[ROOMBA_BUTTON_DOCK]) {
		state = DRIVE_MODE;
		error_blink_on = true;
		error_blink_counter = 0;
	}
}

void initRoomba () {
	uart_write_byte(CMD_START);
	uart_write_byte(CMD_FULL);
}

void setupQueryIDs () {
	query_IDs[0] = PACKET_DISTANCE;
	query_IDs[1] = PACKET_ANGLE;
	query_IDs[2] = PACKET_BUMPS_WHEELDROPS;
}

void initProgram () {
	if (!inited) {
		LED = 0;
		roomba_led_status = 0;
		roomba_power_led_color_status = 0;
		roomba_power_led_intensity_status = 0;
		Roomba_commitLEDs();
		Roomba_commitWeekdayLEDs(0);
		intToASCII(0, roomba_digits);
		Roomba_commitDigits();
		
		drive_state = DRIVE_STRAIGHT_MODE;
		init_state = angle_calibrated ? INIT_SQUARESIDE_MODE : INIT_ANGLE_MODE;

		driven_distance = 0;
		driven_angle = 0;
		total_distance = 0;
		quadrant = 0;
		square_side = 0;
		velocity = 0;
		isDriving = false;
		isTurning = false;
		stopped = false;
		inited = true;
	}
}

bool_t checkDriveProgress () {
	return (myAbs(driven_distance) <= myAbs(square_side) * 10);
}

bool_t checkTurnProgress () {
	return (myAbs(driven_angle) <= myAbs(calibrated_angle));
}
