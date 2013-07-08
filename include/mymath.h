#ifndef _MYMATH_H
#define _MYMATH_H


/****************************************************************** Includes */

#include "tools.h"

/******************************************************************* Defines */




#define MATH_PI 3.14159265358979323846264338327950288
#define MATH_PI2 1.57079632679489661923132169163975144
#define MATH_PI3 1.04719755119659774615421446109316763
#define MATH_PI6 0.52359877559829887307710723554658381

#define POLYNOM1(x, a)  ((a)[1]*(x)+(a)[0])
#define POLYNOM2(x, a)  (POLYNOM1((x),(a)+1)*(x)+(a)[0])
#define POLYNOM3(x, a)  (POLYNOM2((x),(a)+1)*(x)+(a)[0])
#define POLYNOM4(x, a)  (POLYNOM3((x),(a)+1)*(x)+(a)[0])



/******************************************************* Function prototypes */

float my_atan(float x);
float my_atan2(float y, float x);
float radToDeg(float value);
float degToRad(float value);
float my_square_root(float value);

int myAbs (int value);
bool_t mySign (int value);


#endif
