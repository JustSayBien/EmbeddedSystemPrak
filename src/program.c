#include "program.h"
#include "mymath.h"
#include "workbench.h"
#include "ir_remote.h"
#include "ir_base.h"

volatile int32_t ir_action;

millis_t global_clock = 1;

uint8_t base_config_state = BASE_SELECT;

/** current program state */
enum programstate program_state;

/** current drive state */
enum drivestate drive_state = LEAVE_DOCK;
/** current approach selection */
enum drivestate current_approach = ANGLE_APPROACH;

/** current angleapproach state */
enum angleapproachstate angle_approach_state = DRIVE_ANGLE;

/** current line approach state */
enum lineapproachstate line_approach_state = LINE_TURN_FROM_BASE;

/** current collision state */
enum collisionstate collision_state;

/** current calibrate state */
enum calibratestate calibrate_state;

/** current nextbase state */
enum nextbasestate nextbase_state = NEXTBASE_NUM;

/** current fenceapproach state */
enum fenceapproachstate fenceapproach_state = FENCE_ANGLE;

/** current fencedetect state */
enum fencedetectstate fencedetect_state = FENCE_FIRST;

bool_t docked_in_menu = false;
bool_t has_driven = false;

void programRun() {

	//init sevenseg
	roombaWriteSevensegDigits();

	//set to init programm state
	setProgramState(CALIBRATE);

	workbenchInit();

	my_msleep(2000);
	roombaPlaySongDone();

	//main loop
	while (1) {

		ir_action = getIRAction();

		//switch block according to state chart
		switch (program_state) {

		case CALIBRATE:
			program_state = handleStateCalibrate();
			break;
		case DRIVE:
			program_state = handleStateDrive();
			break;
		case COLLISION:
			//collision state uses smaller program tick rate
			if (global_clock % 150 == 1) {
				program_state = handleStateCollision();
			}
			break;
		case SEEKDOCK:
			program_state = handleStateSeekdock();
			break;
		case DOCKED:
			program_state = handleStateDocked();
		}

		//global reset for all states
		if (ir_action == ROOMBA_REMOTE_BACK) {
			roombaStop();
			drive_state = LEAVE_DOCK;
			angle_approach_state = DRIVE_ANGLE;
			line_approach_state = LINE_TURN_FROM_BASE;
			fenceapproach_state = FENCE_ANGLE;
			has_driven = false;
			roombadata.current_base_id = 0;
			roombadata.destination_base_id = 0;
			roombaOnCollisionCleared();
			roombaWriteSevensegDigits();
			setProgramState(CALIBRATE);
		} else {
			setProgramState(program_state);
		}

		ir_action = 0;
		my_msleep(50);
		global_clock += 50;
	}
}

int8_t intToAscii(int32_t value, int32_t out[]) {

	if (value < 0) {
		roombaSetWeekdayLed(1);
	}
	value = mymathAbs(value);

	out[3] = (value % 10000) / 1000 + ASCII_NUMBER_START;
	out[2] = (value % 1000) / 100 + ASCII_NUMBER_START;
	out[1] = (value % 100) / 10 + ASCII_NUMBER_START;
	out[0] = value % 10 + ASCII_NUMBER_START;
	// integer too large
	if (value >= 10000)
		return -1;
	else
		return 1;
}

void setProgramState(enum programstate state) {
	switch (state) {
	case CALIBRATE:
		roombaSetLed(LED_DIRT_DETECT_BLUE, 0, 0);
		break;
	case DRIVE:
		roombaSetLed(LED_SPOT_GREEN, 0, 0);
		break;
	case COLLISION:
		roombaSetLed(LED_CHECK_ROBOT_RED, 0, 0);
		break;
	case SEEKDOCK:
		roombaSetLed(0, SEEKDOCK_LED_BITMASK, SEEKDOCK_LED_BITMASK);
		break;
	case DOCKED:
		roombaSetLed(0, DOCKED_LED_BITMASK, DOCKED_LED_BITMASK);
		break;
	}
	program_state = state;
}

