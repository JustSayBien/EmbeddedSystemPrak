#include "program.h"
#include "mymath.h"

/** stores the queried button state */
volatile uint8_t button_state;

/** stores the queried bumper state */
volatile int32_t bumper_state;

/** bool to switch between displaying trip or whole distance */
uint8_t display_whole_distance = 0;

/** planned distance  */
int32_t planned_distance = 0;

/** planned angle */
int32_t planned_angle = 0;

/** driven angle */
int32_t driven_angle = 0;

/** current program state */
enum programstate program_state;


/** current drive state */
enum drivestate drive_state;


/** current calibrate state */
enum calibratestate calibrate_state;






void program_run() {

	//init sevenseg
	roombaWriteSevensegDigits();	

	//set to init programm state
	setProgramState(INIT);

	//main loop
	while(1){
		led_set_blue(ledb_vals[3]);

		//check button here, return value is needed for all states
		button_state = roombaCheckButton();

		//switch block according to state chart
		switch(program_state){
			case INIT:
				handleStateInit();
				break;
			case CALIBRATE:
				handleStateCalibrate();
				break;
			case DRIVE:
				handleStateDrive();
				break;
			case COLLISION:	
				handleStateCollision();
				break;
		}
		
		//global reset for all states
		if(button_state == BTN_CLEAN){
			roombaStop();
			int32_t distance_centimeter = display_whole_distance ? roombadata.driven_distance/10 : roombadata.trip_meter/10;
			if(intToAscii(distance_centimeter, roomba_sevenseg_digits) != 1){
				roombaSetWeekdayLed(distance_centimeter / 10000);	
			}
			else{
				roombaSetWeekdayLed(0);
			}
			roombaWriteSevensegDigits();
			setProgramState(INIT);
		}

		button_state = 0;
		my_msleep(150);	
	}
}



int intToAscii(int32_t value, int32_t out[]){
	out[3] = (value % 10000) / 1000 + ASCII_NUMBER_START;
	out[2] = (value % 1000) / 100 + ASCII_NUMBER_START;
	out[1] = (value % 100) / 10 + ASCII_NUMBER_START;
	out[0] = value % 10 + ASCII_NUMBER_START;
	// integer to large
	if(value >= 10000)
		return -1;
	else 
		return 1;
}

void setProgramState(enum programstate state){
	switch(state) {
		case INIT:
			roombaSetLed(LED_DOCK_GREEN, 0, 0);	
			break;
		case CALIBRATE:
			roombaSetLed(LED_DIRT_DETECT_BLUE, 0, 0);
			break;
		case DRIVE:
			roombaSetLed(LED_SPOT_GREEN, 0, 0);
			break;
		case COLLISION:
			roombaSetLed(LED_CHECK_ROBOT_RED, 0, 0);
			break;
	}
	program_state = state;
}

void handleStateInit(){
	//display trip or whole distance
	if(button_state == BTN_SPOT || button_state == BTN_DOCK){
		display_whole_distance = !display_whole_distance;
		int32_t distance_centimeter = display_whole_distance ? roombadata.driven_distance/10 : roombadata.trip_meter/10;
		if(intToAscii(distance_centimeter, roomba_sevenseg_digits) != 1){
			roombaSetWeekdayLed(distance_centimeter / 10000);	
		}
		else{
			roombaSetWeekdayLed(0);
		}
		roombaWriteSevensegDigits();
	}
				

	//switch to drive mode
	if(button_state == BTN_DAY){
		roombadata.trip_meter = 0;
		drive_state = DRIVING;
		setProgramState(DRIVE);
	}
	
	//switch to calibration mode
	if(button_state == BTN_HOUR){
		calibrate_state = DISTANCE;
		roomba_sevenseg_digits[0] = 'D';
		roomba_sevenseg_digits[1] = 'L';
		roomba_sevenseg_digits[2] = 'A';
		roomba_sevenseg_digits[3] = 'C';
		roombaWriteSevensegDigits();
		setProgramState(CALIBRATE);
	}
}

