#include "mymath.h"


float my_atan(float x)
{
     
         static float p[] = {
                 -0.13688768894191926929e+2,
                 -0.20505855195861651981e+2,
                 -0.84946240351320683534e+1,
                 -0.83758299368150059274e+0
         };
         static float q[] = {
                  0.41066306682575781263e+2,
                  0.86157349597130242515e+2,
                  0.59578436142597344465e+2,
                  0.15024001160028576121e+2,
                  1.0
         };
         static float a[] = {
                 0.0,
                 MATH_PI6,  /* pi/6 */
                 MATH_PI2, /* pi/2 */
                 MATH_PI3   /* pi/3 */
         };
 
         int32_t     neg = x < 0;
         int32_t     n;
         float  g;
 
         if (neg) {
                 x = -x;
         }
         if (x > 1.0) {
                 x = 1.0/x;
                 n = 2;
         }
         else    n = 0;
 
         if (x > 0.26794919243112270647) {       /* 2-sqtr(3) */
                 n = n + 1;
                 x = (((0.73205080756887729353*x-0.5)-0.5)+x)/
                         (1.73205080756887729353+x);
         } 
     
         g = x * x;
         x += x * g * POLYNOM3(g, p) / POLYNOM4(g, q);
         if (n > 1) x = -x;
         x += a[n];
         return neg ? -x : x;
}



float my_atan2(float y, float x)
{
         float absx, absy, val;
 
         if (x == 0 && y == 0) {
                 return 0;
         }
         absy = y < 0 ? -y : y;
         absx = x < 0 ? -x : x;
         if (absy - absx == absy) {
                 /* x negligible compared to y */
                 return y < 0 ? -MATH_PI2 : MATH_PI2;
         }
         if (absx - absy == absx) {
                 /* y negligible compared to x */
                 val = 0.0;
         }
         else    val = my_atan(y/x);
         if (x > 0) {
                 /* first or fourth quadrant; already correct */
                 return val;
         }
         if (y < 0) {
                 /* third quadrant */
                 return val - MATH_PI;
         }
         return val + MATH_PI;
}




float my_sinus(float x, int8_t cos_flag)
{
        /*      Algorithm and coefficients from:
                        "Software manual for the elementary functions"
                        by W.J. Cody and W. Waite, Prentice-Hall, 1980
        */

         static float r[] = {
                 -0.16666666666666665052e+0,
                  0.83333333333331650314e-2,
                -0.19841269841201840457e-3,
                  0.27557319210152756119e-5,
               -0.25052106798274584544e-7,
                 0.16058936490371589114e-9,
                -0.76429178068910467734e-12,
                  0.27204790957888846175e-14
         };
 
         float y;
         int8_t neg = 1;
 

         if (x < 0) {
                 x = -x;
                 neg = -1;
         }
         if (cos_flag) {
                 neg = 1;
                 y = MATH_PI2 + x;
         }
         else {  
		y = x;
	 } 
         /* ??? avoid loss of significance, if y is too large, error ??? */
 
         y = y * MATH_1_PI + 0.5;
 
        if (y >= FLOAT_MAX/MATH_PI) return 0.0;
 
         /*      Use extended precision to calculate reduced argument.
                Here we used 12 bits of the mantissa for a1.
                 Also split x in integer part x1 and fraction part x2.
         */
 #define A1 3.1416015625
 #define A2 -8.908910206761537356617e-6
        {
                 float x1, x2;
 
                 my_modf(y, &y);
                 if (my_modf(0.5*y, &x1)) neg = -neg;
                 if (cos_flag) y -= 0.5;
                 x2 = my_modf(x, &x1);
                 x = x1 - y * A1;
                 x += x2;
                 x -= y * A2;
 #undef A1
 #undef A2
         }
  
         if (x < 0) {
                 neg = -neg;
                 x = -x;
         }
 
        /* ??? avoid underflow ??? */
 
         y = x * x;
         x += x * y * POLYNOM7(y, r);
         return neg==-1 ? -x : x;
}
 
float my_sin(float x)
{
        return my_sinus(x, 0);
}

float my_cos(float x)
{
         if (x < 0) x = -x;
         return my_sinus(x, 1);
}


float my_modf(float value,float *iptr)
{
         struct f64 *f64p;
         float tmp;
         int32_t exp;
         int32_t mask_bits;
         uint32_t mant;
 
        f64p= (struct f64 *)&value;
 
         exp= F64_GET_EXP(f64p);
         exp -= F64_EXP_BIAS;
         if (exp < 0)
         {
                 *iptr= 0;
                 return value;
         }
         mask_bits= 52-exp;
         if (mask_bits <= 0)
         {
                 *iptr= value;
                 return 0;
         }
         tmp= value;
         if (mask_bits >= 32)
         {
                 F64_SET_MANT_LOW(f64p, 0);
                 mask_bits -= 32;
                 mant= F64_GET_MANT_HIGH(f64p);
                 mant &= ~((1 << mask_bits)-1);
                 F64_SET_MANT_HIGH(f64p, mant);
         }
         else
         {
                 mant= F64_GET_MANT_LOW(f64p);
                 mant &= ~((1 << mask_bits)-1);
                F64_SET_MANT_LOW(f64p, mant);
         }
         *iptr= value;
         return tmp-value;
}



float radToDeg(float value){
	return value * 180.0f / MATH_PI;
}

float degToRad(float value){
	return MATH_PI * value / 180.0f;
}

int myAbs (int value) {
	return (value < 0) ? -value : value;
}

bool_t mySign (int value) {
	return (value < 0);
}


float my_square_root(float value){
	float n = value / 2.0f;
 	float lstX = 0.0f; 
 	while(n != lstX)  
 	{ 
 		lstX = n;
 		n = (n + value/n) / 2.0f; 
 	}
 	return n;
}
