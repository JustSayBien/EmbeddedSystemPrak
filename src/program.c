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

enum lineapproachstate line_approach_state = LINE_TURN_FROM_BASE;

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

	roombadata.current_base_id = 1;
	roombadata.destination_base_id = 2;

	play_song_done();

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

			drive_state = LEAVE_DOCK;
			angle_approach_state = DRIVE_ANGLE;
			line_approach_state = LINE_TURN_FROM_BASE;
			on_collision_cleared();
			write_sevenseg_digits();
			setProgramState(CALIBRATE);
		}
		else{
			setProgramState(program_state);
		}

		button_state = 0;
		ir_action = 0;
		my_msleep(50);
		global_clock += 50;
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
		//case INIT:
			//setLed(LED_DOCK_GREEN, 0, 0);	
			//break;
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

		if(display_whole_distance){
			setWeekdayLed(1);
		}
		else{
			setWeekdayLed(0);
		}
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
}*/

enum programstate handleStateCalibrate(){		
	switch(calibrate_state) {
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
				case ROOMBA_REMOTE_CROSS_DOWN:
					calibrate_state = CALIBRATE_DONE;
					break;
				case ROOMBA_REMOTE_CROSS_UP:
					calibrate_state = CALIBRATE_ANGLE;
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
				case ROOMBA_REMOTE_CROSS_DOWN:
					calibrate_state = CALIBRATE_DISTANCE;
					break;
				case ROOMBA_REMOTE_CROSS_UP:
					calibrate_state = CALIBRATE_BASE;
					break;
			}
			break;
			
		case CALIBRATE_BASE:
			if (base_config_state == BASE_SELECT) {
				switch (ir_action) {
					case ROOMBA_REMOTE_CROSS_DOWN:
						calibrate_state = CALIBRATE_ANGLE;
						break;
					case ROOMBA_REMOTE_CROSS_UP:
						calibrate_state = CALIBRATE_DONE;
						break;
				}
			}
			
			// base calibration starts when number key is pressed
			handleSubstateBaseSetup();
			break;
			
		case CALIBRATE_DONE:
			base_config_state = BASE_SELECT;
			roomba_sevenseg_digits[3] = 'D';
			roomba_sevenseg_digits[2] = 'O';
			roomba_sevenseg_digits[1] = 'N';
			roomba_sevenseg_digits[0] = 'E';
			write_sevenseg_digits();
			
			switch (ir_action) {
				case ROOMBA_REMOTE_CROSS_OK:
					my_msleep(1000);
					return DOCKED;
				case ROOMBA_REMOTE_CROSS_DOWN:
					calibrate_state = CALIBRATE_BASE;
					break;
				case ROOMBA_REMOTE_CROSS_UP:
					calibrate_state = CALIBRATE_DISTANCE;
					break;
			}
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


void update_distance_sum(){
	int32_t angle_sum_abs = collisiondata.angle_sum < 0 ? -collisiondata.angle_sum : collisiondata.angle_sum;
	//check angle quadrant and if roomba drives left(+) or right(-) away
	int8_t distance_neg_sign = (angle_sum_abs / 180) % 2;
						
	//roomba isnt away from or parallel to his course
	if(angle_sum_abs % 180 == 0){
		if(angle_sum_abs % 360 == 0){
			collisiondata.driven_trip_distance += roombadata.trip_distance;
		}
		else{
			collisiondata.driven_trip_distance -= roombadata.trip_distance;
		}
	}
	else if(angle_sum_abs % 90 == 0){
		int32_t distance_diff = distance_neg_sign ? -roombadata.trip_distance : roombadata.trip_distance;
		if(collisiondata.angle_sum < 0){
			distance_diff = -distance_diff;
		}
		collisiondata.distance_sum += distance_diff;
	}
	else{
		angle_sum_abs %= 180;
		if(angle_sum_abs > 90){
			angle_sum_abs = 180 - angle_sum_abs;
		}
		else{
							
		}

		//TODO calc degToRad only one....

		int32_t trip_distance_diff = (int32_t) (my_cos(degToRad((float)angle_sum_abs)) * roombadata.trip_distance);
		collisiondata.driven_trip_distance += trip_distance_diff;

		int32_t distance_diff = (int32_t) (my_sin(degToRad((float)angle_sum_abs)) * roombadata.trip_distance);
		if(distance_diff < 0){
			distance_diff = -distance_diff;
		}
		distance_diff = distance_neg_sign ? -distance_diff : distance_diff;
		if(collisiondata.angle_sum < 0){
				distance_diff = -distance_diff;
		}
		collisiondata.distance_sum += distance_diff;
	}
}


int32_t lost_counter = 0;




enum programstate handleStateCollision(){


