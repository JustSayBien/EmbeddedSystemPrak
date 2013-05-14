/******************************************************************************

File: util.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel

Created: 09.05.2013

*******************************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

/*!
 *	\brief		Converts an integer value to an ASCII character array
 *	
 *	\param		value		the value to convert
 *	\param		out			the array to save the ASCII characters to
 *	
 *	\bug		Currently only works with a maximum of 4 characters in length
 */
void intToASCII (int value, uint8_t out[]);

#endif // #ifndef UTIL_H
