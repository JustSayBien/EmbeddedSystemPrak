#ifndef _PROGRAM_H
#define _PROGRAM_H

/****************************************************************** Includes */

#include "board.h"
#include "led.h"
#include "pio.h"
#include "roomba.h"
#include "tools.h"
#include "uart.h"

/******************************************************************* Defines */


/******************************************************************* Type definitions */


/** An enum for the main program state machine */
enum programstate {
	CALIBRATE, DRIVE, COLLISION, SEEKDOCK, DOCKED
};

/** An enum for the base configuration state machine */
enum base_config_states {
	BASE_SELECT = 1, BASE_COORDINATE_X, BASE_COORDINATE_Y, BASE_ROTATION
};

/** An enum for the DRIVE state machine */
enum drivestate {
	ANGLE_APPROACH, LINE_APPROACH, FENCE_APPROACH, LEAVE_DOCK
};

/** An enum for the sub state ANGLE_APPROACH  */
enum angleapproachstate {
	DRIVE_ANGLE, DRIVE_DISTANCE
};

/** An enum for the sub state LINE_APPROACH  */
enum lineapproachstate {
	LINE_TURN_FROM_BASE, LINE_DRIVE
};

/** An enum for the sub state FENCE_APPROACH  */
enum fenceapproachstate {
	FENCE_ANGLE,
	FENCE_STRAIGHT,
	FENCE_CORRECTION_STRAIGHT,
	FENCE_CORRECTION_ANGLE
};

/** An enum for the collision state machine  */
enum collisionstate {
	COLLISION_TURN, COLLISION_DRIVE
};

/** An enum for the calibrate state machine  */
enum calibratestate {
	CALIBRATE_DISTANCE, CALIBRATE_ANGLE, CALIBRATE_BASE, CALIBRATE_DONE
};

/** An enum for the sub state next base  */
enum nextbasestate {
	NEXTBASE_DRIVE, NEXTBASE_NUM, NEXTBASE_APPROACH
};

/** milliseconds definition  */
typedef unsigned long millis_t;

/** An enum for direction  */
typedef enum {
	LEFT = 1, STRAIGHT = 0, RIGHT = -1
} direction;



/********************************************************** Global variables */

/** Global clock; incremented in every iteration of the main loop */
extern millis_t global_clock;

/** Current state of the base configuration module */
extern uint8_t base_config_state;

/** stores the queried ir button state */
extern volatile int32_t ir_action;




/******************************************************* Function prototypes */

/**
 * \brief  Main program loop
 *
 *         The main program loop function
 */
void programRun(void);

/**
 * \brief  Convert int to ascii
 *
 *         Convert an integer value to an array of ascii digits with function
 *
 * \param    value	 the integer value to convert
 * \param 	out		the result array, containing the ascii digits
 * \return	1 if the integer value is smaller than 10000, -1 otherwise
 */

int8_t intToAscii(int32_t value, int32_t out[]);

/**
 * \brief  Set program state function
 *
 *         Set the program state according to the state chart with this function
 *
 * \param      state	 	the state to set
 */
void setProgramState(enum programstate state);

/**
 * \brief	Handle DRIVE state
 *
 *         this function handles the DRIVE program state
 * \return the main program state for the next iteration (can be CALIBRATE, DRIVE,COLLISION, SEEKDOCK, DOCKED)
 */
enum programstate handleStateDrive();

/**
 * \brief	Handle COLLISION state
 *
 *         this function handles the COLLISION program state
 * \return the main program state for the next iteration (can be CALIBRATE, DRIVE,COLLISION, SEEKDOCK, DOCKED)
 */
enum programstate handleStateCollision();

/**
 * \brief	Handle CALIBRATE state
 *
 *         this function handles the CALIBRATE program state
 * \return the main program state for the next iteration (can be CALIBRATE, DRIVE,COLLISION, SEEKDOCK, DOCKED)
 */
enum programstate handleStateCalibrate();

/**
 * \brief	Handle SEEKDOCK state
 *
 *         this function handles the SEEKDOCK program state
 * \return the main program state for the next iteration (can be CALIBRATE, DRIVE,COLLISION, SEEKDOCK, DOCKED)
 */
enum programstate handleStateSeekdock();


/**
 * \brief	Handle DOCKED state
 *
 *         this function handles the DOCKED program state
 * \return the main program state for the next iteration (can be CALIBRATE, DRIVE,COLLISION, SEEKDOCK, DOCKED)
 */
enum programstate handleStateDocked();

/**
 * \brief	Handle sub-state ANGLE_APPROACH
 *
 *         this function handles the drive state ANGLE_APPROACH
 * \return the main program state for the next iteration (can be CALIBRATE, DRIVE,COLLISION, SEEKDOCK, DOCKED)
 */
enum programstate handleSubStateAngleApproach();


/**
 * \brief	Handle sub-state LEAVEDOCK
 *
 *         this function handles the drive state LEAVEDOCK
 * \return the main program state for the next iteration (can be CALIBRATE, DRIVE,COLLISION, SEEKDOCK, DOCKED)
 */
enum programstate handleSubStateLeaveDock();

/**
 * \brief	Handle sub-state LINE_APPROACH
 *
 *         this function handles the drive state LINE_APPROACH
 * \return the main program state for the next iteration (can be CALIBRATE, DRIVE,COLLISION, SEEKDOCK, DOCKED)
 */
enum programstate handleSubStateLineApproach();


/**
 * \brief	Handle sub-state FENCE_APPROACH
 *
 *         this function handles the drive state FENCE_APPROACH
 * \return the main program state for the next iteration (can be CALIBRATE, DRIVE,COLLISION, SEEKDOCK, DOCKED)
 */
enum programstate handleSubStateFenceApproach();

/************************************************************** Global const */



/*************************************************************** Local const */

/*********************************************************** Local variables */

/******************************************************************** Macros */

/********************************************************** Global functions */

/*********************************************************** Local functions */

#endif