	collisiondata.program_tick_counter++;
	//after 30 seconds in collision mode stop driving and wait for user action
	if(collisiondata.program_tick_counter >= 200){
		stop();
		play_song_beep();
		if(button_state == BTN_DAY){
			on_collision_cleared();
			return DRIVE;
		}
		else{
			return COLLISION;
		}
	}

	switch(collision_state){
		case COLLISION_TURN:
			if(!roombadata.is_moving){

				//check physical bumpers and wheeldrops before light bumps to ensure that they have priority
				if(collisiondata.bumper_state != 0){
					//any wheeldrop 
					if(collisiondata.bumper_state >= 0x4){
						//use Day button to return to drive mode
						if(button_state == BTN_DAY){
							on_collision_cleared();
							return DRIVE;
						}
						else{
							return COLLISION;
						}
					}

					drive_a_bit_backward(DIFFERENCE_TO_BASE/4);
				}

				reset_trips();
					

				if(!collisiondata.played_acustic_feedback){
					play_song_collision();
					my_msleep(10000);
					//check if collision still exists
					collisiondata.bumper_state = query_sensor(PACKET_BUMPS_WHEELDROPS);
					collisiondata.light_bumper_state = query_sensor(PACKET_LIGHT_BUMPER);
					if(collisiondata.bumper_state == 0 && collisiondata.light_bumper_state == 0){
						on_collision_cleared();
						return DRIVE;
					}
					//any wheeldrop 
					else if(collisiondata.bumper_state >= 0x4){
						return COLLISION;
					}
					collisiondata.played_acustic_feedback = 1;
				}

				drive(DEFAULT_VELOCITY/2, 1);
			}
			else{

				query_sensor(PACKET_ANGLE);
				//didnt find any collision
				if(roombadata.trip_angle >= 360){
					stop();
					on_collision_cleared();
					return DRIVE;
				}


				int32_t light_signal = query_sensor(PACKET_LIGHT_BUMP_RIGHT_SIGNAL);
				if(light_signal >= 30 && light_signal <= 400){
					stop();
					collisiondata.angle_sum += roombadata.trip_angle;
					reset_trips();
					collision_state = COLLISION_DRIVE;
				}
			}
			break;
		case COLLISION_DRIVE:

			if(!roombadata.is_moving){
				drive(DEFAULT_VELOCITY/2, 0);
			}
			else{
				query_sensor(PACKET_ANGLE);
				query_sensor(PACKET_DISTANCE);

				//while driving check if new collisions are detected
				int32_t bumper_state = query_sensor(PACKET_BUMPS_WHEELDROPS);
				int32_t light_bumper_state = query_sensor(PACKET_LIGHT_BUMPER);
				if(bumper_state != 0 || light_bumper_state != 0){

						collisiondata.angle_sum += roombadata.trip_angle;
						update_distance_sum();
						stop();
						reset_trips();
						collisiondata.bumper_state = bumper_state;
						collisiondata.light_bumper_state = light_bumper_state;
						collision_state = COLLISION_TURN;
				}
				else{
					int32_t light_signal = query_sensor(PACKET_LIGHT_BUMP_RIGHT_SIGNAL);
					//lost right bumper
					if(light_signal <= 30){
						
						lost_counter++;
						if(lost_counter == 5){
							//TODO drive back to course
						}
						
						int32_t distance_value = roombadata.trip_distance + DIFFERENCE_TO_BASE/6;
						drive(DEFAULT_VELOCITY/2, (int16_t) 0);
						while(roombadata.trip_distance <= distance_value){
							my_msleep(50);
							query_sensor(PACKET_DISTANCE);
						}
						stop();

						collisiondata.angle_sum += roombadata.trip_angle;
						update_distance_sum();
						reset_trips();
					

						drive(DEFAULT_VELOCITY/2, -1);
					}
					else if(light_signal >= 400){
						collisiondata.angle_sum += roombadata.trip_angle;
						update_distance_sum();
						reset_trips();
						lost_counter = 0;
						drive(DEFAULT_VELOCITY/2, 1);
					}
					else{
						lost_counter = 0;
						if(roombadata.trip_distance > 0 && roombadata.trip_distance >= 50){
							collisiondata.angle_sum += roombadata.trip_angle;
							update_distance_sum();
							reset_trips();
						}

						drive(DEFAULT_VELOCITY/2, 0);
					}


					//after 3 seconds in collision mode begin to check if collision is avoided
					if(collisiondata.program_tick_counter >= 20){
						if(drive_state == LINE_APPROACH){
							int32_t cliff_front_left_signal = query_sensor(PACKET_CLIFF_FRONT_LEFT_SIGNAL);
							int32_t cliff_front_right_signal = query_sensor(PACKET_CLIFF_FRONT_RIGHT_SIGNAL);
							if(cliff_front_left_signal >= 1200 && cliff_front_right_signal >= 1200){
								stop();
								reset_trips();
								int32_t angle_to_turn = 45;
								drive(DEFAULT_VELOCITY/2, 1);
								if(angle_to_turn < 0) {
									angle_to_turn = -angle_to_turn;
								}
								while(roombadata.trip_angle <= angle_to_turn){
									my_msleep(50);
									query_sensor(PACKET_ANGLE);
								}
								stop();
								on_collision_cleared();
								return DRIVE;
							}
							else if(cliff_front_left_signal >= 1200){
								stop();
								reset_trips();
								int32_t angle_to_turn = 75;
								drive(DEFAULT_VELOCITY/2, 1);
		
								if(angle_to_turn < 0) {
									angle_to_turn = -angle_to_turn;
								}
								while(roombadata.trip_angle <= angle_to_turn){
									my_msleep(50);
									query_sensor(PACKET_ANGLE);
								}
								stop();
								on_collision_cleared();
								return DRIVE;
							}
							else if(cliff_front_right_signal >= 1200){
								stop();
								reset_trips();
								on_collision_cleared();
								return DRIVE;

							}
						}
					}

					if(collisiondata.driven_trip_distance >= 200){
						if(drive_state == ANGLE_APPROACH){
							if(collisiondata.distance_sum < 40 && collisiondata.distance_sum > -40){
								stop();
								reset_trips();
								int32_t angle_to_turn = 0;
								int32_t angle_sum_abs = collisiondata.angle_sum < 0 ? -collisiondata.angle_sum : collisiondata.angle_sum;
								angle_sum_abs %= 360;
								angle_to_turn = angle_sum_abs > 180 ? -(360 - angle_sum_abs) : angle_sum_abs;
								angle_to_turn = collisiondata.angle_sum < 0 ? angle_to_turn : -angle_to_turn;
					
								int8_t direction = angle_to_turn < 0 ? -1 : 1;
								drive(DEFAULT_VELOCITY/2, direction);

								while((angle_to_turn > 0 && roombadata.trip_angle < angle_to_turn) || (angle_to_turn < 0 && roombadata.trip_angle > angle_to_turn)){
									my_msleep(50);
									query_sensor(PACKET_ANGLE);
								}
								stop();
								reset_trips();
								
								on_collision_cleared();
		
								return DRIVE;
							}
						}
					}
					




				}
			}
			break;

	}

