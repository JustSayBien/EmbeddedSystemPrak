/******************************************************************************

File: mymath.h

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel

Created: 09.05.2013

*******************************************************************************/

#include "mymath.h"

int32_t myAbs (int32_t value) {
	return ((value < 0) ? -value : value);
}

int myPow (int base, unsigned int exponent) {
	int pow = 1;
	int i;
	for (i=0; i<exponent; i++) {
		pow *= base;
	}
	return pow;
}

int32_t calcDriveTime (int32_t distance, int32_t velocity) {
	return ((int32_t)(distance/velocity))*1000;
}
