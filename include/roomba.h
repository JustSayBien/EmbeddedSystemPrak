#ifndef _ROOMBA_H
#define _ROOMBA_H

/****************************************************************** Includes */
#include <stdint.h>

/******************************************************************* Defines */

#define ROOMBA_DELAY_MODECHANGE_MS      20

#define CMD_SOFT_RESET			0x07	//supposedly does a soft reset
#define CMD_START				0x80	//start the ROI; roomba is in passive mode afterwards; use multiple times to reset
#define CMD_BAUD				0x81	//1 data byte; set baud rate in bits per second; data byte is the baud code (0-11)
										//use a code of 11 as a good default (115200bps)
#define CMD_CONTROL				0x82	//enables control of the roomba; send after start; sets roomba to "safe" mode
#define CMD_SAFE				0x83	//return roomba to "safe" mode
#define CMD_FULL				0x84	//put roomba in "full" mode
#define CMD_POWER				0x85	//virtual "power" button press; puts roomba in "sleep" mode
#define CMD_SPOT				0x86	//virtual "spot" button press; starts the "spot" cleaning algorithm
#define CMD_CLEAN				0x87	//virtual "clean" button press; starts "clean" cleaning algorithm
#define CMD_MAX					0x88	//virtual "max" button press; starts "max" cleaning algorithm
#define CMD_DRIVE				0x89	//4 data bytes; controls the drive wheels two bytes for velocity and two for direction;
										//each represents a 16-bit signed value using two's complement
										//velocity is the speed to use from -500 to 500 (0xFE0C to 0x01F4)
										//direction is either the turn radius (-2000 to 2000mm) (0xF830 to 0x07D0) or a special value:
										//	straight: 32768 (0x8000) or 32767 (0x7FFF)
										//	Spin counter-clockwise: 1 (0x0001)
										//	Spin clockwise: -1 (0xFFFF)
										//when going forward a positive radius turns to the right and a negative to the left
#define CMD_MOTORS				0x8A	//1 data byte; turn clean motors on/off;
										//bitfield: main brush=bit 2(0x04), vacuum=bit 1(0x02), side brush=bit 0(0x01)
										//bits 3(0x08) and 4(0x10) control side brush and main brush direction;
										//0=default direction;1=other direction
#define CMD_LEDS				0x8B	//3 data bytes;
										//first is a bitfield: status(green)=bit 5(0x20), status(red)=bit 4(0x10),
										//spot(green)=bit 3(0x08), clean(green)=bit 2(0x04), max(green)=bit 1(0x02),
										//dirt detect(blue)=bit 0(0x01)
										//second is the color of the power led: 0=green, 255=red; values inbetween mix the colors
										//third is the intensity of the power led: 0=off, 255=full on;
										//values inbetween are intermediate intensities
#define CMD_SONG				0x8C	//2+2N data bytes; first byte defines number of song to specify (0-4);
										//the second byte defines the length of the song(1-16); number of specified notes follow
										//a note consists of two bytes: note number(0x1F-0x7F)(G0-G8)
										//and note duration in units of 1/64th of a second
#define CMD_PLAY				0x8D	//1 data byte; play a song in the slot defined by data byte 1; song does not repeat
#define CMD_SENSORS				0x8E	//1 data byte; requests sensor data; data byte specifies the requested packet ID;
										//see OI Specification for sensor packet IDs;
										//a value of 100 specifies to retrieve all sensor data
										//values from 0 to 6 and from 101 to 107 specify groups of sensor data
#define CMD_DOCK				0x8F	//turns on "force seeking dock" algorithm
#define CMD_PWM_MOTORS			0x90	//3 data bytes;control speed of brushes: [main brush pwm][side brush pwm][vacuum pwm];
										//brushes take values from -127-127; vacuum 0-127
#define CMD_DRIVE_DIRECT		0x91	//4 data bytes;first two bytes control the right wheel velocity;
										//the last two bytes the left wheel velocity (mm/s)
										//the first byte of each velocity is the high byte,
										//and the second is the low byte of a 16-bit signed value (twos-complement); see CMD_DRIVE
										//velocity is -500 - 500
#define CMD_DRIVE_PWM			0x92	//4 data bytes;first two bytes control the right wheel PWM;
										//the last two bytes the left wheel PWM
										//the first byte of each PWM is the high byte,
										//and the second is the low byte of a 16-bit signed value
										//PWM is -255 - 255
#define CMD_STREAM				0x94	//N+1 data bytes; starts a stream of data packets; the data is sent every 15ms
										//first data byte is the number of requested packets
										//each following data byte is an ID of a requested packet
										//return format is: [0x13] [Length] [Packet 1 ID][Packet 1 Data]
										//[Packet 2 ID][Packet 2 Data] ... [Checksum]
										//length is the number of following bytes (not including checksum)
										//the checksum is 1 byte: the 8-bit complement of all bytes between header and checksum
										//if more data is requested than can be sent in the timeslot the results get corrupted
