/******************************************************************************

File: de270util.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel

Created: 09.05.2013

*******************************************************************************/

#include <stdint.h>

#ifndef DE270UTIL_H
#define DE270UTIL_H

extern int de270_led_status;

/*!
 *	\brief		Use six of the eight LEDs of the DE2-70 board as an incremental counter (in reverse order; LEDs 1 and 0 are left untouched)
 *	
 *	\param		value		the increment to display (0-6)
 */
void de270_LEDCounter (uint8_t value);

#endif // #ifndef DE270_H
