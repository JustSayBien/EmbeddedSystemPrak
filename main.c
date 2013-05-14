/******************************************************************************

File: main.c

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Andreas Sabitzer
        Universität Augsburg

Created: 08.05.2013

*******************************************************************************

Modification history:
---------------------

*/


/****************************************************************** Includes */

#include "board.h"
#include "led.h"
#include "pio.h"
#include "roomba.h"
#include "tools.h"
#include "uart.h"

/******************************************************************* Defines */


#define MAX_QUADRANT_COUNT 8
enum programstate { INIT, DRIVE, COLLISION};
enum drivestate {TURNING, DRIVING};


/******************************************************* Function prototypes */



/**
  * \brief  Convert int to ascii 
  *
  *         Convert an integer value to an array of ascii digits with function
  *
  * \param      value	 	the integer value to convert
  * \param 	out		the result array, containing the ascii digits
  *
  */
void intToAscii(int32_t value, int32_t out[]);


/**
  * \brief  Set program state 
  *
  *         Set the program state according to the state chart with this function
  *
  * \param      state	 	the state to set
  */
void setProgramState(enum programstate state);

/**
  * \brief	Handle INIT state
  *
  *         this function handles the INIT programm state
  *
  */
void handleStateInit();

/**
  * \brief	Handle DRIVE state
  *
  *         this function handles the DRIVE programm state
  *
  */
void handleStateDrive();

/**
  * \brief	Handle COLLISION state
  *
  *         this function handles the COLLISION programm state
  *
  */
void handleStateCollision();


/************************************************************** Global const */


/********************************************************** Global variables */


//stores the button bit mask returned from roomba
volatile uint8_t button_state;

//planned distance in centimeters
int32_t planned_distance = 0;

//driven distance in milimeters
int32_t driven_distance = 0;
int32_t driven_quadrant_distance = 0;
int32_t driven_angle = 0;

int32_t quadrant_counter = 0;

//bool
uint8_t is_moving = 0;

//states according to state chart
enum programstate program_state;
enum drivestate drive_state;




/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */


/*********************************************************** Local functions */

int main(int argc, char *argv[]) {

	//init program
	led_set_blue(ledb_vals[0] | ledb_vals[1]);

	button_wait(0);

	init_roomba();


	//init sevenseg
	writeDigits(digits);	

	//set to init programm state
	setProgramState(INIT);

	//main loop
	while(1){
		led_set_blue(ledb_vals[3]);

		//check button here, return value is needed for all states
		button_state = check_button();

		//switch block according to state chart
		switch(program_state){
			case INIT:
				handleStateInit();
				break;
			case DRIVE:
				handleStateDrive();
				break;
			case COLLISION:	
				handleStateCollision();
				break;
		}

		my_msleep(100);	
	}

  	return 0;
}



void intToAscii(int32_t value, int32_t out[]){
	// integer to large
	if(value >= 1000){
		return;	
	}
	if(value < 0){
		out[3] = ASCII_MINUS;
		value *= -1;
	}
	else{
		out[3] = ASCII_PLUS;
	}
	
	out[2] = (value % 1000) / 100 + ASCII_NUMBER_START;
	out[1] = (value % 100) / 10 + ASCII_NUMBER_START;
	out[0] = value % 10 + ASCII_NUMBER_START;
}

void setProgramState(enum programstate state){
	switch(state) {
		case INIT:
			setLed(LED_DOCK_GREEN, 0, 0);	
			break;
		case DRIVE:
			setLed(LED_SPOT_GREEN, 0, 0);
			break;
		case COLLISION:
			setLed(LED_CHECK_ROBOT_RED, 0, 0);
			break;
	}
	program_state = state;
}

void handleStateInit(){
	//increase ascii output at current cursor position
	if(button_state == BTN_SPOT){
		
		if(planned_distance == 200){
			planned_distance = -200;
		}			
		else{
			planned_distance += 10;
		}
	
		intToAscii(planned_distance, digits);
		writeDigits(digits);
	}
				
	//decrease ascii output at current cursor position
	if(button_state == BTN_DOCK){
		if(planned_distance == -200){
			planned_distance = 200;
		}			
		else{
			planned_distance -= 10;
		}

		intToAscii(planned_distance, digits);
		writeDigits(digits);	
	}

	//switch to drive mode
	if(button_state == BTN_DAY){
		driven_distance = 0;
		driven_quadrant_distance = 0;
		driven_angle = 0;
		quadrant_counter = 0;
		is_moving = 0;
		drive_state = DRIVING;
		setProgramState(DRIVE);
	}
}


void handleStateDrive(){


	//query distance and angle sensors
	query_list(packet_queries, QUERY_LENGTH, query_results);

	driven_quadrant_distance += query_results[2];
	driven_distance += query_results[2];
	driven_angle += query_results[3];


	// bumper and wheeldrop collision detected ?
	if(query_results[0] == 1 || query_results[0] == 2 || query_results[0] == 3){
		stop();
		is_moving = 0;
		setProgramState(COLLISION);
		//return immediately, dont query sensors or update ui
		return;
	}
			
		
	switch(drive_state){

		case DRIVING:

			//start driving
			if(!is_moving){
				int16_t velocity = planned_distance < 0 ? DEFAULT_VELOCITY : -DEFAULT_VELOCITY;
				drive(velocity, (int16_t) 0);
				is_moving = 1;
			}

			else{
				// finished driving ?
				if((planned_distance >= 0 && driven_quadrant_distance >= planned_distance * 10) ||
						(planned_distance < 0 && driven_quadrant_distance <= planned_distance * 10)){
					is_moving = 0;
					driven_quadrant_distance = 0;
					quadrant_counter++;
					drive_state = TURNING;
					stop();
				}						
				
			}

			break;
		case TURNING:

			//start turning
			if(!is_moving){
				int16_t velocity = planned_distance < 0 ? DEFAULT_VELOCITY : -DEFAULT_VELOCITY;
				drive(velocity, -1);
				is_moving = 1;
			}
			else{
				//finished turning ?
				if(driven_angle >= 90 || driven_angle <= -90){
					is_moving = 0;
					driven_angle = 0;
					quadrant_counter++;
					drive_state = DRIVING;
					stop();
				}						
				
			}

			break;
		}
				
		//finished whole driving ?
		if(quadrant_counter == 8){
			is_moving = 0;
			setProgramState(INIT);
		}
		else{
			setWeekdayLed(quadrant_counter + 1);
		}
				
				
		//reset
		if(button_state == BTN_CLEAN){
			setProgramState(INIT);
			setWeekdayLed(0);
		}


		// print distance
		intToAscii(driven_distance/10, digits);
		writeDigits(digits);
}

void handleStateCollision(){
	//drive again
	if(button_state == BTN_DAY){
		setProgramState(DRIVE);
	}

	//reset
	if(button_state == BTN_CLEAN){
		setProgramState(INIT);
		setWeekdayLed(0);
	}
}