#define CMD_QUERY_LIST			0x95	//N+1 data bytes; first data byte is the number of requested packets
										//each following data byte is an ID of a requested packet
										//the packets are returned in the specified order
#define CMD_PAUSE_RESUME_STREAM	0x96	//1 data byte; data of 0 stops a running stream without clearing the list of requested packets
										//data of 1 starts the stream using the last used packet list
#define CMD_SCHEDULE_LED		0xA2	//2 data bytes; scheduling leds;first byte for the weekday leds;
										//second byte for the scheduling leds; both bitfields
#define CMD_7SEG_RAW			0xA3	//4 data bytes;[digit 3][digit 2][digit 1][digit 0];see roomba oi spec p. 13
#define CMD_7SEG_ASCII			0xA4	//4 data bytes;[digit 3][digit 2][digit 1][digit 0];ascii codes from 32-126 are supported
#define CMD_BUTTONS				0xA5	//1 data byte;simulates button push with autorelease; bitfield
#define CMD_SCHEDULE			0xA7	//15 data bytes; sends a new schedule;24 hour time format
										//[days] [sun_hour] [sun_minute] [mon_hour] [mon_minute] [tue_hour] [tue_minute]
										//[wed_hour] [wed_minute] [thu_hour] [thu_minute] [fri_hour] [fri_minute]
										//[sat_hour] [sat_minute]
#define CMD_SET_DATE			0xA8	//3 data bytes; [day][hour][minute]; 24 hour time format;
										//day codes from 0(sunday) to 6(saturday)

#define LED_CHECK_ROBOT_RED			0x08
#define LED_DOCK_GREEN				0x04
#define LED_SPOT_GREEN				0x02
#define LED_DIRT_DETECT_BLUE		0x01

#define LED_SCHED_WDAY_SAT	0x40
#define LED_SCHED_WDAY_FRI	0x20
#define LED_SCHED_WDAY_THU	0x10
#define LED_SCHED_WDAY_WED	0x08
#define LED_SCHED_WDAY_TUE	0x04
#define LED_SCHED_WDAY_MON	0x02
#define LED_SCHED_WDAY_SUN	0x01

#define LED_SCHED_SCHEDULE	0x10
#define LED_SCHED_CLOCK		0x08
#define LED_SCHED_AM		0x04
#define LED_SCHED_PM		0x02
#define LED_SCHED_COLON		0x01

#define BTN_PACKET_ID		0x12
#define CMD_BUTTONS_STATE 	0x8E
#define BTN_CLOCK		0x80
#define BTN_SCHEDULE		0x40
#define BTN_DAY			0x20
#define BTN_HOUR		0x10
#define BTN_MINUTE		0x08
#define BTN_DOCK		0x04
#define BTN_SPOT		0x02
#define BTN_CLEAN		0x01

#define CMD_QUERY_SENSOR	0x8E

#define SENSORS_ALL						0x00
#define SENSORS_PHYSICAL				0x01
#define SENSORS_BUTTONS_AND_INTERNAL	0x02
#define SENSORS_POWER					0x03

#define DRIVE_ON_CHARGER_FORCEFIELD 0xA1
#define DRIVE_ON_CHARGER_GREENBUOY 0xA4
#define DRIVE_ON_CHARGER_REDBUOY 0xA8
#define DRIVE_ON_CHARGER_ALL 0xAD

#define CHARGING_SOURCE_HOMEBASE 0x2

#define DIGIT_LENGTH 4
#define ASCII_PLUS 43
#define ASCII_MINUS 45
#define ASCII_NUMBER_START 48

#define DEFAULT_VELOCITY 200


/******************************************************************* Structures */

/**
 * A structure for roomba serial communication packets
 */
typedef struct {
	uint8_t id; /**< The packet id */
	uint8_t length; /**< the length of the returned value (in bytes) */
	uint8_t has_sign; /**< flag to specify if the returned sensor value is sigend */
} packet;

extern int32_t roomba_sevenseg_digits[DIGIT_LENGTH];

/**
 * A structure which holds important data of the roomba robot
 */
typedef struct {
	uint8_t is_moving; /**< flag to specify if the roomba is moving(e.g. drive, turn) or not */
	uint8_t current_base_id; /**< the id of the base at which the roomba is docked */
	uint8_t destination_base_id; /**< the id of the base to which the roomba should drive */
	int16_t current_velocity; /**< the current velocity of the roomba (0 if not moving) */
	int32_t driven_distance; /**< the whole driven distance since the program was started */
	int32_t trip_distance; /**< the driven distance since trip meters were reset */
	int32_t trip_angle; /**< the driven angle since trip meters were reset */
	int32_t angle_360_degrees; /**< the calibrated angle value which corresponds 360 degrees */
	int32_t distance_1_meter; /**< the calibrated distance value which corresponds 1000 millimeters (1 meter) */
} roomba_data;

