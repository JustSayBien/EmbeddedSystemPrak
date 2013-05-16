/******************************************************************************

File: roomba.c

Project: Roomba Embedded Systems Training Blatt3

Description: Roomba packet struct

Author: Andreas Sabitzer
        Universität Augsburg

Created: 30.04.2013

*******************************************************************************

Modification history:
---------------------

*/



/****************************************************************** Includes */

#include <roomba.h>
#include "tools.h"



/******************************************************************* Defines */
const packet * packet_queries[QUERY_LENGTH] = {&PACKET_BUMPS_WHEELDROPS, &PACKET_INFRARED_CHARACTER_OMNI, &PACKET_DISTANCE, &PACKET_ANGLE, &PACKET_TEMPERATURE};
volatile int32_t query_results[QUERY_LENGTH];

roomba_data roombadata = {0, 0, 360, 10};

// array of currently displayed digits on Roomba's seven segment display
int32_t digits[DIGIT_LENGTH] = {
        48,
        48,
        48,
        48
};



/******************************************************* Function prototypes */


/************************************************************** Global const */

const packet PACKET_BUMPS_WHEELDROPS          = {7, 1, 0};
const packet PACKET_WALL                      = {8, 1, 0};
const packet PACKET_CLIFF_LEFT                = {9, 1, 0};
const packet PACKET_CLIFF_FRONT_LEFT          = {10, 1, 0};
const packet PACKET_CLIFF_FRONT_RIGHT         = {11, 1, 0};
const packet PACKET_CLIFF_RIGHT               = {12, 1, 0};
const packet PACKET_VIRTUAL_WALL              = {13, 1, 0};
const packet PACKET_WHEEL_OVERCURRENTS        = {14, 1, 0};
const packet PACKET_DIRT_DETECT               = {15, 1, 0};
const packet PACKET_UNUSED_BYTE               = {16, 1, 0};
const packet PACKET_INFRARED_CHARACTER_OMNI   = {17, 1, 0};
const packet PACKET_BUTTONS                   = {18, 1, 0};
const packet PACKET_DISTANCE                  = {19, 2, 1};
const packet PACKET_ANGLE                     = {20, 2, 1};
const packet PACKET_CHARGING_STATE            = {21, 1, 0};
const packet PACKET_VOLTAGE                   = {22, 2, 0};
const packet PACKET_CURRENT                   = {23, 2, 1};
const packet PACKET_TEMPERATURE               = {24, 1, 1};
const packet PACKET_BATTERY_CHARGE            = {25, 2, 0};
const packet PACKET_BATTERY_CAPICITY          = {26, 2, 0};
const packet PACKET_WALL_SIGNAL               = {27, 2, 0};
const packet PACKET_CLIFF_LEFT_SIGNAL         = {28, 2, 0};
const packet PACKET_CLIFF_FRONT_LEFT_SIGNAL   = {29, 2, 0};
const packet PACKET_CLIFF_FRONT_RIGHT_SIGNAL  = {30, 2, 0};
const packet PACKET_CLIFF_RIGHT_SIGNAL        = {31, 2, 0};
const packet PACKET_CHARGING_SOURCES_AVAILABLE= {34, 1, 0};
const packet PACKET_OI_MODE                   = {35, 1, 0};
const packet PACKET_SONG_NUMBER               = {36, 1, 0};
const packet PACKET_SONG_PLAYING              = {37, 1, 0};
const packet PACKET_NUMBER_OF_STREAM_PACKETS  = {38, 1, 0};
const packet PACKET_REQUESTED_VELOCITY        = {39, 2, 1};
const packet REQUESTED_RADIUS                 = {40, 2, 1};
const packet PACKET_REQUESTED_RIGHT_VELOCITY  = {41, 2, 1};
const packet PACKET_REQUESTED_LEFT_VELOCITY   = {42, 2, 1};
const packet PACKET_RIGHT_ENCODER_COUNTS      = {43, 2, 0};
const packet PACKET_LEFT_ENCODER_COUNTS       = {44, 2, 0};
const packet PACKET_LIGHT_BUMPER              = {45, 1, 0};
const packet PACKET_LIGHT_BUMP_LEFT_SIGNAL    = {46, 2, 0};
const packet PACKET_LIGHT_BUMP_FRONT_LEFT_SIGNAL      = {47, 2, 0};
const packet PACKET_LIGHT_BUMP_CENTER_LEFT_SIGNAL     = {48, 2, 0};
const packet PACKET_LIGHT_BUMP_CENTER_RIGHT_SIGNAL    = {49, 2, 0};
const packet PACKET_LIGHT_BUMP_FRONT_RIGHT_SIGNAL     = {50, 2, 0};
const packet PACKET_LIGHT_BUMP_RIGHT_SIGNAL   = {51, 2, 0};
const packet PACKET_INFRARED_CHARACTER_LEFT   = {52, 1, 0};
const packet PACKET_INFRARED_CHARACTER_RIGHT  = {53, 1, 0};
const packet PACKET_LEFT_MOTOR_CURRENT        = {54, 2, 1};
const packet PACKET_RIGHT_MOTOR_CURRENT       = {55, 2, 1};
const packet PACKET_MAIN_BRUSH_MOTOR_CURRENT  = {56, 2, 1};
const packet PACKET_SIDE_BRUSH_MOTOR_CURRENT  = {57, 2, 1};
const packet PACKET_STASIS                    = {58, 1, 0};