void handleStateCalibrate(){

	switch(calibrate_state){

	case DISTANCE:
		// use Day Button to start calibration
		if(button_state == BTN_DAY){
			roombaCalibrateDistance();
		}
		break;

	case ANGLE:
		// use Day Button to start calibration
		if(button_state == BTN_DAY){
			roombaCalibrateAngle();
		}
		break;
	
	}

	//switch between distance and angle calibration
	if(button_state == BTN_SPOT || button_state == BTN_DOCK){
		calibrate_state = calibrate_state == DISTANCE ? ANGLE : DISTANCE;
		if(calibrate_state == DISTANCE){
			roomba_sevenseg_digits[0] = 'D';
			roomba_sevenseg_digits[1] = 'L';
			roomba_sevenseg_digits[2] = 'A';
			roomba_sevenseg_digits[3] = 'C';
		}
		else{
			roomba_sevenseg_digits[0] = 'A';
			roomba_sevenseg_digits[1] = 'L';
			roomba_sevenseg_digits[2] = 'A';
			roomba_sevenseg_digits[3] = 'C';
		}
		roombaWriteSevensegDigits();
	}
}


void handleStateDrive(){


	bumper_state = roombaQuerySensor(PACKET_BUMPS_WHEELDROPS);
	// bumper and wheeldrop collision detected ?
	if(bumper_state != 0){
		roombaStop();
		setProgramState(COLLISION);
		//return immediately
		return;
	}


	roombaQuerySensor(PACKET_DISTANCE);
	int32_t angle = roombaQuerySensor(PACKET_ANGLE);
	driven_angle += angle;
	
	switch(drive_state){

		case DRIVING:

			//start driving
			if(!roombadata.is_moving){
				roombaDrive(DEFAULT_VELOCITY, (int16_t) 0);
			}

			else{
				int32_t light_bumper = roombaQuerySensor(PACKET_LIGHT_BUMPER);
				if(light_bumper != 0){
					if(light_bumper & 0x7 && light_bumper & 0x38){
						planned_angle = -roombadata.angle_360_degrees/2;
						drive_state = TURNING;
						roombaStop();
					}
					else if(light_bumper & 0x7){
						planned_angle = -roombadata.angle_360_degrees/12;
						drive_state = TURNING;
						roombaStop();
					}
					else{
						planned_angle = roombadata.angle_360_degrees/12;
						drive_state = TURNING;
						roombaStop();
					}
				}					
				
			}

			break;
		case TURNING:

			//start turning
			if(!roombadata.is_moving){
				int16_t direction = planned_angle < 0 ? -1 : 1;
				roombaDrive(DEFAULT_VELOCITY, direction);
			}
			else{
				//finished turning ?
				if((planned_angle < 0 && driven_angle <= planned_angle) || (planned_angle > 0 && driven_angle >= planned_angle)){
					driven_angle = 0;
					planned_angle = 0;
					roombaStop();
					drive_state = DRIVING;	
				}						
				
			}

			break;
		}
				
		
		// print distance
		int32_t distance_centimeter = display_whole_distance ? roombadata.driven_distance/10 : roombadata.trip_meter/10;
		if(intToAscii(distance_centimeter, roomba_sevenseg_digits) != 1){
			roombaSetWeekdayLed(distance_centimeter / 10000);	
		}
		else{
			roombaSetWeekdayLed(0);
		}
		roombaWriteSevensegDigits();
}

void handleStateCollision(){

	int16_t radius;
	int32_t planned_angle = 0;
	//right bumper
	if(bumper_state == 1) {
		radius = 1;
		planned_angle = roombadata.angle_360_degrees/4;
	}
	//left bumper
	else if(bumper_state == 2){	
		radius = -1;
		planned_angle = roombadata.angle_360_degrees/4;
		
	}
	//both bumpers
	else if(bumper_state == 3){
		//TODO random -1 and 1
		radius = 1;		
		planned_angle = roombadata.angle_360_degrees/3;
	}
	//any wheeldrop 
	else{
		//use Day button to return to drive mode
		if(button_state == BTN_DAY){
			setProgramState(DRIVE);
		}
		return;
	}


	//consume sensor values
	roombaQuerySensor(PACKET_DISTANCE);
	roombaQuerySensor(PACKET_ANGLE);

	int32_t driven_distance = 0;
	int32_t driven_angle = 0;

	//drive a bit backward
	roombaDrive(-DEFAULT_VELOCITY/2, (int16_t) 0);
	while(driven_distance <= 10){
		my_msleep(200);
		int32_t distance = roombaQuerySensor(PACKET_DISTANCE);
		driven_distance += distance;
	}
	roombaStop();


	//turn away
	roombaDrive(DEFAULT_VELOCITY/2, radius);
	while(driven_angle <= planned_angle){
		my_msleep(200);
		int32_t angle = roombaQuerySensor(PACKET_ANGLE);
		driven_angle += angle < 0 ? angle * -1 : angle;

	}
	roombaStop();

	drive_state = DRIVING;
	setProgramState(DRIVE);
}