/**
 * A structure which holds all data to handle a collision
 */
typedef struct {
	int32_t angle_sum; /**< the sum of turned angles since collision detection (in degrees) */
	int32_t distance_sum; /**< the sum of driven distances since collision detection (in millimeters) */
	int32_t driven_trip_distance; /**< calculated distance value needed to evade collision object */
	int32_t bumper_state; /**< the last known state of the physical bumpers */
	int32_t light_bumper_state; /**< the last known state of the light bumpers */
	int32_t program_tick_counter; /**< counts the program iterations in collision mode  */
	int8_t played_acustic_feedback; /**< flag to specify if a acoustic feedback was played for entering collision mode */
	int32_t trip_distance_at_collision; /**< backup of the roomba's trip distance value at the moment of collision detection  */
	int32_t trip_angle_at_collision; /**< backup of the roomba's trip angle value at the moment of collision detection  */
} collision_data;

/******************************************************* Function prototypes */
/**
 * \brief  init roomba function
 *
 *	This function initializes the roomba robot
 *	to full mode
 *
 */
void roombaInit(void);

/**
 * \brief  calibrate angle function
 *
 *	This function can be used to calibrate a angle of 360 degrees
 *
 */
void roombaCalibrateAngle(void);

/**
 * \brief  calibrate distance function
 *
 *	This function can be used to calibrate a distance of 1 meter
 *
 */
void roombaCalibrateDistance(void);

/**
 * \brief  query sensor function
 *
 *	This function queries the specified sensor
 * \param	query_packet the packet which represents the sensor to query
 * \return the queried sensor value, converted to a 32-bit integer
 */
int32_t roombaQuerySensor(packet query_packet);

/**
 * \brief  angle convert function
 *
 *	This function converts a raw angle value to the corresponding calibrated value
 *
 * \param	angle_raw	the raw angle value as queried from roomba's sensor
 * \return the converted angle value
 */
int32_t roombaAsCalibratedAngle(int32_t angle_raw);

/**
 * \brief  distance convert function
 *
 *	This function converts a raw distance value to the corresponding calibrated value
 * \param	distance_raw	the raw distance value as queried from roomba's sensor
 * \return the converted distance value
 */
int32_t roombaAsCalibratedDistance(int32_t distance_raw);

/**
 * \brief  seekdock function
 *
 *	This function executes roomba's built-in seekdock algorithm
 *
 */
void roombaSeekdock(void);

/**
 * \brief  reset roomba's trip meters
 *
 *	This function consumes the angle and distance sensor values and resets angle and distance trip meters
 *
 */
void roombaResetTrips(void);

/**
 * \brief drive backward function
 *
 *	This function can be used to drive a bit backward
 * \param	backward_distance	the distance to drive backward (in milimeters)
 *
 */
void roombaDriveABitBackward(int32_t backward_distance);

/**
 * \brief collision detected function
 *
 *	This function should be called when a collision is detected either by light bumps or physical bumps
 * \param	light_bumper_state	the state of the light_bumpers at the moment of collision
 * \param	bumper_state	the state of the physical bumpers at the moment of collision
 *
 */
void roombaOnCollisionDetected(int32_t bumper_state,
		int32_t light_bumper_state);
/**
 * \brief  collision cleared function
 *
 *	This function should be called if a previously detected collision has been cleared
 *
 */
void roombaOnCollisionCleared(void);
/**
 * \brief  play song theme function
 *
 *	This function plays the zelda theme
 *
 */
void roombaPlaySongTheme(void);
/**
 * \brief  play song collision function
 *
 *	This function plays a warning sound, which is used when a collision is detected
 *
 */
void roombaPlaySongCollision(void);
/**
 * \brief  play song done function
 *
 *	This function plays a positive acoustic feedback, which can be used for any type of success
 *
 */
void roombaPlaySongDone(void);
/**
 * \brief  play song beep function
 *
 *	This function plays a neutral beep sound
 *
 */
void roombaPlaySongBeep(void);

/**
 * \brief  Write to roomba's weekday LEDs
 *
 *	This function controls the weekday LEDs of the roomba robot
 *
 * \param      led_mask	the bit mask to be transfered to roomba
 *
 */
void roombaSetWeekdayLed(uint8_t led_mask);

/**
 * \brief  Write to roomba's LEDs
 *
 *	This function controls the LEDs of the roomba robot
 *
 * \param      led_mask	the bit mask to be transfered to roomba
 * \param	color		the color value to be transfered to roomba
 * \param	intensity	the intensity value to be transfered to roomba
 *
 */
void roombaSetLed(uint8_t led_mask, uint8_t color, uint8_t intensity);

