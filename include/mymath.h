#ifndef _MYMATH_H
#define _MYMATH_H


/****************************************************************** Includes */

#include "uart.h"
#include "tools.h"

/******************************************************************* Defines */




#define MATH_PI 3.14159265358979323846264338327950288
#define MATH_PI2 1.57079632679489661923132169163975144
#define MATH_PI3 1.04719755119659774615421446109316763
#define MATH_PI6 0.52359877559829887307710723554658381

#define MATH_1_PI   0.31830988618379067153776752674502872 

#define POLYNOM1(x, a)  ((a)[1]*(x)+(a)[0])
#define POLYNOM2(x, a)  (POLYNOM1((x),(a)+1)*(x)+(a)[0])
#define POLYNOM3(x, a)  (POLYNOM2((x),(a)+1)*(x)+(a)[0])
#define POLYNOM4(x, a)  (POLYNOM3((x),(a)+1)*(x)+(a)[0])
#define POLYNOM5(x, a)	(POLYNOM4((x),(a)+1)*(x)+(a)[0])
#define POLYNOM6(x, a)	(POLYNOM5((x),(a)+1)*(x)+(a)[0])
#define POLYNOM7(x, a)	(POLYNOM6((x),(a)+1)*(x)+(a)[0])

#define FLOAT_MAX	3.40282347e+38F


#define F64_SIGN_SHIFT  31
#define F64_SIGN_MASK   1
#define F64_EXP_SHIFT   20
#define F64_EXP_MASK    0x7ff
#define F64_EXP_BIAS    1023
#define F64_EXP_MAX     2047
#define F64_MANT_SHIFT  0
#define F64_MANT_MASK   0xfffff

#define F64_GET_SIGN(fp)        (((fp)->high_word >> F64_SIGN_SHIFT) & \
                                         F64_SIGN_MASK)

#define F64_GET_EXP(fp)         (((fp)->high_word >> F64_EXP_SHIFT) & \
                                       F64_EXP_MASK)

#define F64_SET_EXP(fp, val)    ((fp)->high_word= ((fp)->high_word &    \
                                ~(F64_EXP_MASK << F64_EXP_SHIFT)) |     \
                                 (((val) & F64_EXP_MASK) << F64_EXP_SHIFT))
 
#define F64_GET_MANT_LOW(fp)            ((fp)->low_word)

#define F64_SET_MANT_LOW(fp, val)       ((fp)->low_word= (val))

#define F64_GET_MANT_HIGH(fp)   (((fp)->high_word >> F64_MANT_SHIFT) & \
  	                                      F64_MANT_MASK)

#define F64_SET_MANT_HIGH(fp, val)      ((fp)->high_word= ((fp)->high_word & \
                                 ~(F64_MANT_MASK << F64_MANT_SHIFT)) |   \
                              (((val) & F64_MANT_MASK) << F64_MANT_SHIFT))



struct f64
{
        uint32_t low_word;
        uint32_t high_word;
};



/******************************************************* Function prototypes */

float my_atan(float x);
float my_atan2(float y, float x);
float my_sinus(float x, int8_t cos_flag);
float my_sin(float x);
float my_cos(float x);
float my_modf(float value, float *iptr);


float radToDeg(float value);
float degToRad(float value);
float my_square_root(float value);

int myAbs (int value);
bool_t mySign (int value);


#endif
