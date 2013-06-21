#include "program.h"
#include "mymath.h"
#include "workbench.h"

/** stores the queried button state */
volatile uint8_t button_state;

/** stores the queried bumper state */
volatile int32_t bumper_state;
volatile int32_t light_bumper_state;

/** bool to switch between displaying trip or whole distance */
uint8_t display_whole_distance = 0;

/** planned distance  */
int32_t planned_distance = 0;

/** planned angle */
int32_t planned_angle = 0;

/** current program state */
enum programstate program_state;

/** current drive state */
enum drivestate drive_state = LEAVE_DOCK;

enum angleapproachstate angle_approach_state = DRIVE_ANGLE;

/** current calibrate state */
enum calibratestate calibrate_state;







void program_run() {

	//init sevenseg
	write_sevenseg_digits();	

	//set to init programm state
	setProgramState(INIT);


	workbenches_init();


	//test
	int16_t angle = get_angle(1,2);
	int16_t distance_cm = get_distance(1,2);
	intToAscii(angle, roomba_sevenseg_digits);
	write_sevenseg_digits();
	//

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
			case CALIBRATE:
				handleStateCalibrate();
				break;
			case DRIVE:
				handleStateDrive();
				break;
			case COLLISION:	
				handleStateCollision();
				break;
			case SEEKDOCK:
				handleStateSeekdock();
				break;
			case DOCKED:
				handleStateDocked();
		}
		
		//global reset for all states
		if(button_state == BTN_CLEAN){
			stop();
			int32_t distance_centimeter = display_whole_distance ? roombadata.driven_distance/10 : roombadata.trip_distance/10;
			if(intToAscii(distance_centimeter, roomba_sevenseg_digits) != 1){
				setWeekdayLed(distance_centimeter / 10000);	
			}
			else{
				setWeekdayLed(0);
			}
			write_sevenseg_digits();
			setProgramState(INIT);
		}

		button_state = 0;
		my_msleep(150);	
	}
}



int intToAscii(int32_t value, int32_t out[]){

	if(value < 0){
		setWeekdayLed(1);
		value *= -1;
	}
	else{
		setWeekdayLed(0);
	}

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
			setLed(LED_DOCK_GREEN, 0, 0);	
			break;
		case CALIBRATE:
			setLed(LED_DIRT_DETECT_BLUE, 0, 0);
			break;
		case DRIVE:
			setLed(LED_SPOT_GREEN, 0, 0);
			break;
		case COLLISION:
			setLed(LED_CHECK_ROBOT_RED, 0, 0);
			break;
		case SEEKDOCK:
			//TODO use defines
			setLed(0, 255, 255);
			break;
		case DOCKED:
			setLed(0,100,100);
			break;
	}
	program_state = state;
}

void handleStateInit(){
	//display trip or whole distance
	if(button_state == BTN_SPOT || button_state == BTN_DOCK){
		display_whole_distance = !display_whole_distance;
		int32_t distance_centimeter = display_whole_distance ? roombadata.driven_distance/10 : roombadata.trip_distance/10;
		if(intToAscii(distance_centimeter, roomba_sevenseg_digits) != 1){
			setWeekdayLed(distance_centimeter / 10000);	
		}
		else{
			setWeekdayLed(0);
		}
		write_sevenseg_digits();
	}
				

	//switch to drive mode
	if(button_state == BTN_DAY){
		roombadata.trip_distance = 0;
		setProgramState(DRIVE);
	}
	
	//switch to calibration mode
	if(button_state == BTN_HOUR){
		calibrate_state = DISTANCE;
		roomba_sevenseg_digits[0] = 'D';
		roomba_sevenseg_digits[1] = 'L';
		roomba_sevenseg_digits[2] = 'A';
		roomba_sevenseg_digits[3] = 'C';
		write_sevenseg_digits();
		setProgramState(CALIBRATE);
	}
}

