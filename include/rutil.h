/******************************************************************************

File: rutil.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel

Created: 09.05.2013

*******************************************************************************/

#ifndef RUTIL_H
#define RUTIL_H

#include <stdint.h>
#include "tools.h"
#include "roomba.h"

enum roomba_buttons {
	ROOMBA_BUTTON_DAY,
	ROOMBA_BUTTON_HOUR,
	ROOMBA_BUTTON_MINUTE,
	ROOMBA_BUTTON_CLEAN,
	ROOMBA_BUTTON_SPOT,
	ROOMBA_BUTTON_DOCK
};

extern bool_t roomba_button_pressed[6];		///< Temporary array for the pressed state of the Roomba's buttons
extern bool_t BUTTON_WAIT_RELEASE[6];

extern uint8_t roomba_digits[4];				///< Temporary array for the values of the Roomba's display
extern int roomba_led_status;					///< Temporary variable for the status of the Roomba's LEDs
extern int roomba_power_led_color_status;		///< Temporary variable for the color status of the Roomba's power LED
extern int roomba_power_led_intensity_status;	///< Temporary variable for the intensity status of the Roomba's power LED

extern uint8_t roomba_leds[4];

/*!
 *	\brief		Encapsulates calls to both Roomba_Roomba_clearDigitsSilent() and Roomba_commitDigits();
 */
void Roomba_clearDigits();

/*!
 *	\brief		Clears the temporary variables for the digits of the Roomba's display
 */
void Roomba_clearDigitsSilent();

/*!
 *	\brief		Write the current digit values to the Roomba's serial interface
 */
void Roomba_commitDigits();

/*!
 *	\brief		Write the current LED values to the Roomba's serial interface
 */
void Roomba_commitLEDs();

/*!
 *	\brief		Write the specified weekday LED value to the Roomba's serial interface
 *	
 *	\param		value		the value to send to the Roomba
 */
void Roomba_commitWeekdayLEDs(int value);

/*!
 *	\brief		Drive the Roomba with the specified velocity, where negative velocity is forward motion
 *	
 *	\param		velocity		the velocity to drive the Roomba with
 */
void Roomba_drive(int16_t velocity);

/*!
 *	\brief		Turn the Roomba with the specified velocity
 *	
 *	\param		velocity		the velocity to drive the Roomba with
 *	\param		radius			the radius to turn around
 */
void Roomba_turn(int16_t velocity, int16_t radius);

/*!
 *	\brief		Stop the Roomba; encapsulates a call to drive() with velocity 0
 */
void Roomba_stop();

/*!
 *	\brief		Use the four LEDs of the Roomba as an incremental counter
 *	
 *	\param		value		the increment to display (0-4)
 */
void Roomba_LEDCounter(uint8_t value);

/*!
 *	\brief		Write a query list to the Roomba and read the response back
 *	
 *	\param		length		the number of values to send and receive
 *	\param		query		the array of pointers to query packet ids to send to the Roomba
 *	\param		result		the array to store the received values in
 */
void Roomba_queryList(uint8_t length, const packet *query[], int result[]);

/*!
 *	\brief		Write a query list to the Roomba
 *	
 *	\param		length		the number of values to send and receive
 *	\param		query		the array of pointers to query packet ids to send to the Roomba
 */
void Roomba_writeQueryList(uint8_t length, const packet *query[]);

/*!
 *	\brief		Read the response to a query list back from the Roomba
 *	
 *	\param		length		the number of values to receive from the Roomba
 *	\param		name		description
 *	\param		name		description
 *
 *	\return		return
 */
void Roomba_readQueryList(uint8_t length, const packet *query[], int out[]);

/*!
 *	\brief		Read out the current pressed state of the Roomba's button and store it in the roomba_button_pressed array
 */
void Roomba_refreshButtons();

#endif // #ifndef RUTIL_H