	intToAscii(collisiondata.driven_trip_distance, roomba_sevenseg_digits);
	write_sevenseg_digits();

	return COLLISION;
}


enum programstate handleStateSeekdock(){
	//check if we reached the dock?
	if(query_sensor(PACKET_CHARGING_SOURCES_AVAILABLE) == CHARGING_SOURCE_HOMEBASE){
		init_roomba();
		//consume sensor values
		query_sensor(PACKET_ANGLE);
		query_sensor(PACKET_DISTANCE);
		return DOCKED;
	}
	else{
		return SEEKDOCK;
	}
}

bool_t docked_in_menu = false;
bool_t lighthouse_has_turned = false;

enum programstate handleStateDocked() {
	// Reset boolean value for fence approach
	lighthouse_has_turned = false;
	
	uint8_t old_current_base_id = roombadata.current_base_id;
	roombadata.current_base_id = check_discrete_base_id();
	
	roomba_sevenseg_digits[3] = 'A';
	roomba_sevenseg_digits[2] = 'T';
	roomba_sevenseg_digits[1] = ' ';
	roomba_sevenseg_digits[0] = (roombadata.current_base_id + ASCII_NUMBER_START);
	write_sevenseg_digits();
	
	return DOCKED;
	
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

	reset_trips();
	drive_a_bit_backward(DIFFERENCE_TO_BASE);

	drive_state = display_whole_distance ? LINE_APPROACH : ANGLE_APPROACH;
	reset_trips();

	return DRIVE;
}




enum programstate handleSubStateAngleApproach(){

	volatile int16_t angle_to_drive = get_angle(roombadata.current_base_id, roombadata.destination_base_id);
	volatile int16_t distance_to_drive = get_distance(roombadata.current_base_id, roombadata.destination_base_id);

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
			
			// 2 meter before whole distance is reached we check infrared sensors
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

	switch(line_approach_state){
		case LINE_TURN_FROM_BASE:
			//start driving if necessary
			if(!roombadata.is_moving){
				drive(DEFAULT_VELOCITY, 1);
			}
			else{
				//update trip angle value
				query_sensor(PACKET_ANGLE);
				if(roombadata.trip_angle >= 180){
					stop();
					reset_trips();
					line_approach_state = LINE_DRIVE;
				}
			}
			break;
		case LINE_DRIVE:
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
			}
			break;
	}

	return DRIVE;
}

bool_t evasive = false;
direction previous_direction = LEFT;
direction current_direction = 0;
int32_t recog_counter = 0;
uint8_t turn_counter = 0;

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