void handleStateCalibrate(){

	switch(calibrate_state){

	case DISTANCE:
		// use Day Button to start calibration
		if(button_state == BTN_DAY){
			roomba_calibrate_distance();
		}
		break;

	case ANGLE:
		// use Day Button to start calibration
		if(button_state == BTN_DAY){
			roomba_calibrate_angle();
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
		write_sevenseg_digits();
	}
}


void handleStateDrive(){

	//any collision detected?
	/*bumper_state = query_sensor(PACKET_BUMPS_WHEELDROPS);
	light_bumper_state = query_sensor(PACKET_LIGHT_BUMPER);
	if(bumper_state != 0 || light_bumper_state != 0){
		stop();
		setProgramState(COLLISION);
		//return immediately
		return;
	}*/


	switch(drive_state){
		case LEAVE_DOCK:
			handleSubStateLeaveDock();
			break;
		case ANGLE_APPROACH:
			handleSubStateAngleApproach();
			break;
		case LINE_APPROACH:
			handleSubStateLineApproach();
			break;
		case FENCE_APPROACH:
			handleSubStateFenceApproach();
			break;

	}
}

void handleStateCollision(){



	//old light bumper code....
	/*if(light_bumper_state != 0){
		if(light_bumper_state & 0x7 && light_bumper_state & 0x38){
			planned_angle = -roombadata.angle_360_degrees/2;
			drive_state = TURNING;
			stop();
		}
		else if(light_bumper_state & 0x7){
			planned_angle = -roombadata.angle_360_degrees/12;
			drive_state = TURNING;
			stop();
		}
		else{
			planned_angle = roombadata.angle_360_degrees/12;
			drive_state = TURNING;
			stop();
		}
	} */



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
	query_sensor(PACKET_DISTANCE);
	query_sensor(PACKET_ANGLE);

	int32_t driven_distance = 0;
	int32_t driven_angle = 0;

	//drive a bit backward
	drive(-DEFAULT_VELOCITY/2, (int16_t) 0);
	while(driven_distance <= 10){
		my_msleep(200);
		int32_t distance = query_sensor(PACKET_DISTANCE);
		driven_distance += distance;
	}
	stop();


	//turn away
	drive(DEFAULT_VELOCITY/2, radius);
	while(driven_angle <= planned_angle){
		my_msleep(200);
		int32_t angle = query_sensor(PACKET_ANGLE);
		driven_angle += angle < 0 ? angle * -1 : angle;

	}
	stop();

	
	setProgramState(DRIVE);
}


void handleStateSeekdock(){

	if(query_sensor(PACKET_CHARGING_SOURCES_AVAILABLE) == CHARGING_SOURCE_HOMEBASE){
		setProgramState(DOCKED);
		init_roomba();
	}

	

	/*switch(seekdock_state){
		case FORCEFIELD:

			break;
		case GREENBUOY:

			break;
		case REDBUOY:

			break;
		case GREENBUOY_AND_FORCEFIELD:

			break;
		case REDBUOY_AND_FORCEFIELD:

			break;
		case GREENBUOY_AND_REDBUOY:

			break;
		case GREENBUOY_REDBUOY_AND_FORCEFIELD:

			break;


	}*/

}

void handleStateDocked(){
	setWeekdayLed(1);
}




void handleSubStateLeaveDock(){

	reset_trips();
	//drive a bit backward
	drive(-DEFAULT_VELOCITY/2, (int16_t) 0);
	while(roombadata.trip_distance <= DIFFERENCE_TO_BASE){
		my_msleep(200);
		query_sensor(PACKET_DISTANCE);
	}
	stop();
	drive_state = ANGLE_APPROACH;
	reset_trips();
}




void handleSubStateAngleApproach(){

	int16_t angle_to_drive = get_angle(1,2);
	int16_t distance_to_drive = get_distance(1,2);

	switch(angle_approach_state){
		case DRIVE_ANGLE:
			
			if(!roombadata.is_moving){
				int16_t direction = angle_to_drive < 0 ? -1 : 1;
				drive(DEFAULT_VELOCITY, direction);
			}
			

			query_sensor(PACKET_ANGLE);
			if((angle_to_drive < 0 && roombadata.trip_angle <= angle_to_drive) || (angle_to_drive > 0 && roombadata.trip_angle >= angle_to_drive)){
				stop();
				reset_trips();
				angle_approach_state = DRIVE_DISTANCE;
			}
				
			break;
		case DRIVE_DISTANCE:
			if(!roombadata.is_moving){
				drive(DEFAULT_VELOCITY, 0);
			}
			
			query_sensor(PACKET_DISTANCE);
			int32_t infrared_value = 0;
			
			// 1 meter before whole distance is reached we check infrared sensors
			if(roombadata.trip_distance >= distance_to_drive - SEEKDOCK_TRIGGER_DISTANCE){
				infrared_value = query_sensor(PACKET_INFRARED_CHARACTER_LEFT);
				infrared_value = infrared_value <= 160 ? query_sensor(PACKET_INFRARED_CHARACTER_RIGHT) : infrared_value;
				infrared_value = infrared_value <= 160 ? query_sensor(PACKET_INFRARED_CHARACTER_OMNI) : infrared_value;
			}

			// found infrared sensor or reached whole distance
			if(infrared_value > 160 || roombadata.trip_distance >= distance_to_drive){
				stop();
				reset_trips();
				setProgramState(SEEKDOCK);
				seekdock();
			}
			

			break;
	
	}



}

void handleSubStateLineApproach(){
}

void handleSubStateFenceApproach(){
}

