/******************************************************************************

File: de270util.c

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel

Created: 09.05.2013

*******************************************************************************/

#include "de270util.h"
#include "led.h"

int de270_led_status = 0;

void de270_LEDCounter (uint8_t value) {
	if (value > 4)
		value = 4;
	de270_led_status &= (led_vals[0] | led_vals[1]);
	int i;
	for (i=0; i<value; i++) {
		de270_led_status |= led_vals[7-i];
	}
	LED = de270_led_status;
}
