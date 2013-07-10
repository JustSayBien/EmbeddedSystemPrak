/**
 * $Id: lcd.h 672 2013-04-12 10:30:44Z klugeflo $
 */

/******************************************************************************

File: lcd.h

Project: Roomba Embedded Systems Training

Description: LCD text output

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS Training

*/

#ifndef _LCD_H
#define _LCD_H 1

/****************************************************************** Includes */

#include <stdint.h>

#include <board.h>

#ifdef HAVE_LCD

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************* Defines */


/****************************************************************** Typedefs */


/************************************************************** Global const */


/********************************************************** Global variables */


/************************************************ Global function prototypes */

/*! Initialise LCD */
  void lcd_init(void);

  /*!
    write a char to LCD
    \param c char
    \return (unsigned char) c
  */
  int32_t lcd_putchar(int32_t c);

  /*!
    write a string to LCD
    \param s the string
    \return number of chars written to LCD
   */
  int32_t lcd_puts(char *s);


  /*!
    Clear the display
  */
  void lcd_clear(void);
  


/*************************************************** Global inline functions */



/******************************************************************** Macros */



#ifdef __cplusplus
}
#endif

#endif /* HAVE_LCD */

#endif /* !_LCD_H */
