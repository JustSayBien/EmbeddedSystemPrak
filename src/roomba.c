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

#include "roomba.h"
#include "uart.h"
#include "mymath.h"
#include "workbench.h"



/******************************************************************* Defines */
roomba_data roombadata = {0, 0, 0, 0, 0, 0, 130, 1000};
collision_data collisiondata = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// array of currently displayed digits on Roomba's seven segment display
uint8_t roomba_sevenseg_digits[DIGIT_LENGTH] = {
        ASCII_NUMBER_START,
        ASCII_NUMBER_START,
        ASCII_NUMBER_START,
        ASCII_NUMBER_START
};

bool_t defined_song_done = false;
bool_t defined_song_theme = false;
bool_t defined_song_collision = false;
bool_t defined_song_beep = false;


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


void roombaInit() {
	uart_write_byte(CMD_START);
	uart_write_byte(CMD_FULL);

	my_msleep(1000);

	//play sound
	//uart_write_byte(141);
	//uart_write_byte(4);
}

void roombaCalibrateAngle() {
	roombaResetTrips();
	
	my_msleep(300);

	uint8_t cliff_counter = 0;
	int32_t cliff_signal;

	roombaDrive(DEFAULT_VELOCITY, 1);
	
	roombaQuerySensor(PACKET_ANGLE);

	while(1){
		cliff_signal = roombaQuerySensor(PACKET_CLIFF_FRONT_LEFT_SIGNAL);
		if(cliff_signal >= 1200){
			do{
				my_msleep(50);
				cliff_signal = roombaQuerySensor(PACKET_CLIFF_FRONT_LEFT_SIGNAL);
			}while(cliff_signal >= 1200);

			cliff_counter++;
			if(cliff_counter == 1) {
				//reset angle value
				roombaQuerySensor(PACKET_ANGLE);
			}
			if(cliff_counter == 5){
				break;
			}
		}
		my_msleep(50);
	}
	roombaStop();
	int32_t angle = roombaQuerySensor(PACKET_ANGLE);
	roombadata.angle_360_degrees = angle;

	roombaPlaySongDone();
}	

void roombaCalibrateDistance(){
	roombaResetTrips();
	
	my_msleep(300);

	uint8_t cliff_counter = 0;
	int32_t cliff_signal;

	roombaDrive(DEFAULT_VELOCITY, (int16_t) 0);
	while(1){
		cliff_signal = roombaQuerySensor(PACKET_CLIFF_FRONT_LEFT_SIGNAL);
		if(cliff_signal >= 1200){
			do{
				my_msleep(50);
				cliff_signal = roombaQuerySensor(PACKET_CLIFF_FRONT_LEFT_SIGNAL);
			}while(cliff_signal >= 1200);

			cliff_counter++;
			if(cliff_counter == 1) {
				//reset distance value
				roombaQuerySensor(PACKET_DISTANCE);
			}
			if(cliff_counter == 2){
				break;
			}
		}
		my_msleep(50);
	}

	roombaStop();
	int32_t distance = roombaQuerySensor(PACKET_DISTANCE);
	roombadata.distance_1_meter = mymathAbs(distance);

	roombaPlaySongDone();
}



int32_t roombaQuerySensor(packet query_packet){
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


	if(query_packet.id == PACKET_DISTANCE.id){
		int32_t distance_value = mymathAbs(result);
		distance_value = roombaAsCalibratedDistance(distance_value);
		roombadata.driven_distance += distance_value;
		roombadata.trip_distance += distance_value;
	}

	if(query_packet.id == PACKET_ANGLE.id){
		roombadata.trip_angle += roombaAsCalibratedAngle(result);
	}

	return result;
}



int32_t roombaAsCalibratedAngle(int32_t angle_raw){
	return (int32_t) (angle_raw * (360.0f / roombadata.angle_360_degrees));
}

int32_t roombaAsCalibratedDistance(int32_t distance_raw){
	return (int32_t) (distance_raw * (1000.0f/roombadata.distance_1_meter));
}


void roombaSeekdock(){
	uart_write_byte(CMD_DOCK);
}

void roombaResetTrips() {
	//consume sensor values
	roombaQuerySensor(PACKET_ANGLE);
	roombaQuerySensor(PACKET_DISTANCE);
	roombadata.trip_angle = 0;
	roombadata.trip_distance = 0;
}



void roombaWriteSevensegDigits () {
        uart_write_byte(CMD_7SEG_ASCII);
	int i;
	for(i=3; i >= 0; i--){
        	uart_write_byte(roomba_sevenseg_digits[i]);
	}
}
 
void roombaSetWeekdayLed (uint8_t led_mask){
        uart_write_byte(CMD_SCHEDULE_LED);
        uart_write_byte(led_mask);
        uart_write_byte((uint8_t)0);
	uart_write_byte((uint8_t)0);
}

void roombaSetLed(uint8_t led_mask, uint8_t color, uint8_t intensity) {
        uart_write_byte(CMD_LEDS);
        uart_write_byte(led_mask);
        uart_write_byte(color);
        uart_write_byte(intensity);
}
 

void roombaDrive(int16_t velocity, int16_t radius) {
	uart_write_byte(CMD_DRIVE);
	uart_write_byte((uint8_t) (velocity >> 8));
	uart_write_byte((uint8_t) (velocity & 0xFF));
	uart_write_byte((uint8_t) (radius >> 8));
	uart_write_byte((uint8_t) (radius & 0xFF));
	roombadata.is_moving = 1;
	roombadata.current_velocity = velocity;
}



