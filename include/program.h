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


enum programstate { INIT, CALIBRATE, DRIVE, COLLISION, SEEKDOCK, DOCKED};
enum drivestate {ANGLE_APPROACH, LINE_APPROACH, FENCE_APPROACH, LEAVE_DOCK};
enum angleapproachstate {DRIVE_ANGLE, DRIVE_DISTANCE};
enum calibratestate {DISTANCE, ANGLE};
//enum seekdockstate {FORCEFIELD, GREENBUOY, REDBUOY, GREENBUOY_AND_FORCEFIELD, REDBUOY_AND_FORCEFIELD, GREENBUOY_AND_REDBUOY, GREENBUOY_REDBUOY_AND_FORCEFIELD};


/******************************************************* Function prototypes */


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
void handleStateInit();

/**
  * \brief	Handle DRIVE state
  *
  *         this function handles the DRIVE programm state
  *
  */
void handleStateDrive();

/**
  * \brief	Handle COLLISION state
  *
  *         this function handles the COLLISION programm state
  *
  */
void handleStateCollision();


/**
  * \brief	Handle CALIBRATE state
  *
  *         this function handles the CALIBRATE programm state
  *
  */
void handleStateCalibrate();


void handleStateSeekdock();

void handleStateDocked();

void handleSubStateAngleApproach();

void handleSubStateLineApproach();

void handleSubStateFenceApproach();





/************************************************************** Global const */


/********************************************************** Global variables */








/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */


/*********************************************************** Local functions */


#endif



