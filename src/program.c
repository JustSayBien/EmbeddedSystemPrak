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
	//int16_t distance_cm = get_distance(1,2);
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
				program_state = handleStateInit();
				break;
			case CALIBRATE:
				program_state = handleStateCalibrate();
				break;
			case DRIVE:
				program_state = handleStateDrive();
				break;
			case COLLISION:	
				program_state = handleStateCollision();
				break;
			case SEEKDOCK:
				program_state = handleStateSeekdock();
				break;
			case DOCKED:
				program_state = handleStateDocked();
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
		else{
			setProgramState(program_state);
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

enum programstate handleStateInit(){
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
		return DRIVE;
	}
	
	//switch to calibration mode
	else if(button_state == BTN_HOUR){
		calibrate_state = DISTANCE;
		roomba_sevenseg_digits[0] = 'D';
		roomba_sevenseg_digits[1] = 'L';
		roomba_sevenseg_digits[2] = 'A';
		roomba_sevenseg_digits[3] = 'C';
		write_sevenseg_digits();
		return CALIBRATE;
	}
	else{
		return INIT;
	}
}

enum programstate handleStateCalibrate(){

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

	return CALIBRATE;
}


enum programstate handleStateDrive(){

	switch(drive_state){
		case LEAVE_DOCK:
			return handleSubStateLeaveDock();
		case ANGLE_APPROACH:
			return handleSubStateAngleApproach();
		case LINE_APPROACH:
			return handleSubStateLineApproach();
		case FENCE_APPROACH:
			return handleSubStateFenceApproach();
	}
	return DRIVE;
	
}

enum programstate handleStateCollision(){



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
			return DRIVE;
		}
		else{
			return COLLISION;
		}
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

	
	return DRIVE;
}


enum programstate handleStateSeekdock(){
	//check if we reached the dock?
	if(query_sensor(PACKET_CHARGING_SOURCES_AVAILABLE) == CHARGING_SOURCE_HOMEBASE){
		init_roomba();
		return DOCKED;
	}
	else{
		return SEEKDOCK;
	}
}

enum programstate handleStateDocked(){
	return DOCKED;
}




enum programstate handleSubStateLeaveDock(){

	reset_trips();
	//drive a bit backward
	drive(-DEFAULT_VELOCITY/2, (int16_t) 0);
	while(roombadata.trip_distance <= DIFFERENCE_TO_BASE){
		my_msleep(200);
		query_sensor(PACKET_DISTANCE);
	}
	stop();
	drive_state = LINE_APPROACH;
	reset_trips();

	return DRIVE;
}




enum programstate handleSubStateAngleApproach(){

	int16_t angle_to_drive = get_angle(roombadata.current_base_id, roombadata.destination_base_id);
	int16_t distance_to_drive = get_distance(roombadata.current_base_id, roombadata.destination_base_id);

	switch(angle_approach_state){
		case DRIVE_ANGLE:
			
			//start turning if necessary
			if(!roombadata.is_moving){
				int16_t direction = angle_to_drive < 0 ? -1 : 1;
				drive(DEFAULT_VELOCITY, direction);
			}
			
			//check if defined angle is reached
			query_sensor(PACKET_ANGLE);
			if((angle_to_drive < 0 && roombadata.trip_angle <= angle_to_drive) || (angle_to_drive > 0 && roombadata.trip_angle >= angle_to_drive)){
				stop();
				reset_trips();
				angle_approach_state = DRIVE_DISTANCE;
			}
				
			break;
		case DRIVE_DISTANCE:

			//check if collisions are detected
			/*bumper_state = query_sensor(PACKET_BUMPS_WHEELDROPS);
			light_bumper_state = query_sensor(PACKET_LIGHT_BUMPER);
			if(drive_state != LEAVE_DOCK && (bumper_state != 0 || light_bumper_state != 0)){
				stop();
				return COLLISION;
			}*/

			//start driving if necessary
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
				seekdock();
				return SEEKDOCK;
			}
			break;
	
	}

	return DRIVE;
}

enum programstate handleSubStateLineApproach(){

	//start driving if necessary
	if(!roombadata.is_moving){
		drive(DEFAULT_VELOCITY, 0);
	}
	

	int32_t cliff_left_signal = query_sensor(PACKET_CLIFF_LEFT_SIGNAL);
	int32_t cliff_right_signal = query_sensor(PACKET_CLIFF_RIGHT_SIGNAL);
	if(cliff_left_signal >= 1200 && cliff_right_signal >= 1200){
		int32_t infrared_value = 0;
		infrared_value = query_sensor(PACKET_INFRARED_CHARACTER_LEFT);
		infrared_value = infrared_value <= 160 ? query_sensor(PACKET_INFRARED_CHARACTER_RIGHT) : infrared_value;
		infrared_value = infrared_value <= 160 ? query_sensor(PACKET_INFRARED_CHARACTER_OMNI) : infrared_value;
		if(infrared_value > 160){
			stop();
			reset_trips();
			seekdock();
			return SEEKDOCK;
		}
		else{
			
			drive(DEFAULT_VELOCITY, 1);
			return DRIVE;
		}
	}
	else{
		int32_t cliff_front_left_signal = query_sensor(PACKET_CLIFF_FRONT_LEFT_SIGNAL);
		int32_t cliff_front_right_signal = query_sensor(PACKET_CLIFF_FRONT_RIGHT_SIGNAL);

		// just drive on
		if(cliff_front_left_signal < 1200 && cliff_front_right_signal < 1200){
			drive(DEFAULT_VELOCITY, 0);
		}	
		// turn left
		else if(cliff_front_left_signal >= 1200 && cliff_front_right_signal < 1200){
			drive(DEFAULT_VELOCITY/2, 150);
		}
		//turn right
		else if(cliff_front_right_signal >= 1200 && cliff_front_left_signal < 1200){		
			drive(DEFAULT_VELOCITY/2, -150);
		}
		else{
			drive(DEFAULT_VELOCITY/2, 50);
		}

		return DRIVE;
	}



}

enum programstate handleSubStateFenceApproach(){
	return DRIVE;
}