enum programstate handleStateCalibrate() {
	switch (calibrate_state) {
	case CALIBRATE_DISTANCE:
		base_config_state = BASE_SELECT;
		roomba_sevenseg_digits[3] = 'C';
		roomba_sevenseg_digits[2] = 'A';
		roomba_sevenseg_digits[1] = 'L';
		roomba_sevenseg_digits[0] = 'D';
		roombaWriteSevensegDigits();

		switch (ir_action) {
		case ROOMBA_REMOTE_CROSS_OK:
			roombaCalibrateDistance();
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
		roombaWriteSevensegDigits();

		switch (ir_action) {
		case ROOMBA_REMOTE_CROSS_OK:
			roombaCalibrateAngle();
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
		roombaWriteSevensegDigits();

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

enum programstate handleStateDrive() {
	switch (drive_state) {
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

enum programstate handleStateCollision() {

	collisiondata.program_tick_counter++;
	//after 30 seconds in collision mode stop driving and wait for user action
	if (collisiondata.program_tick_counter >= 200) {
		roombaStop();
		roombaPlaySongBeep();
		if (ir_action == ROOMBA_REMOTE_CROSS_OK) {
			roombaOnCollisionCleared();
			return DRIVE;
		} else {
			return COLLISION;
		}
	}

	switch (collision_state) {
	case COLLISION_TURN:
		if (!roombadata.is_moving) {

			//check physical bumpers and wheeldrops before light bumps to ensure that they have priority
			if (collisiondata.bumper_state != 0) {
				//any wheeldrop
				if (collisiondata.bumper_state >= 0x4) {
					//use Day button to return to drive mode
					if (ir_action == ROOMBA_REMOTE_CROSS_OK) {
						roombaOnCollisionCleared();
						return DRIVE;
					} else {
						return COLLISION;
					}
				}

				roombaDriveABitBackward(DIFFERENCE_TO_BASE / 4);
			}

			roombaResetTrips();

			if (!collisiondata.played_acustic_feedback) {
				roombaPlaySongCollision();
				my_msleep(10000);
				//check if collision still exists
				collisiondata.bumper_state = roombaQuerySensor(
						PACKET_BUMPS_WHEELDROPS);
				collisiondata.light_bumper_state = roombaQuerySensor(
						PACKET_LIGHT_BUMPER);
				if (collisiondata.bumper_state == 0
						&& collisiondata.light_bumper_state == 0) {
					roombaOnCollisionCleared();
					return DRIVE;
				}
				//any wheeldrop
				else if (collisiondata.bumper_state >= 0x4) {
					return COLLISION;
				}
				collisiondata.played_acustic_feedback = 1;
			}

			roombaDrive(DEFAULT_VELOCITY / 2, LEFT);
		} else {

			roombaQuerySensor(PACKET_ANGLE);
			//didnt find any collision
			if (roombadata.trip_angle >= 360) {
				roombaStop();
				roombaOnCollisionCleared();
				return DRIVE;
			}

			int32_t light_signal = roombaQuerySensor(
					PACKET_LIGHT_BUMP_RIGHT_SIGNAL);
			if (light_signal >= LIGHT_BUMP_CLEAR
					&& light_signal <= LIGHT_BUMP_NOT_CLEAR) {
				roombaStop();
				collisiondata.angle_sum += roombadata.trip_angle;
				roombaResetTrips();
				collision_state = COLLISION_DRIVE;
			}
		}
		break;
	case COLLISION_DRIVE:

		if (!roombadata.is_moving) {
			roombaDrive(DEFAULT_VELOCITY / 2, STRAIGHT);
		} else {
			roombaQuerySensor(PACKET_ANGLE);
			roombaQuerySensor(PACKET_DISTANCE);

			//while driving check if new collisions are detected
			int32_t bumper_state = roombaQuerySensor(PACKET_BUMPS_WHEELDROPS);
			int32_t light_bumper_state = roombaQuerySensor(PACKET_LIGHT_BUMPER);
			if (bumper_state != 0 || light_bumper_state != 0) {

				collisiondata.angle_sum += roombadata.trip_angle;
				roombaUpdateDistanceSum();
				roombaStop();
				roombaResetTrips();
				collisiondata.bumper_state = bumper_state;
				collisiondata.light_bumper_state = light_bumper_state;
				collision_state = COLLISION_TURN;
			} else {
				int32_t light_signal = roombaQuerySensor(
						PACKET_LIGHT_BUMP_RIGHT_SIGNAL);
				//lost right bumper
				if (light_signal <= LIGHT_BUMP_CLEAR) {

					int32_t distance_value = roombadata.trip_distance
							+ DIFFERENCE_TO_BASE / 6;
					roombaDrive(DEFAULT_VELOCITY / 2, STRAIGHT);
					while (roombadata.trip_distance <= distance_value) {
						my_msleep(50);
						roombaQuerySensor(PACKET_DISTANCE);
					}
					roombaStop();

					collisiondata.angle_sum += roombadata.trip_angle;
					roombaUpdateDistanceSum();
					roombaResetTrips();

					roombaDrive(DEFAULT_VELOCITY / 2, RIGHT);
				} else if (light_signal >= LIGHT_BUMP_NOT_CLEAR) {
					collisiondata.angle_sum += roombadata.trip_angle;
					roombaUpdateDistanceSum();
					roombaResetTrips();
					roombaDrive(DEFAULT_VELOCITY / 2, LEFT);
				} else {
					if (roombadata.trip_distance > 0
							&& roombadata.trip_distance >= 50) {
						collisiondata.angle_sum += roombadata.trip_angle;
						roombaUpdateDistanceSum();
						roombaResetTrips();
					}

					roombaDrive(DEFAULT_VELOCITY / 2, STRAIGHT);
				}

				//after 3 seconds in collision mode begin to check if collision is avoided
				if (collisiondata.program_tick_counter >= 20) {

					// check if we found back to the line (LINE_APPROACH)
					if (drive_state == LINE_APPROACH) {
						int32_t cliff_front_left_signal = roombaQuerySensor(
								PACKET_CLIFF_FRONT_LEFT_SIGNAL);
						int32_t cliff_front_right_signal = roombaQuerySensor(
								PACKET_CLIFF_FRONT_RIGHT_SIGNAL);
						if (cliff_front_left_signal >= TAPE_SIGNAL
								&& cliff_front_right_signal >= TAPE_SIGNAL) {
							roombaStop();
							roombaResetTrips();
							int32_t angle_to_turn = 45;
							roombaDrive(DEFAULT_VELOCITY / 2, LEFT);

							while (roombadata.trip_angle <= angle_to_turn) {
								my_msleep(50);
								roombaQuerySensor(PACKET_ANGLE);
							}
							roombaStop();
							roombaOnCollisionCleared();
							return DRIVE;
						} else if (cliff_front_left_signal >= TAPE_SIGNAL) {
							roombaStop();
							roombaResetTrips();
							int32_t angle_to_turn = 75;
							roombaDrive(DEFAULT_VELOCITY / 2, LEFT);

							while (roombadata.trip_angle <= angle_to_turn) {
								my_msleep(50);
								roombaQuerySensor(PACKET_ANGLE);
							}
							roombaStop();
							roombaOnCollisionCleared();
							return DRIVE;
						} else if (cliff_front_right_signal >= TAPE_SIGNAL) {
							roombaStop();
							roombaResetTrips();
							roombaOnCollisionCleared();
							return DRIVE;

						}
					}
				}
				//check if we have driven 20 centimeters of our the real course while avoiding collision
				if (collisiondata.driven_trip_distance >= 200) {
					//in angle and fence approach check the calculated "away-from-course" distance value
					if (drive_state == ANGLE_APPROACH
							|| drive_state == FENCE_APPROACH) {
						// if we are only 5 centimeters left or right away from our course, then stop collision avoidance turn 'angle_sum' back and return to normal drive mode
						if (collisiondata.distance_sum < 50
								&& collisiondata.distance_sum > -50) {
							roombaStop();
							roombaResetTrips();
							int32_t angle_to_turn = 0;
							int32_t angle_sum_abs =
									collisiondata.angle_sum < 0 ?
											-collisiondata.angle_sum :
											collisiondata.angle_sum;
							angle_sum_abs %= 360;
							angle_to_turn =
									angle_sum_abs > 180 ?
											-(360 - angle_sum_abs) :
											angle_sum_abs;
							angle_to_turn =
									collisiondata.angle_sum < 0 ?
											angle_to_turn : -angle_to_turn;

							roombaDrive(DEFAULT_VELOCITY / 2,
									angle_to_turn < 0 ? RIGHT : LEFT);

							while ((angle_to_turn > 0
									&& roombadata.trip_angle < angle_to_turn)
									|| (angle_to_turn < 0
											&& roombadata.trip_angle
													> angle_to_turn)) {
								my_msleep(50);
								roombaQuerySensor(PACKET_ANGLE);
							}
							roombaStop();
							roombaResetTrips();

							roombaOnCollisionCleared();

							return DRIVE;
						}
					}
				}

			}
		}
		break;

	}

	return COLLISION;
}

enum programstate handleStateSeekdock() {
	//check if we reached the dock?
	if (roombaQuerySensor(
			PACKET_CHARGING_SOURCES_AVAILABLE) == CHARGING_SOURCE_HOMEBASE) {
		roombaInit();

		//consume sensor values
		roombaQuerySensor(PACKET_ANGLE);
		roombaQuerySensor(PACKET_DISTANCE);

		// reset values for docked state
		roombadata.current_base_id = 0;
		angle_approach_state = DRIVE_ANGLE;
		line_approach_state = LINE_TURN_FROM_BASE;
		fenceapproach_state = FENCE_ANGLE;

		return DOCKED;
	} else {
		return SEEKDOCK;
	}
}

enum programstate handleStateDocked() {

	if (roombadata.current_base_id == 0) {
		roombadata.current_base_id = check_base_id();
		if (roombadata.current_base_id != 0) {
			if (has_driven && roombadata.destination_base_id != 0
					&& roombadata.current_base_id
							!= roombadata.destination_base_id) {
				drive_state = LEAVE_DOCK;
				return DRIVE;
			}
		}
	} else if (roombadata.current_base_id == roombadata.destination_base_id) {
		roombadata.destination_base_id = 0;
	}

	switch (nextbase_state) {
	case NEXTBASE_DRIVE:

		roomba_sevenseg_digits[3] = 'A';
		roomba_sevenseg_digits[2] = 'T';
		roomba_sevenseg_digits[1] = ' ';
		roomba_sevenseg_digits[0] = (roombadata.current_base_id
				+ ASCII_NUMBER_START);

		switch (ir_action) {
		case ROOMBA_REMOTE_CROSS_OK:
			if (roombadata.current_base_id != 0
					&& roombadata.destination_base_id != 0) {
				docked_in_menu = false;
				has_driven = true;
				drive_state = LEAVE_DOCK;
				return DRIVE;
			}
			break;
		case ROOMBA_REMOTE_CROSS_DOWN:
			nextbase_state = NEXTBASE_NUM;
			docked_in_menu = false;
			break;
		case ROOMBA_REMOTE_CROSS_UP:
			nextbase_state = NEXTBASE_APPROACH;
			docked_in_menu = false;
			break;
		default:
			break;
		}
		break;

	case NEXTBASE_NUM:
		roomba_sevenseg_digits[3] = 'B';
		roomba_sevenseg_digits[2] = 'N';
		roomba_sevenseg_digits[1] = 'U';
		roomba_sevenseg_digits[0] = 'M';
		switch (ir_action) {
		case ROOMBA_REMOTE_CROSS_DOWN:
			nextbase_state = NEXTBASE_APPROACH;
			break;
		case ROOMBA_REMOTE_CROSS_UP:
			nextbase_state = NEXTBASE_DRIVE;
			break;
		case ROOMBA_REMOTE_NUM_1:
			roombadata.destination_base_id = 1;
			docked_in_menu = true;
			break;
		case ROOMBA_REMOTE_NUM_2:
			roombadata.destination_base_id = 2;
			docked_in_menu = true;
			break;
		case ROOMBA_REMOTE_NUM_3:
			roombadata.destination_base_id = 3;
			docked_in_menu = true;
			break;
		case ROOMBA_REMOTE_NUM_4:
			roombadata.destination_base_id = 4;
			docked_in_menu = true;
			break;
		case ROOMBA_REMOTE_NUM_5:
			roombadata.destination_base_id = 5;
			docked_in_menu = true;
			break;
		default:
			break;
		}

		if (roombadata.destination_base_id != 0 && docked_in_menu == true) {
			roomba_sevenseg_digits[3] = 'B';
			roomba_sevenseg_digits[2] = 'A';
			roomba_sevenseg_digits[1] = ' ';
			roomba_sevenseg_digits[0] = (roombadata.destination_base_id
					+ ASCII_NUMBER_START);
			roombaWriteSevensegDigits();
			docked_in_menu = false;
			my_msleep(2000);
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
				docked_in_menu = false;
				break;
			case ROOMBA_REMOTE_CROSS_DOWN:
				nextbase_state = NEXTBASE_DRIVE;
				docked_in_menu = false;
				break;
			default:
				break;
			}
		} else {
			if (ir_action == ROOMBA_REMOTE_CROSS_OK) {
				docked_in_menu = false;
			}
			switch (current_approach) {
			case ANGLE_APPROACH:
				roomba_sevenseg_digits[3] = 'A';
				roomba_sevenseg_digits[2] = 'N';
				roomba_sevenseg_digits[1] = 'G';
				roomba_sevenseg_digits[0] = 'L';
				if (ir_action == ROOMBA_REMOTE_CROSS_UP)
					current_approach = LINE_APPROACH;
				else if (ir_action == ROOMBA_REMOTE_CROSS_DOWN)
					current_approach = FENCE_APPROACH;

				break;
			case LINE_APPROACH:
				roomba_sevenseg_digits[3] = 'L';
				roomba_sevenseg_digits[2] = 'I';
				roomba_sevenseg_digits[1] = 'N';
				roomba_sevenseg_digits[0] = 'E';
				if (ir_action == ROOMBA_REMOTE_CROSS_UP)
					current_approach = FENCE_APPROACH;
				else if (ir_action == ROOMBA_REMOTE_CROSS_DOWN)
					current_approach = ANGLE_APPROACH;
				break;
			case FENCE_APPROACH:
				roomba_sevenseg_digits[3] = 'F';
				roomba_sevenseg_digits[2] = 'E';
				roomba_sevenseg_digits[1] = 'N';
				roomba_sevenseg_digits[0] = 'C';
				if (ir_action == ROOMBA_REMOTE_CROSS_UP)
					current_approach = ANGLE_APPROACH;
				else if (ir_action == ROOMBA_REMOTE_CROSS_DOWN)
					current_approach = LINE_APPROACH;
				break;
			default:
				break;
			}
		}

		break;

	default:
		break;
	}

	roombaWriteSevensegDigits();

	return DOCKED;
}

enum programstate handleSubStateLeaveDock() {

	roombaResetTrips();
	roombaDriveABitBackward(DIFFERENCE_TO_BASE);

	drive_state = current_approach;
	roombaResetTrips();

	return DRIVE;
}

enum programstate handleSubStateAngleApproach() {

	volatile int16_t angle_to_drive = workbenchGetAngle(
			roombadata.current_base_id, roombadata.destination_base_id);
	volatile int16_t distance_to_drive = workbenchGetDistance(
			roombadata.current_base_id, roombadata.destination_base_id);

	switch (angle_approach_state) {
	case DRIVE_ANGLE:
		//start turning if necessary
		if (!roombadata.is_moving) {
			roombaResetTrips();
			roombaDrive(DEFAULT_VELOCITY, angle_to_drive < 0 ? RIGHT : LEFT);
		} else {
			//check if defined angle is reached
			roombaQuerySensor(PACKET_ANGLE);
			if ((angle_to_drive < 0 && roombadata.trip_angle <= angle_to_drive)
					|| (angle_to_drive > 0
							&& roombadata.trip_angle >= angle_to_drive)) {
				roombaStop();
				roombaResetTrips();
				angle_approach_state = DRIVE_DISTANCE;
			}
		}
		break;
	case DRIVE_DISTANCE: {
		//check if collisions are detected
		int32_t bumper_state = roombaQuerySensor(PACKET_BUMPS_WHEELDROPS);
		int32_t light_bumper_state = roombaQuerySensor(PACKET_LIGHT_BUMPER);
		if (bumper_state != 0 || light_bumper_state != 0) {
			roombaOnCollisionDetected(bumper_state, light_bumper_state);
			return COLLISION;
		}

		//start driving if necessary
		if (!roombadata.is_moving) {
			roombaDrive(DEFAULT_VELOCITY, STRAIGHT);
		} else {
			roombaQuerySensor(PACKET_DISTANCE);
			int32_t infrared_value = 0;

			// 1,5 meter before whole distance is reached we check infrared sensors
			if (roombadata.trip_distance
					>= distance_to_drive - SEEKDOCK_TRIGGER_DISTANCE) {
				infrared_value = roombaQuerySensor(
						PACKET_INFRARED_CHARACTER_LEFT);
				infrared_value =
						infrared_value <= INFRARED_VALUE_DOCK_TRIGGER ?
								roombaQuerySensor(
										PACKET_INFRARED_CHARACTER_RIGHT) :
								infrared_value;
				infrared_value =
						infrared_value <= INFRARED_VALUE_DOCK_TRIGGER ?
								roombaQuerySensor(
										PACKET_INFRARED_CHARACTER_OMNI) :
								infrared_value;
			}

			// found infrared sensor or reached whole distance
			if (infrared_value > INFRARED_VALUE_DOCK_TRIGGER
					|| roombadata.trip_distance >= distance_to_drive) {
				roombaStop();
				roombaResetTrips();
				roombaSeekdock();
				return SEEKDOCK;
			}

		}
		break;
	}

	}

	return DRIVE;
}

enum programstate handleSubStateLineApproach() {

	switch (line_approach_state) {
	case LINE_TURN_FROM_BASE:
		//start driving if necessary
		if (!roombadata.is_moving) {
			roombaResetTrips();
			roombaDrive(DEFAULT_VELOCITY, LEFT);
		} else {
			//update trip angle value
			roombaQuerySensor(PACKET_ANGLE);
			//if turned 180 degrees switch to LINE_DRIVE
			if (roombadata.trip_angle >= 180) {
				roombaStop();
				roombaResetTrips();
				line_approach_state = LINE_DRIVE;
			}
		}
		break;
	case LINE_DRIVE:
		//start driving if necessary
		if (!roombadata.is_moving) {
			roombaDrive(DEFAULT_VELOCITY, STRAIGHT);
		}

		//check if collisions are detected
		int32_t bumper_state = roombaQuerySensor(PACKET_BUMPS_WHEELDROPS);
		int32_t light_bumper_state = roombaQuerySensor(PACKET_LIGHT_BUMPER);
		if (bumper_state != 0 || light_bumper_state != 0) {
			roombaOnCollisionDetected(bumper_state, light_bumper_state);
			return COLLISION;
		}

		//check if we reached base marker (left and right cliff signal)
		int32_t cliff_left_signal = roombaQuerySensor(PACKET_CLIFF_LEFT_SIGNAL);
		int32_t cliff_right_signal = roombaQuerySensor(
				PACKET_CLIFF_RIGHT_SIGNAL);
		if (cliff_left_signal >= TAPE_SIGNAL
				&& cliff_right_signal >= TAPE_SIGNAL) {
			int32_t infrared_value = 0;
			infrared_value = roombaQuerySensor(PACKET_INFRARED_CHARACTER_LEFT);
			infrared_value =
					infrared_value <= INFRARED_VALUE_DOCK_TRIGGER ?
							roombaQuerySensor(PACKET_INFRARED_CHARACTER_RIGHT) :
							infrared_value;
			infrared_value =
					infrared_value <= INFRARED_VALUE_DOCK_TRIGGER ?
							roombaQuerySensor(PACKET_INFRARED_CHARACTER_OMNI) :
							infrared_value;
			if (infrared_value > INFRARED_VALUE_DOCK_TRIGGER) {
				roombaStop();
				roombaResetTrips();
				roombaSeekdock();
				return SEEKDOCK;
			} else {
				roombaDrive(DEFAULT_VELOCITY, STRAIGHT);
			}
		} else {
			int32_t cliff_front_left_signal = roombaQuerySensor(
					PACKET_CLIFF_FRONT_LEFT_SIGNAL);
			int32_t cliff_front_right_signal = roombaQuerySensor(
					PACKET_CLIFF_FRONT_RIGHT_SIGNAL);

			// just drive on
			if (cliff_front_left_signal < TAPE_SIGNAL
					&& cliff_front_right_signal < TAPE_SIGNAL) {
				roombaDrive(DEFAULT_VELOCITY, STRAIGHT);
			}
			// turn left
			else if (cliff_front_left_signal >= TAPE_SIGNAL
					&& cliff_front_right_signal < TAPE_SIGNAL) {
				roombaDrive(DEFAULT_VELOCITY / 2, 150);
			}
			//turn right
			else if (cliff_front_right_signal >= TAPE_SIGNAL
					&& cliff_front_left_signal < TAPE_SIGNAL) {
				roombaDrive(DEFAULT_VELOCITY / 2, -150);
			} else {
				roombaDrive(DEFAULT_VELOCITY / 2, 50);
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

direction getOppositeDirection(direction dir) {
	switch (dir) {
	case LEFT:
		return RIGHT;
	case RIGHT:
		return LEFT;
	case STRAIGHT:
	default:
		return STRAIGHT;
	}
}

#define ROOMBA_FENCE_MAX_EVASIVE_ANGLE 25
#define FENCE_VALUE_COLLECTION_TIME 3000

direction current_fence_direction = STRAIGHT;

millis_t fence_recog_start_time = -1;
int32_t wall_detection_counter = 0;
uint8_t fence_drive_counter = 0;
int32_t fence_correct_straight_distance = 0;

enum programstate handleSubStateFenceApproach() {
	/*
	 OLD LIGHTHOUSE CODE
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
	 return DRIVE;*/

	int16_t angle_to_drive = workbenchGetAngle(roombadata.current_base_id,
			roombadata.destination_base_id);

	//check if collisions are detected
	int32_t bumper_state = roombaQuerySensor(PACKET_BUMPS_WHEELDROPS);
	int32_t light_bumper_state = roombaQuerySensor(PACKET_LIGHT_BUMPER);
	if (bumper_state != 0 || light_bumper_state != 0) {
		roombaOnCollisionDetected(bumper_state, light_bumper_state);
		return COLLISION;
	}

	// if correct base is recognized drive to it and dock
	if (check_discrete_base_id() == roombadata.destination_base_id) {
		roombaStop();
		roombaResetTrips();
		roombaSeekdock();
		return SEEKDOCK;
	}

	switch (fenceapproach_state) {
	case FENCE_ANGLE:
		//start turning if necessary
		if (!roombadata.is_moving) {
			roombaResetTrips();
			roombaDrive(DEFAULT_VELOCITY, (angle_to_drive < 0 ? RIGHT : LEFT));
		} else {
			//check if defined angle is reached
			roombaQuerySensor(PACKET_ANGLE);
		}

		if (mymathAbs(roombadata.trip_angle) >= mymathAbs(angle_to_drive)) {
			roombaStop();
			roombaResetTrips();
			fenceapproach_state = FENCE_STRAIGHT;
		}
		break;

	case FENCE_STRAIGHT: {

		// drive straight
		uint8_t wall_detected = 0;
		if (!roombadata.is_moving) {
			roombaDrive(DEFAULT_VELOCITY, STRAIGHT);
		} else {
			roombaQuerySensor(PACKET_DISTANCE);
			wall_detected = roombaQuerySensor(PACKET_VIRTUAL_WALL);
			if (wall_detected == 1 && fencedetect_state == FENCE_NONE) {
				fencedetect_state = FENCE_FIRST;
				roombaStop();
				roombaResetTrips();
				wall_detection_counter = 1;
				fence_recog_start_time = global_clock;
			}
		}

		if (fencedetect_state != FENCE_NONE) {
			if (fence_drive_counter <= 3) {
				roombaDrive(DEFAULT_VELOCITY / 2, (int16_t) 0);
				fence_drive_counter++;
				while (roombadata.trip_distance <= 20) {
					my_msleep(50);
					roombaQuerySensor(PACKET_DISTANCE);
				}
				roombaStop();
			}

			while ((global_clock - fence_recog_start_time)
					> FENCE_VALUE_COLLECTION_TIME) {
				wall_detection_counter += wall_detected;
				my_msleep(150);
				global_clock += 150;
			}
			switch (fencedetect_state) {
			case FENCE_NONE:
				break;

			case FENCE_FIRST:
				led_set_blue(ledb_vals[0]);
				if (wall_detection_counter == 0) {
					fencedetect_state = FENCE_VOID;
					fence_drive_counter = 0;
				}
				break;

			case FENCE_VOID:
				led_set_blue(ledb_vals[1]);
				if (wall_detection_counter > 1) {
					fencedetect_state = FENCE_SECOND;
					fence_drive_counter = 0;
				} else if (fence_drive_counter >= 3) {
					current_fence_direction = RIGHT;
					fenceapproach_state = FENCE_CORRECTION_STRAIGHT;
				}
				break;

			case FENCE_SECOND:
				led_set_blue(ledb_vals[2]);
				if (wall_detection_counter == 0) {
					current_fence_direction = LEFT;
					fenceapproach_state = FENCE_CORRECTION_STRAIGHT;
				}
				break;

			default:
				break;
			}
			if (fenceapproach_state == FENCE_CORRECTION_STRAIGHT) {
				fencedetect_state = FENCE_NONE;
				fence_drive_counter = 0;
			}
		}

		break;
	}
	case FENCE_CORRECTION_STRAIGHT:
		// drive backwards
		if (!roombadata.is_moving) {
			roombaDrive(-DEFAULT_VELOCITY, STRAIGHT);
			fence_correct_straight_distance = roombaQuerySensor(
					PACKET_DISTANCE);
		} else {
			roombaQuerySensor(PACKET_DISTANCE);
		}

		// if driven far enough backwards (average distance between two lighthouses on the right side)
		if ((roombadata.trip_distance / 10) > fence_correct_straight_distance) {
			roombaStop();
			roombaResetTrips();
			fenceapproach_state = FENCE_CORRECTION_ANGLE;
		}

		break;

	case FENCE_CORRECTION_ANGLE:
		// turn away from fence
		if (!roombadata.is_moving) {
			roombaDrive(DEFAULT_VELOCITY,
					getOppositeDirection(current_fence_direction));
		} else {
			roombaQuerySensor(PACKET_ANGLE);
		}

		// if fence no longer recognized or angle turned too large resume straight path
		if (roombaQuerySensor(PACKET_VIRTUAL_WALL)
				== 0|| mymathAbs(roombadata.trip_angle) >= ROOMBA_FENCE_MAX_EVASIVE_ANGLE) {
			roombaStop();
			roombaResetTrips();
			current_fence_direction = STRAIGHT;
			fenceapproach_state = FENCE_STRAIGHT;
			roombaPlaySongDone();
			my_msleep(3000);
		}

		break;

	}

	/*uint8_t trip_difference = 0;


	 switch (fenceapproach_state) {
	 case FENCE_ANGLE:
	 led_set_blue(1);
	 //start turning if necessary
	 if(!roombadata.is_moving){
	 roombaResetTrips();
	 roombaDrive(DEFAULT_VELOCITY, (angle_to_drive < 0 ? RIGHT : LEFT));
	 } else {
	 //check if defined angle is reached
	 roombaQuerySensor(PACKET_ANGLE);
	 }

	 if(mymathAbs(roombadata.trip_angle) >= mymathAbs(angle_to_drive)) {
	 roombaStop();
	 roombaResetTrips();
	 fenceapproach_state = FENCE_STRAIGHT;
	 }
	 break;

	 case FENCE_STRAIGHT:
	 led_set_blue(2);
	 // drive straight
	 if (!roombadata.is_moving) {
	 roombaDrive(DEFAULT_VELOCITY, STRAIGHT);
	 } else {
	 roombaQuerySensor(PACKET_DISTANCE);
	 }

	 // calculate distance driven since last seen virtual wall lighthouse
	 if (recognition_trip_distance != -1) {
	 trip_difference = (int32_t)(mymathAbs(roombadata.trip_distance - recognition_trip_distance) / 10);
	 intToAscii(trip_difference, roomba_sevenseg_digits);
	 roombaWriteSevensegDigits();
	 }

	 // check if virtual wall lighthouse detected (only possible values: 0, 1)
	 if (roombaQuerySensor(PACKET_VIRTUAL_WALL) == 1) {
	 roombaSetLed(LED_DIRT_DETECT_BLUE, 0, 0);
	 if (recognition_trip_distance == -1) {
	 recognition_trip_distance = roombadata.trip_distance;
	 fence_waiting_for_second = true;
	 roombaSetWeekdayLed(1);
	 } else if (fence_may_detect_second) {
	 current_fence_direction = RIGHT;
	 fence_may_detect_second = false;
	 roombaSetWeekdayLed(3);
	 }
	 } else if (roombaQuerySensor(PACKET_VIRTUAL_WALL) == 0 && fence_waiting_for_second && trip_difference >= LIGHTHOUSE_SECOND_MIN_RECOGNITION_DISTANCE) {
	 fence_may_detect_second = true;
	 fence_waiting_for_second = false;
	 roombaSetWeekdayLed(2);
	 }

	 // if driven for set distance and not seen
	 //if (recognition_trip_distance == -1 && trip_difference >= LIGHTHOUSE_SECOND_MIN_RECOGNITION_DISTANCE)
	 //	current_fence_direction = LEFT;

	 // if lighthouse direction successfully determined switch to correction mode
	 if (current_fence_direction != STRAIGHT) {
	 roombaStop();
	 roombaResetTrips();
	 my_msleep(2000);
	 if (current_fence_direction == RIGHT) {
	 roombaSetWeekdayLed(0x40);
	 } else if (current_fence_direction == LEFT) {
	 roombaSetWeekdayLed(0x1);
	 }
	 roombaPlaySongDone();

	 fence_waiting_for_second = false;
	 fence_may_detect_second = false;
	 recognition_trip_distance = -1;

	 my_msleep(2000);
	 fenceapproach_state = FENCE_CORRECTION_STRAIGHT;
	 trip_difference = 0;
	 }

	 break;

	 case FENCE_CORRECTION_STRAIGHT:
	 led_set_blue(3);
	 recognition_trip_distance = -1;

	 // drive backwards
	 if (!roombadata.is_moving) {
	 roombaDrive(-DEFAULT_VELOCITY, STRAIGHT);
	 } else {
	 roombaQuerySensor(PACKET_DISTANCE);
	 }

	 // if driven far enough backwards (average distance between two lighthouses on the right side)
	 if ((roombadata.trip_distance / 10) > LIGHTHOUSE_RECOGNITION_DISTANCE) {
	 roombaStop();
	 roombaResetTrips();
	 fenceapproach_state = FENCE_CORRECTION_ANGLE;
	 }

	 break;

	 case FENCE_CORRECTION_ANGLE:
	 led_set_blue(4);
	 // turn away from fence
	 if (!roombadata.is_moving) {
	 roombaDrive(DEFAULT_VELOCITY, getOppositeDirection(current_fence_direction));
	 } else {
	 roombaQuerySensor(PACKET_ANGLE);
	 }

	 // if fence no longer recognized or angle turned too large resume straight path
	 if (roombaQuerySensor(PACKET_VIRTUAL_WALL) == 0 || mymathAbs(roombadata.trip_angle) >= ROOMBA_FENCE_MAX_EVASIVE_ANGLE) {
	 roombaStop();
	 roombaResetTrips();
	 current_fence_direction = STRAIGHT;
	 fenceapproach_state = FENCE_STRAIGHT;
	 roombaPlaySongDone();
	 my_msleep(3000);
	 }

	 break;

	 }*/
	return DRIVE;
}