/**
 * \brief  Write the ascii digits
 *
 *	Write the global ascii digits array to roomba's seven seg display with this function
 *
 */
void roombaWriteSevensegDigits(void);

/**
 * \brief  Drive function
 *
 *	Let the roomba drive with the specified 'velocity' by calling this function
 *
 * \param      velocity	the velocity to drive
 *
 */
void roombaDrive(int16_t velocity, int16_t radius);

/**
 * \brief  Stop function
 *
 *	Stop the (driving) roomba with this function
 *
 */
void roombaStop(void);

/**
 * \brief  Check the roomba buttons
 *
 *	Request the state of the roomba buttons with this function
 *
 * \return                 an unsigned 8-bit integer value which represents the current state of the roomba buttons
 *
 */
uint8_t roombaCheckButton(void);

/**
 * \brief  Calculate the time to drive function
 *
 *         Use this function to calculate the time to drive 'distance' centimeters with the specified 'velocity'
 *
 * \param   distance 	the distance to drive
 * \param 	velocity	the velocity to drive
 * \return the calculated time, which is needed to drive 'distance' with 'velocity'
 *
 */
int32_t roombaCalculateTimeToDrive(int32_t distance, int32_t velocity);

/************************************************************** Global const */
//packet struct variables for all sensors
extern const packet PACKET_BUMPS_WHEELDROPS;
extern const packet PACKET_WALL;
extern const packet PACKET_CLIFF_LEFT;
extern const packet PACKET_CLIFF_FRONT_LEFT;
extern const packet PACKET_CLIFF_FRONT_RIGHT;
extern const packet PACKET_CLIFF_RIGHT;
extern const packet PACKET_VIRTUAL_WALL;
extern const packet PACKET_WHEEL_OVERCURRENTS;
extern const packet PACKET_DIRT_DETECT;
extern const packet PACKET_UNUSED_BYTE;
extern const packet PACKET_INFRARED_CHARACTER_OMNI;
extern const packet PACKET_BUTTONS;
extern const packet PACKET_DISTANCE;
extern const packet PACKET_ANGLE;
extern const packet PACKET_CHARGING_STATE;
extern const packet PACKET_VOLTAGE;
extern const packet PACKET_CURRENT;
extern const packet PACKET_TEMPERATURE;
extern const packet PACKET_BATTERY_CHARGE;
extern const packet PACKET_BATTERY_CAPICITY;
extern const packet PACKET_WALL_SIGNAL;
extern const packet PACKET_CLIFF_LEFT_SIGNAL;
extern const packet PACKET_CLIFF_FRONT_LEFT_SIGNAL;
extern const packet PACKET_CLIFF_FRONT_RIGHT_SIGNAL;
extern const packet PACKET_CLIFF_RIGHT_SIGNAL;
extern const packet PACKET_CHARGING_SOURCES_AVAILABLE;
extern const packet PACKET_OI_MODE;
extern const packet PACKET_SONG_NUMBER;
extern const packet PACKET_SONG_PLAYING;
extern const packet PACKET_NUMBER_OF_STREAM_PACKETS;
extern const packet PACKET_REQUESTED_VELOCITY;
extern const packet REQUESTED_RADIUS;
extern const packet PACKET_REQUESTED_RIGHT_VELOCITY;
extern const packet PACKET_REQUESTED_LEFT_VELOCITY;
extern const packet PACKET_RIGHT_ENCODER_COUNTS;
extern const packet PACKET_LEFT_ENCODER_COUNTS;
extern const packet PACKET_LIGHT_BUMPER;
extern const packet PACKET_LIGHT_BUMP_LEFT_SIGNAL;
extern const packet PACKET_LIGHT_BUMP_FRONT_LEFT_SIGNAL;
extern const packet PACKET_LIGHT_BUMP_CENTER_LEFT_SIGNAL;
extern const packet PACKET_LIGHT_BUMP_CENTER_RIGHT_SIGNAL;
extern const packet PACKET_LIGHT_BUMP_FRONT_RIGHT_SIGNAL;
extern const packet PACKET_LIGHT_BUMP_RIGHT_SIGNAL;
extern const packet PACKET_INFRARED_CHARACTER_LEFT;
extern const packet PACKET_INFRARED_CHARACTER_RIGHT;
extern const packet PACKET_LEFT_MOTOR_CURRENT;
extern const packet PACKET_RIGHT_MOTOR_CURRENT;
extern const packet PACKET_MAIN_BRUSH_MOTOR_CURRENT;
extern const packet PACKET_SIDE_BRUSH_MOTOR_CURRENT;
extern const packet PACKET_STASIS;

//roomba_data struct variable
extern roomba_data roombadata;

//collision_data struct variable
extern collision_data collisiondata;

/********************************************************** Global variables */

/** endif _ROOMBA_H **/
#endif

