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
 
         int     neg = x < 0;
         int     n;
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

float radToDeg(float value){
	return value * 180.0f / MATH_PI;	

}

float degToRad(float value){
	return MATH_PI * value / 180.0f;

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
