/**
 * $Id: tools.c 672 2013-04-12 10:30:44Z klugeflo $
 */

/******************************************************************************

File: tools.c

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS Training

*/


/****************************************************************** Includes */

#include <tools.h>
#include <asm/nios2intrinsics.h>


/******************************************************************* Defines */


/******************************************************* Function prototypes */


/************************************************************** Global const */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */

/* sleep for approx. 10 us (500 cycles @ 50 MHz) */
void my_sleep(uint32_t counter) {
  uint32_t i, j;
  for(i = 0; i < counter; ++i) {
    for (j=0; j<7; j++) {
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
      __asm__ __volatile__ (NOP : : );
    }
  }
}


uint32_t invert_byte_order32(uint32_t in32) {
  uint32_t out32;
  out32 = in32 << 24;
  out32 |= (in32 & 0x0000ff00) << 8;
  out32 |= (in32 & 0x00ff0000) >> 8;
  out32 |= in32 >> 24;
  return out32;
}


uint16_t invert_byte_order16(uint16_t in16) {
  uint16_t out16;
  out16 = in16 << 8;
  out16 |= (in16 & 0xff00) >> 8;
  return out16;
}


/*********************************************************** Local functions */

