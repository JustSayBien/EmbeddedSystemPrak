/**
 * $Id: tools.c 468 2011-02-21 15:53:05Z klugeflo $
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
26.04.2011 (MG) Modified to be used in embedded practical training

*/


/****************************************************************** Includes */

#include <tools.h>


/******************************************************************* Defines */


/******************************************************* Function prototypes */


/************************************************************** Global const */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */


void msleep(uint32_t ms)
{
	wait(ms*100);
}

void wait(uint32_t us) {
  uint32_t i;
  for(i = 0; i < us; ++i) {
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
    __asm__ __volatile__ ("\tl.nop" : : );
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

