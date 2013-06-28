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


enum programstate { INIT, CALIBRATE, DRIVE, COLLISION, SEEKDOCK, DOCKED };
enum base_config_states { BASE_SELECT = 1, BASE_COORDINATE_X, BASE_COORDINATE_Y, BASE_ROTATION };
enum drivestate { ANGLE_APPROACH, LINE_APPROACH, FENCE_APPROACH, LEAVE_DOCK };
enum angleapproachstate { DRIVE_ANGLE, DRIVE_DISTANCE };
enum fenceapproachstate { STRAIGHT, CORRECTION };
enum collisionstate { COLLISION_TURN, COLLISION_DRIVE };
enum calibratestate { DISTANCE, ANGLE, BASE, START };

typedef unsigned long millis_t;



/******************************************************* Function prototypes */


/** Global clock; incremented in every iteration of the main loop */
extern millis_t global_clock;

/** Current state of the base configuration module */
extern uint8_t base_config_state;


void program_run();


/**
  * \brief  Convert int to ascii 
  *
  *         Convert an integer value to an array of ascii roomba_sevenseg_digits with function
  *
  * \param      value	 	the integer value to convert
  * \param 	out		the result array, containing the ascii roomba_sevenseg_digits
  * \return	1 if the integer value is smaller than 10000, -1 otherwise
  */
int intToAscii(int32_t value, int32_t out[]);


/**
  * \brief  Set program state 
  *
  *         Set the program state according to the state chart with this function
  *
  * \param      state	 	the state to set
  */
void setProgramState(enum programstate state);

/**
  * \brief	Handle INIT state
  *
  *         this function handles the INIT programm state
  *
  */
enum programstate handleStateInit();

/**
  * \brief	Handle DRIVE state
  *
  *         this function handles the DRIVE programm state
  *
  */
enum programstate handleStateDrive();

/**
  * \brief	Handle COLLISION state
  *
  *         this function handles the COLLISION programm state
  *
  */
enum programstate handleStateCollision();


/**
  * \brief	Handle CALIBRATE state
  *
  *         this function handles the CALIBRATE programm state
  *
  */
enum programstate handleStateCalibrate();


enum programstate handleStateSeekdock();

enum programstate handleStateDocked();

enum programstate handleSubStateAngleApproach();

enum programstate handleSubStateLeaveDock();

enum programstate handleSubStateLineApproach();

enum programstate handleSubStateFenceApproach();





/************************************************************** Global const */


/********************************************************** Global variables */








/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */


/*********************************************************** Local functions */


#endif



