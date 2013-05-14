/******************************************************************************

File: square.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel

Created: 09.05.2013

*******************************************************************************/

#ifndef SQUARE_H
#define SQUARE_H

#include <stdint.h>
#include "tools.h"

/*!
 *	\brief		The main function for the "drive a square" program
 *				Contains main loop; no need for other function calls after this one
 */
void square_main();



/*!
 *	\brief		Bring the Roomba into operational state
 */
void initRoomba();

/*!
 *	\brief		Setup the query_IDs array with the appropriate IDs for this program
 */
void setupQueryIDs ();

/*!
 *	\brief		Reset the state machine into it's initial state; for first run and reset
 */
void initProgram();



/*!
 *	\brief		Handles the "angle calibration" mode of the initialization process
 */
void handleAngleCalibration();

/*!
 *	\brief		Handles the "squareside setup" mode of the initialization process
 */
void handleSquaresideSetup();

/*!
 *	\brief		Handles the "velocity setup" mode of the initialization process
 */
void handleVelocitySetup();

/*!
 *	\brief		Handles the "drive straight" mode of the driving program
 */
void handleDriveStraightMode();

/*!
 *	\brief		Handles the "turn" mode of the driving program
 */
void handleTurnMode();

/*!
 *	\brief		Handles the "stop" mode of the driving program
 */
void handleStopMode();

/*!
 *	\brief		Handles collisions in the driving program
 */
void handleCollision();



/*!
 *	\brief		Retrieves the current values of the drive parameters from the Roomba
 */
void retrieveDriveData();

/*!
 *	\brief		Checks if the Roomba has driven far enough in "drive straight" mode
 */
bool_t checkDriveProgress();

/*!
 *	\brief		Checks if the Roomba has turned far enough in "turn" mode
 */
bool_t checkTurnProgress();

#endif // #ifndef SQUARE_H
