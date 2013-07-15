/******************************************************************************

File: ir_remote.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel
        Universität Augsburg

Created: 28.06.2013


******************************************************************************/

/************************************************************* Include Guard */

#ifndef _IR_REMOTE_H
#define _IR_REMOTE_H

/****************************************************************** Includes */

#include "program.h"
#include "workbench.h"
#include "tools.h"
#include <stdint.h>


/******************************************************************* Defines */

#define NUM_ROOMBA_REMOTE_BUTTONS		14

#define ROOMBA_REMOTE_MIN_REPEAT		0
#define ROOMBA_REMOTE_SHORT_REPEAT		1
#define ROOMBA_REMOTE_NORMAL_REPEAT		2
#define ROOMBA_REMOTE_LONG_REPEAT		3
#define ROOMBA_REMOTE_MAX_REPEAT		4

/** An enum for the possible roomba ir remote buttons */
enum roomba_remote_actions {
	ROOMBA_REMOTE_CROSS_UP = 		0x85,
	ROOMBA_REMOTE_CROSS_DOWN = 		0x86,
	ROOMBA_REMOTE_CROSS_RIGHT = 		0x83,
	ROOMBA_REMOTE_CROSS_LEFT = 		0x81,
	ROOMBA_REMOTE_CROSS_OK = 		0x82,
	
	ROOMBA_REMOTE_BACK = 			0x8D,
	ROOMBA_REMOTE_PAUSE = 			0x89,
	ROOMBA_REMOTE_POWER = 			0x8A,

	ROOMBA_REMOTE_NUM_1 = 			0x8C,
	ROOMBA_REMOTE_NUM_2 = 			0x8B,
	ROOMBA_REMOTE_NUM_3 = 			0x84,
	ROOMBA_REMOTE_NUM_4 = 			0x87,
	ROOMBA_REMOTE_NUM_5 = 			0x88,
	ROOMBA_REMOTE_NUM_6 = 			0x8F
};

/** An enum for array indices of the possible roomba ir remote buttons */
enum roomba_action_indices {
	ROOMBA_REMOTE_CROSS_UP_INDEX =		0,
	ROOMBA_REMOTE_CROSS_DOWN_INDEX,
	ROOMBA_REMOTE_CROSS_RIGHT_INDEX,
	ROOMBA_REMOTE_CROSS_LEFT_INDEX,
	ROOMBA_REMOTE_CROSS_OK_INDEX,

	ROOMBA_REMOTE_BACK_INDEX,
	ROOMBA_REMOTE_PAUSE_INDEX,
	ROOMBA_REMOTE_POWER_INDEX,

	ROOMBA_REMOTE_NUM_1_INDEX,
	ROOMBA_REMOTE_NUM_2_INDEX,
	ROOMBA_REMOTE_NUM_3_INDEX,
	ROOMBA_REMOTE_NUM_4_INDEX,
	ROOMBA_REMOTE_NUM_5_INDEX,
	ROOMBA_REMOTE_NUM_6_INDEX
};

/** An array for the times of the first remote button presses */
extern millis_t roomba_remote_activation_times[NUM_ROOMBA_REMOTE_BUTTONS];

/** Stores the raw received IR data */
extern uint8_t raw_ir_data;


/******************************************************* Function prototypes */

/**
 * \brief  Setup the base coordinates and angles
 *
 *
 * \return	the appropriate main program state
 */
enum programstate handleSubstateBaseSetup ();

/**
 * \brief  Convert int to ascii
 *
 *         Convert an integer value to an array of ascii digits (2 places int with sign)
 *
 * \param    	value	the integer value to convert
 * \param 	out	the result array, containing the ascii digits
 * \return	success
 */
bool_t intToAscii2placesSigned (int32_t value, int32_t out[]);

/**
 * \brief  Convert int to ascii
 *
 *         Convert an integer value to an array of ascii digits (3 places int without sign)
 *
 * \param    	value	the integer value to convert
 * \param 	out	the result array, containing the ascii digits
 * \return	success
 */
bool_t intToAscii3placesUnsigned (uint32_t value, int32_t out[]);

/**
 * \brief  Get the current IR remote action
 *
 *         Handles sensor querying, index lookup, ensured recognition of IR remote commands (eliminates accidental recognition) and repeat timeouts for button press repetition
 *
 * \return	the ensured and repeat handled IR remote button code
 */
int getIRAction ();

/**
 * \brief  Get the array index for an IR remote button
 *
 *         Get the array index of a specified IR remote button
 *
 * \param    	character	the character to look up
 * \return	the array index of the specified character
 */
int getIRActionIndex (int character);

/**
 * \brief  Print the current x coordinate to the roomba's display
 *
 *         Print the x coordinate of the specified workbench to the roomba's seven segment display
 *
 * \param    	toPrint		the workbench of which to print the x coordinate
 */
void printXcoordinate (workbench *toPrint);

/**
 * \brief  Print the current y coordinate to the roomba's display
 *
 *         Print the y coordinate of the specified workbench to the roomba's seven segment display
 *
 * \param    	toPrint		the workbench of which to print the y coordinate
 */
void printYcoordinate (workbench *toPrint);

/**
 * \brief  Print the current angle to the roomba's display
 *
 *         Print the angle of the specified workbench to the roomba's seven segment display
 *
 * \param    	toPrint		the workbench of which to print the angle
 */
void printRotation (workbench *toPrint);

/**
 * \brief  Print the specified coordinate to the roomba's display (max. 2 digits, signed)
 *
 *         Print the specified coordinate to the roomba's display (max. 2 digits, signed)
 *
 * \param    	coordinateValue		the coordinate value to print to the roomba's display
 */
void printCoordinate(int8_t coordinate_value);

/**
 * \brief  Sets the current workbench pointer to the workbench with the specified index
 *
 *         Sets the current workbench pointer to the workbench with the specified index (between 1 and MAX_COUNT_WORKBENCHES)
 *
 * \param    	base_num		the id of the base to work with from now on
 */
void handleBaseSelect (uint8_t base_num);

/**
 * \brief  Handles discrete IR remote button recognition
 *
 *         Handles ensured recognition of IR remote commands (eliminates accidental recognition) and repeat timeouts for button press repetition
 *
 * \param    	index		the index of the IR remote button
 */
bool_t checkDiscreteRoombaButtonArray (int index);

#endif  // _IR_REMOTE_H
