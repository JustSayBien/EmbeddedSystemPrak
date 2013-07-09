#include "program.h"
#include "mymath.h"
#include "workbench.h"
#include "ir_remote.h"
#include "ir_base.h"

/** stores the queried button state */
volatile uint8_t button_state;

volatile int32_t ir_action;


/** bool to switch between displaying trip or whole distance */
uint8_t display_whole_distance = 0;

millis_t global_clock = 1;

uint8_t base_config_state = BASE_SELECT;

/** current program state */
enum programstate program_state;

/** current drive state */
enum drivestate drive_state = LEAVE_DOCK;
//enum drivestate drive_state = DOCKED;

enum angleapproachstate angle_approach_state = DRIVE_ANGLE;

enum collisionstate collision_state;

/** current calibrate state */
enum calibratestate calibrate_state;

/** current nextbase state */
enum nextbasestate nextbase_state = NEXTBASE_NUM;


int32_t roomba_default_evasive_angle = 0;
int32_t roomba_current_evasive_angle = 0;







void program_run() {

	//init sevenseg
	write_sevenseg_digits();	

	//set to init programm state
	setProgramState(CALIBRATE);


	workbenches_init();


	//test
	int16_t angle = get_angle(1,2);
	//int16_t distance_cm = get_distance(1,2);
	intToAscii(angle, roomba_sevenseg_digits);
	write_sevenseg_digits();

	//main loop
	while(1){
		//led_set_blue(ledb_vals[3]);

		//check button here, return value is needed for all states
		//button_state = check_button();
		button_state = 0;
		
		ir_action = getIRAction();

		//switch block according to state chart
		switch(program_state){
			/*case INIT:
				program_state = handleStateInit();
				break;*/
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
		//if(button_state == BTN_CLEAN){
		if(ir_action == ROOMBA_REMOTE_BACK) {
			stop();
			int32_t distance_centimeter = display_whole_distance ? roombadata.driven_distance/10 : roombadata.trip_distance/10;
			if(intToAscii(distance_centimeter, roomba_sevenseg_digits) != 1){
				setWeekdayLed(distance_centimeter / 10000);	
			}
			else{
				setWeekdayLed(0);
			}
			write_sevenseg_digits();
			setProgramState(CALIBRATE);
		}
		else{
			setProgramState(program_state);
		}

		button_state = 0;
		ir_action = 0;
		my_msleep(150);
		global_clock += 150;
	}
}



int intToAscii(int32_t value, int32_t out[]){

	/*if(value < 0){
		setWeekdayLed(1);
		value *= -1;
	}
	else{
		setWeekdayLed(0);
	}*/

	out[3] = (value % 10000) / 1000 + ASCII_NUMBER_START;
	out[2] = (value % 1000) / 100 + ASCII_NUMBER_START;
	out[1] = (value % 100) / 10 + ASCII_NUMBER_START;
	out[0] = value % 10 + ASCII_NUMBER_START;
	// integer too large
	if(value >= 10000)
		return -1;
	else 
		return 1;
}

void setProgramState(enum programstate state){
	switch(state) {
		/*case INIT:
			setLed(LED_DOCK_GREEN, 0, 0);	
			break;*/
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

/*enum programstate handleStateInit(){
	//intToAscii(ir_action, roomba_sevenseg_digits);
	//write_sevenseg_digits();
	
	//display trip or whole distance
	if(ir_action == ROOMBA_REMOTE_CROSS_UP || ir_action == ROOMBA_REMOTE_CROSS_DOWN){
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
	if(ir_action == ROOMBA_REMOTE_CROSS_OK){
		roombadata.trip_distance = 0;
		
		roomba_sevenseg_digits[3] = 'D';
		roomba_sevenseg_digits[2] = 'R';
		roomba_sevenseg_digits[1] = 'I';
		roomba_sevenseg_digits[0] = 'V';
		
		write_sevenseg_digits();
		
		my_msleep(1000);
		
		return DRIVE;
	}
	
	//switch to calibration mode
	else if(ir_action == ROOMBA_REMOTE_PAUSE){
		calibrate_state = CALIBRATE_DRIVE;
		/*roomba_sevenseg_digits[0] = 'D';
		roomba_sevenseg_digits[1] = 'L';
		roomba_sevenseg_digits[2] = 'A';
		roomba_sevenseg_digits[3] = 'C';
		write_sevenseg_digits();
		
		return CALIBRATE;
	}
	else{
		return INIT;
	}
}*/

enum programstate handleStateCalibrate(){		
	switch(calibrate_state) {
		case CALIBRATE_DRIVE:
			base_config_state = BASE_SELECT;
			roomba_sevenseg_digits[3] = 'D';
			roomba_sevenseg_digits[2] = 'R';
			roomba_sevenseg_digits[1] = 'I';
			roomba_sevenseg_digits[0] = 'V';
			write_sevenseg_digits();
			
			if (ir_action == ROOMBA_REMOTE_CROSS_OK) {
				my_msleep(1000);		
				return DRIVE;
			}
			switch (ir_action) {
				case ROOMBA_REMOTE_CROSS_OK:
					return DRIVE;
					break;
				case ROOMBA_REMOTE_CROSS_UP:
					calibrate_state = CALIBRATE_BASE;
					break;
				case ROOMBA_REMOTE_CROSS_DOWN:
					calibrate_state = CALIBRATE_DISTANCE;
					break;
			}
			break;
		/*case CALIBRATE_DRIVE:
			break;*/
			
		case CALIBRATE_DISTANCE:
			base_config_state = BASE_SELECT;
			roomba_sevenseg_digits[3] = 'C';
			roomba_sevenseg_digits[2] = 'A';
			roomba_sevenseg_digits[1] = 'L';
			roomba_sevenseg_digits[0] = 'D';
			write_sevenseg_digits();
			
			switch (ir_action) {
				case ROOMBA_REMOTE_CROSS_OK:
					roomba_calibrate_distance();
					break;
				case ROOMBA_REMOTE_CROSS_UP:
					calibrate_state = CALIBRATE_ANGLE;
					break;
				case ROOMBA_REMOTE_CROSS_DOWN:
					calibrate_state = CALIBRATE_BASE;
					break;
			}
			break;
			
		case CALIBRATE_ANGLE:
			base_config_state = BASE_SELECT;
			roomba_sevenseg_digits[3] = 'C';
			roomba_sevenseg_digits[2] = 'A';
			roomba_sevenseg_digits[1] = 'L';
			roomba_sevenseg_digits[0] = 'A';
			write_sevenseg_digits();
			
			switch (ir_action) {
				case ROOMBA_REMOTE_CROSS_OK:
					roomba_calibrate_angle();
					roomba_current_evasive_angle = roomba_default_evasive_angle = (roombadata.angle_360_degrees / 12);     //  30 degrees
					break;
				case ROOMBA_REMOTE_CROSS_UP:
					calibrate_state = CALIBRATE_BASE;
					break;
				case ROOMBA_REMOTE_CROSS_DOWN:
					calibrate_state = CALIBRATE_DISTANCE;
					break;
			}
			break;
			
		case CALIBRATE_BASE:
			if (calibrate_state == BASE_SELECT) {
				switch (ir_action) {
					case ROOMBA_REMOTE_CROSS_UP:
						calibrate_state = CALIBRATE_BASE;
						break;
					case ROOMBA_REMOTE_CROSS_DOWN:
						calibrate_state = CALIBRATE_DISTANCE;
						break;
				}
			}
			
			// base calibration starts when number key is pressed
			handleSubstateBaseSetup();
			break;
	
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
	
	switch(collision_state){
		case COLLISION_TURN:
			if(!roombadata.is_moving){

				//check physical bumpers and wheeldrops before light bumps to ensure that they have priority
				if(collisiondata.bumper_state != 0){
					//both bumpers
					if(collisiondata.bumper_state == 0x3){
						//TODO random - and +
						collisiondata.planned_angle = -90;
					}
					//right bumper
					else if(collisiondata.bumper_state == 0x1) {
						collisiondata.planned_angle = 45;
					}
					//left bumper
					else if(collisiondata.bumper_state == 0x2){	
						collisiondata.planned_angle = -45;
					}
					//any wheeldrop 
					else{
						//use Day button to return to drive mode
						if(button_state == BTN_DAY){
							//TODO restore trip meters from collisiondata
							on_collision_cleared();
							return DRIVE;
						}
						else{
							return COLLISION;
						}
					}
					drive_a_bit_backward();
				}

				else if(collisiondata.light_bumper_state != 0){
					//both bumpers
					if(collisiondata.light_bumper_state & 0x7 && collisiondata.light_bumper_state & 0x38){
						collisiondata.planned_angle = -90;
					}
					else if(collisiondata.light_bumper_state & 0x7){
						collisiondata.planned_angle = -45;
					}
					else{
						collisiondata.planned_angle = 45;
					}
				}
				// no collision, we can balance angle_sum
				else{
					//angle_sum is already balanced
					if(collisiondata.angle_sum == 0){
						//TODO check and balance distance sum
						on_collision_cleared();
						return DRIVE;
					}
					else{
						collisiondata.planned_angle = collisiondata.angle_sum * -1;
					}
				}


				collisiondata.bumper_state = 0;
				collisiondata.light_bumper_state = 0;
				int16_t direction = collisiondata.planned_angle < 0 ? -1 : 1;
				drive(DEFAULT_VELOCITY, direction);

			}
			else{
				query_sensor(PACKET_ANGLE);
				// reached planned angle?
				if((collisiondata.planned_angle < 0 && roombadata.trip_angle <= collisiondata.planned_angle) ||
 					(collisiondata.planned_angle >= 0 && roombadata.trip_angle >= collisiondata.planned_angle)){

					collisiondata.angle_sum += collisiondata.planned_angle;
					collisiondata.planned_angle = 0;
					reset_trips();
					stop();
				
					int32_t bumper_state = query_sensor(PACKET_BUMPS_WHEELDROPS);
					int32_t light_bumper_state = query_sensor(PACKET_LIGHT_BUMPER);
					if(bumper_state != 0 || light_bumper_state != 0){
						collisiondata.bumper_state = bumper_state;
						collisiondata.light_bumper_state = light_bumper_state;
					}
					else{
						collisiondata.planned_distance = 300;
						collision_state = COLLISION_DRIVE;
					}
				}
			}
			break;
		case COLLISION_DRIVE:

			if(!roombadata.is_moving){
				drive(DEFAULT_VELOCITY, 0);
			}
			else{

				query_sensor(PACKET_DISTANCE);
				//while driving planned distance, check if new collisions are detected
				int32_t bumper_state = query_sensor(PACKET_BUMPS_WHEELDROPS);
				int32_t light_bumper_state = query_sensor(PACKET_LIGHT_BUMPER);
				if(bumper_state != 0 || light_bumper_state != 0){
					stop();
					reset_trips();
					//TODO calculate distance value based on trigonometrie
					collisiondata.distance_sum += roombadata.trip_distance;
					collisiondata.bumper_state = bumper_state;
					collisiondata.light_bumper_state = light_bumper_state;
					collision_state = COLLISION_TURN;
				}
				else{
					if(roombadata.trip_distance >= collisiondata.planned_distance){
						//TODO distance_sum should be distance value which roomba is away from his course(trigonometrie)
						collisiondata.distance_sum += collisiondata.planned_distance;
						collisiondata.planned_distance = 0;
						stop();
						reset_trips();
						collision_state = COLLISION_TURN;
					}
				}
			}
			break;

	}

	return COLLISION;
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

bool_t docked_in_menu = false;

enum programstate handleStateDocked() {
	uint8_t old_current_base_id = roombadata.current_base_id;
	roombadata.current_base_id = check_discrete_base_id();
	if (old_current_base_id != 0 && roombadata.current_base_id != roombadata.destination_base_id) {
		drive_state = LEAVE_DOCK;
		return DRIVE;
	} else {
		roombadata.destination_base_id = 0;
	}
	
	switch (nextbase_state) {
		case NEXTBASE_DRIVE:
			roomba_sevenseg_digits[3] = 'D';
			roomba_sevenseg_digits[2] = 'R';
			roomba_sevenseg_digits[1] = 'I';
			roomba_sevenseg_digits[0] = 'V';
			if (ir_action == ROOMBA_REMOTE_CROSS_OK) {
				docked_in_menu = false;
			}
			break;
			
		case NEXTBASE_NUM:
			if (!docked_in_menu) {
				roomba_sevenseg_digits[3] = 'B';
				roomba_sevenseg_digits[2] = 'N';
				roomba_sevenseg_digits[1] = 'U';
				roomba_sevenseg_digits[0] = 'M';
				switch (ir_action) {
					case ROOMBA_REMOTE_CROSS_OK:
						docked_in_menu = true;
						break;
					case ROOMBA_REMOTE_CROSS_UP:
						nextbase_state = NEXTBASE_DRIVE;
						break;
					case ROOMBA_REMOTE_CROSS_DOWN:
						nextbase_state = NEXTBASE_APPROACH;
						break;
					default:
						break;
				}
			} else {
				switch (ir_action) {
					case ROOMBA_REMOTE_BACK:
						docked_in_menu = false;
						break;
					case ROOMBA_REMOTE_NUM_1:
						roombadata.destination_base_id = 1;
						break;
					case ROOMBA_REMOTE_NUM_2:
						roombadata.destination_base_id = 2;
						break;
					case ROOMBA_REMOTE_NUM_3:
						roombadata.destination_base_id = 3;
						break;
					case ROOMBA_REMOTE_NUM_4:
						roombadata.destination_base_id = 4;
						break;
					case ROOMBA_REMOTE_NUM_5:
						roombadata.destination_base_id = 5;
						break;
					case ROOMBA_REMOTE_NUM_6:
						roombadata.destination_base_id = 6;
						break;
					default:
						break;
				}
				if (roombadata.destination_base_id != 0) {
					roomba_sevenseg_digits[3] = 'B';
					roomba_sevenseg_digits[2] = 'A';
					roomba_sevenseg_digits[1] = ' ';
					roomba_sevenseg_digits[0] = (roombadata.destination_base_id + ASCII_NUMBER_START);
				}
			}
			
			break;
			
		case NEXTBASE_APPROACH:
			if (!docked_in_menu) {
				roomba_sevenseg_digits[3] = 'A';
				roomba_sevenseg_digits[2] = 'P';
				roomba_sevenseg_digits[1] = 'P';
				roomba_sevenseg_digits[0] = 'R';
				switch (ir_action) {
					case ROOMBA_REMOTE_CROSS_OK:
						docked_in_menu = true;
						break;
					case ROOMBA_REMOTE_CROSS_UP:
						nextbase_state = NEXTBASE_NUM;
						break;
					case ROOMBA_REMOTE_CROSS_DOWN:
						nextbase_state = NEXTBASE_DRIVE;
						break;
					default:
						break;
				}
			} else {
				if (ir_action == ROOMBA_REMOTE_BACK || ir_action == ROOMBA_REMOTE_CROSS_OK) {
					docked_in_menu = false;
				}
				switch (drive_state) {
					case ANGLE_APPROACH:
						roomba_sevenseg_digits[3] = 'A';
						roomba_sevenseg_digits[2] = 'N';
						roomba_sevenseg_digits[1] = 'G';
						roomba_sevenseg_digits[0] = 'L';
						if (ir_action == ROOMBA_REMOTE_CROSS_UP)
							drive_state = LINE_APPROACH;
						else if (ir_action == ROOMBA_REMOTE_CROSS_DOWN)
							drive_state = FENCE_APPROACH;
						break;
					case LINE_APPROACH:
						roomba_sevenseg_digits[3] = 'L';
						roomba_sevenseg_digits[2] = 'I';
						roomba_sevenseg_digits[1] = 'N';
						roomba_sevenseg_digits[0] = 'E';
						if (ir_action == ROOMBA_REMOTE_CROSS_UP)
							drive_state = FENCE_APPROACH;
						else if (ir_action == ROOMBA_REMOTE_CROSS_DOWN)
							drive_state = ANGLE_APPROACH;
						break;
					case FENCE_APPROACH:
						roomba_sevenseg_digits[3] = 'F';
						roomba_sevenseg_digits[2] = 'E';
						roomba_sevenseg_digits[1] = 'N';
						roomba_sevenseg_digits[0] = 'C';
						if (ir_action == ROOMBA_REMOTE_CROSS_UP)
							drive_state = ANGLE_APPROACH;
						else if (ir_action == ROOMBA_REMOTE_CROSS_DOWN)
							drive_state = LINE_APPROACH;
						break;
					default:
						break;
				}
			}
			
			break;
			
		default:
			break;
	}
	
	write_sevenseg_digits();
	
	return DOCKED;
}




enum programstate handleSubStateLeaveDock(){

	drive_a_bit_backward();

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
			{
			//check if collisions are detected
			int32_t bumper_state = query_sensor(PACKET_BUMPS_WHEELDROPS);
			int32_t light_bumper_state = query_sensor(PACKET_LIGHT_BUMPER);
			if(bumper_state != 0 || light_bumper_state != 0){
				on_collision_detected(bumper_state, light_bumper_state);
				return COLLISION;
			}

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
	
	}

	return DRIVE;
}

enum programstate handleSubStateLineApproach(){

	//start driving if necessary
	if(!roombadata.is_moving){
		drive(DEFAULT_VELOCITY, 0);
	}
	

	//check if collisions are detected
	int32_t bumper_state = query_sensor(PACKET_BUMPS_WHEELDROPS);
	int32_t light_bumper_state = query_sensor(PACKET_LIGHT_BUMPER);
	if(bumper_state != 0 || light_bumper_state != 0){
		on_collision_detected(bumper_state, light_bumper_state);
		return COLLISION;
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
			
			drive(DEFAULT_VELOCITY, 0);
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

bool_t evasive = false;
direction previous_direction = LEFT;
direction current_direction = 0;
int32_t recog_counter = 0;
uint8_t turn_counter = 0;
bool_t lighthouse_has_turned = false;

enum programstate handleSubStateFenceApproach() {
	int16_t angle_to_drive = get_angle(roombadata.current_base_id, roombadata.destination_base_id);
	
	if (!lighthouse_has_turned) {
		//start turning if necessary
		if(!roombadata.is_moving){
			reset_trips();
			direction dir = angle_to_drive < 0 ? RIGHT : LEFT;
			drive(DEFAULT_VELOCITY, dir);
		}
		
		//check if defined angle is reached
		query_sensor(PACKET_ANGLE);
		if((angle_to_drive < 0 && roombadata.trip_angle <= angle_to_drive) || (angle_to_drive > 0 && roombadata.trip_angle >= angle_to_drive)){
			stop();
			reset_trips();
			lighthouse_has_turned = true;
		}	
	} else {
		//check if collisions are detected
		int32_t bumper_state = query_sensor(PACKET_BUMPS_WHEELDROPS);
		int32_t light_bumper_state = query_sensor(PACKET_LIGHT_BUMPER);
		if(bumper_state != 0 || light_bumper_state != 0){
			on_collision_detected(bumper_state, light_bumper_state);
			return COLLISION;
		}
		
		if (query_sensor(PACKET_VIRTUAL_WALL) == 1) {
			if (evasive == false) {
				recog_counter++;
				evasive = true;
				current_direction = (previous_direction == RIGHT ? LEFT : RIGHT);
				drive(DEFAULT_VELOCITY, current_direction);
			}
			query_sensor(PACKET_ANGLE);
			
			if (myAbs(roombadata.trip_angle) >= roomba_current_evasive_angle) {
				turn_counter++;
				current_direction = (current_direction == RIGHT ? LEFT : RIGHT);
				drive(DEFAULT_VELOCITY, current_direction);
				roombadata.trip_angle = 0;
				if (turn_counter < 3)
					roomba_current_evasive_angle = roomba_default_evasive_angle * (turn_counter+1);
				//roomba_current_evasive_angle = roomba_default_evasive_angle * 2;
			}
		} else {
			if (evasive == true) {
				evasive = false;
				previous_direction = current_direction;
				current_direction = 0;
				turn_counter = 0;
				drive(DEFAULT_VELOCITY, 0);
				roombadata.trip_angle = 0;
				roomba_current_evasive_angle = roomba_default_evasive_angle;
			}
		}
	}
	return DRIVE;
}

