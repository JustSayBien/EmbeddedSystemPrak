#ifndef _MYMATH_H
#define _MYMATH_H

/****************************************************************** Includes */
#include "uart.h"
#include "tools.h"

/** The following source code is highly based on the math module of the minix project
 *
 *
 * (c) copyright 1988 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Readme".
 *
 * Author: Ceriel J.H. Jacobs
 *
 *
 * Copyright (c) ...

 The Regents of the University of California. All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 All advertising materials mentioning features or use of this software must display the following acknowledgement: “This product includes software developed by the University of California, Berkeley and its contributors.”
 Neither the name of the University nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/****** BEGIN ************************************************************* */
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

struct f64 {
	uint32_t low_word;
	uint32_t high_word;
};
/****** END ************************************************************* */

/******************************************************* Function prototypes */


float mymathAtan(float x);
float mymathAtan2(float y, float x);
float mymathSinus(float x, int8_t cos_flag);
float mymathSin(float x);
float mymathCos(float x);
float mymathModf(float value, float *iptr);

float mymathRadToDeg(float value);
float mymathDegToRad(float value);
float mymathSquareRoot(float value);
int mymathAbs(int value);
bool_t mymathSign(int value);

#endif