/********************************************************** Global variables */

/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */
void init_roomba() {
	uart_write_byte(CMD_START);
	uart_write_byte(CMD_FULL);

	uart_write_byte(141);
	uart_write_byte(4);
}

void roomba_calibrate_angle() {

	digits[0] = 'A';
	digits[1] = 'L';
	digits[2] = 'A';
	digits[3] = 'C';
	writeDigits();

	uint8_t button_state;
	bool_t moving = false;
	while(1){
		button_state = check_button();
		if(!moving && button_state == BTN_CLEAN){
			drive(DEFAULT_VELOCITY, 1);
			moving = true;
		}
		else if(moving && button_state == BTN_CLEAN){
			stop();
			moving = false;
			int32_t angle = query_sensor(PACKET_ANGLE);
			roombadata.angle_360_degrees = angle;
			break;
		}
		my_msleep(150);
	}
}	

void roomba_calibrate_distance(){
	digits[0] = 'D';
	digits[1] = 'L';
	digits[2] = 'A';
	digits[3] = 'C';
	writeDigits();
}



int32_t query_sensor(packet query_packet){
	uart_write_byte(CMD_QUERY_SENSOR);
	uart_write_byte(query_packet.id);

	int32_t result;
	uint8_t temp = uart_read_byte();
	if(query_packet.length == 1){
		if(query_packet.has_sign){
			int8_t tempSigned = (int8_t) temp;
			result = (int32_t) tempSigned;		
		}
		else{
			result = (int32_t) temp;
		}
	}
	else{
		uint16_t tempConcat = temp << 8;
		temp = uart_read_byte();
		tempConcat |= temp;
			
		if(query_packet.has_sign){
			int16_t tempConcatSigned = (int16_t) tempConcat;
			result = (int32_t) tempConcatSigned;
		}
		else{
			result =  (int32_t) tempConcat;
		}
	}

	return result;
}


void query_list(const packet * packets[], uint8_t count, int32_t results[]){
	write_query_list(packets, count);
	read_query_list(packets, count, results);
}

void write_query_list(const packet * packets[], uint8_t count){
	uart_write_byte(CMD_QUERY_LIST);
	uart_write_byte(count);
	int i;
	for(i=0; i < count; i++){
		uart_write_byte(packets[i]->id);	
	}	
}

void read_query_list(const packet * packets[], uint8_t count, int32_t results[]){
	int i;
	for(i=0; i < count; i++){

		uint8_t temp = uart_read_byte();
		if(packets[i]->length == 1){
			if(packets[i]->has_sign){
				int8_t tempSigned = (int8_t) temp;
				results[i] = (int32_t) tempSigned;	
			}
			else{
				results[i] = (int32_t) temp;
			}
		}
		else{
			uint16_t tempConcat = temp << 8;
			temp = uart_read_byte();
			tempConcat |= temp;
			
			if(packets[i]->has_sign){
				int16_t tempConcatSigned = (int16_t) tempConcat;
				results[i] = (int32_t) tempConcatSigned;
			}
			else{
				results[i] =  (int32_t) tempConcat;
			}
		}
	}
}

	

void writeDigits () {
        uart_write_byte(CMD_7SEG_ASCII);
	int i;
	for(i=3; i >= 0; i--){
        	uart_write_byte(digits[i]);
	}
}
 
void setWeekdayLed (uint8_t led_mask){
        uart_write_byte(CMD_SCHEDULE_LED);
        uart_write_byte(led_mask);
        uart_write_byte((uint8_t)0);
	uart_write_byte((uint8_t)0);
}

void setLed(uint8_t led_mask, uint8_t color, uint8_t intensity) {
        uart_write_byte(CMD_LEDS);
        uart_write_byte(led_mask);
        uart_write_byte(color);
        uart_write_byte(intensity);
}
 

void drive(int16_t velocity, int16_t radius) {
	uart_write_byte(CMD_DRIVE);
	uart_write_byte((uint8_t) (velocity >> 8));
	uart_write_byte((uint8_t) (velocity & 0xFF));
	uart_write_byte((uint8_t) (radius >> 8));
	uart_write_byte((uint8_t) (radius & 0xFF));
}



void stop(){
	drive(0, 0);
}

uint8_t check_button(){
	uart_write_byte(CMD_BUTTONS_STATE);
	uart_write_byte(BTN_PACKET_ID);
	return uart_read_byte();
}


int32_t calculateTimeToDrive (int32_t distance, int32_t velocity){
	return ((int32_t)(distance/velocity))*1000;
}
 


/*********************************************************** Local functions */
