/******************************************************************************

File: mymath.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel

Created: 09.05.2013

*******************************************************************************/

#ifndef MYMATH_H
#define MYMATH_H

#include <stdint.h>

/*
 *	\brief		Calculates the absolute value of a given number
 *	
 *	\param		value		the value to absolutize
 *
 *	\return		the absolutized value
 */
int32_t myAbs(int32_t value);

/*
 *	\brief		Calculates the n-th power of a number
 *	
 *	\param		base		the base value
 *	\param		power		the power value
 *	
 *	\return		the n-th power
 */
int myPow(int base, unsigned int exponent);

/*
 *	\brief		Calculates the estimated time needed to Roomba_drive the given distance at a certain speed
 *	
 *	\param		distance		the distance to Roomba_drive
 *	\param		velocity		the velocity at which the object travels
 *
 *	\return		the estimated time needed
 */
int32_t calcDriveTime (int32_t distance, int32_t velocity);

#endif // #ifndef MYMATH_H
