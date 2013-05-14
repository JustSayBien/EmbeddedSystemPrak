/******************************************************************************

File: util.c

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel

Created: 09.05.2013

*******************************************************************************/

#include "util.h"
#include "mymath.h"

void intToASCII (int value, uint8_t out[]) {
	uint8_t sign = (value < 0) && (value != 0);
	value = myAbs(value);
	int counter = 1;
	int old_place = 1;
	int place = 10;
	do {
		place = myPow(10, counter);
		out[counter-1] = ((value % place) / old_place) + 48;
		old_place = place;
		counter++;
	} while (value >= place);
	out[counter-1] = sign ? 45 : 32;
	while (counter < 4) {
		counter++;
		out[counter-1] = 32;
	}
}