void roombaStop(){
	roombaDrive(0, 0);
	roombadata.is_moving = 0;
	roombadata.current_velocity = 0;
}

uint8_t roombaCheckButton(){
	uart_write_byte(CMD_BUTTONS_STATE);
	uart_write_byte(BTN_PACKET_ID);
	return uart_read_byte();
}


int32_t roombaCalculateTimeToDrive (int32_t distance, int32_t velocity){
	return ((int32_t)(distance/velocity))*1000;
}

void roombaDriveABitBackward(int32_t backward_distance){
	int32_t trip_distance_backup = roombadata.trip_distance;
	int32_t trip_angle_backup = roombadata.trip_angle;
	roombaResetTrips();
	roombaDrive(-DEFAULT_VELOCITY/2, (int16_t) 0);
	while(roombadata.trip_distance <= backward_distance){
		my_msleep(200);
		roombaQuerySensor(PACKET_DISTANCE);
	}
	roombaStop();
	roombadata.trip_distance = trip_distance_backup;
	roombadata.trip_angle = trip_angle_backup;
}

void roombaOnCollisionDetected(int32_t bumper_state, int32_t light_bumper_state){

	roombaStop();
	collisiondata.light_bumper_state = light_bumper_state;
	collisiondata.bumper_state = bumper_state;
	collisiondata.trip_distance_at_collision = roombadata.trip_distance;
	collisiondata.trip_angle_at_collision = roombadata.trip_angle;
	collisiondata.program_tick_counter = 0;
	roombaResetTrips();
}

void roombaOnCollisionCleared(){

	collisiondata.angle_sum = 0;
	collisiondata.distance_sum = 0;
	collisiondata.light_bumper_state = 0;
	collisiondata.bumper_state = 0;
	collisiondata.program_tick_counter = 0;
	collisiondata.driven_trip_distance = 0;
	collisiondata.played_acustic_feedback = 0;

	//restore trip meters
	roombadata.trip_distance = collisiondata.trip_distance_at_collision + collisiondata.driven_trip_distance;
	roombadata.trip_angle = 0;
	
}

void roombaPlaySongTheme(){
	#ifndef _SONG_THEME
	#define _SONG_THEME
	uart_write_byte(0x8C);
	uart_write_byte(0x01);
	uart_write_byte(0x11);

	uart_write_byte(0x3C);
	uart_write_byte(0x10);
	uart_write_byte(0x35);
	uart_write_byte(0x20);
	uart_write_byte(0x39);
	uart_write_byte(0x10);
	uart_write_byte(0x3C);
	uart_write_byte(0x10);
	uart_write_byte(0x35);
	uart_write_byte(0x20);
	uart_write_byte(0x39);
	uart_write_byte(0x10);
	uart_write_byte(0x3C);
	uart_write_byte(0x0E);
	uart_write_byte(0x40);
	uart_write_byte(0x0E);
	uart_write_byte(0x3E);
	uart_write_byte(0x10);
	uart_write_byte(0x3B);
	uart_write_byte(0x10);
	uart_write_byte(0x39);
	uart_write_byte(0x0E);
	uart_write_byte(0x3B);
	uart_write_byte(0x0E);
	uart_write_byte(0x3C);
	uart_write_byte(0x10);
	uart_write_byte(0x35);
	uart_write_byte(0x10);
	uart_write_byte(0x34);
	uart_write_byte(0x0E);
	uart_write_byte(0x37);
	uart_write_byte(0x0E);
	uart_write_byte(0x35);
	uart_write_byte(0x40);
	#endif

	uart_write_byte(0x8D);
	uart_write_byte(0x01);
}

void roombaPlaySongCollision(){
	#ifndef _SONG_COLLISION
	#define _SONG_COLLISION
	uart_write_byte(0x8C);
	uart_write_byte(0x02);
	uart_write_byte(0x14);

	int i;
	for(i=0; i < 20; i++){
		uart_write_byte(0x67);
		uart_write_byte(0x10);
	}
	#endif

	uart_write_byte(0x8D);
	uart_write_byte(0x02);	
}

void roombaPlaySongDone(){
	//TODO use bool instead of defines
	#ifndef _SONG_DONE
	#define _SONG_DONE
	uart_write_byte(0x8C);
	uart_write_byte(0x03);
	uart_write_byte(0x06);


	uart_write_byte(0x56);
	uart_write_byte(0x20);
	uart_write_byte(0x59);
	uart_write_byte(0x10);
	uart_write_byte(0x5B);
	uart_write_byte(0x10);
	uart_write_byte(0x60);
	uart_write_byte(0x20);
	uart_write_byte(0x6B);
	uart_write_byte(0x08);
	uart_write_byte(0x6B);
	uart_write_byte(0x08);
	#endif

	uart_write_byte(0x8D);
	uart_write_byte(0x03);	
}

void roombaPlaySongBeep(){
	#ifndef _SONG_BEEP
	#define _SONG_BEEP
	uart_write_byte(0x8C);
	uart_write_byte(0x00);
	uart_write_byte(0x02);

	uart_write_byte(0x60);
	uart_write_byte(0x20);
	uart_write_byte(0x60);
	uart_write_byte(0x20);
	#endif

	uart_write_byte(0x8D);
	uart_write_byte(0x00);	
}





/*********************************************************** Local functions */
