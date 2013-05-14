/**
 * $Id: led.c 676 2013-04-12 13:57:21Z klugeflo $
 * PIO LED control
 */

/******************************************************************************

File: led.c

Project: Roomba Embedded Systems Training

Description: simulator special functions

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universit�t Augsburg

Created: 13.04.2013

*******************************************************************************

Modification history:
---------------------

06.05.2013 (MG) Changed for Embedded Sys Training

*/


/****************************************************************** Includes */

#include <board.h>
#include <led.h>
#include <pio.h>
#include <asm/io.h>


/******************************************************************* Defines */


/******************************************************* Function prototypes */


/************************************************************** Global const */

#if defined(HAVE_PIO_LBLUE)
#  if __BOARD==db_4ce10
const uint32_t ledb_vals[] = {
  0x00000001, 0x00000002, 0x00000004, 0x00000008,
  0x00000010, 0x00000020
};
#  endif /* __BOARD */
#endif /* HAVE_PIO_LBLUE */


#if defined(HAVE_PIO_LGREEN)
#  if __BOARD==de2-70
const uint32_t ledg_vals[] = {
  0x00000001, 0x00000002, 0x00000004, 0x00000008,
  0x00000010, 0x00000020, 0x00000040, 0x00000080,
  0x00000100
};
#  endif /* __BOARD */
#endif /* HAVE_PIO_LGREEN */


#if defined(HAVE_PIO_LRED)
#  if __BOARD==de2-70
const uint32_t ledr_vals[] = {
  0x00000001, 0x00000002, 0x00000004, 0x00000008,
  0x00000010, 0x00000020, 0x00000040, 0x00000080,
  0x00000100, 0x00000200, 0x00000400, 0x00000800,
  0x00001000, 0x00002000, 0x00004000, 0x00008000,
  0x00010000, 0x00020000
};
#  endif /* __BOARD */
#endif /* HAVE_PIO_LRED */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */

void led_init(void) {
#if defined(HAVE_PIO_LBLUE)
  IOWR32(A_PIO_LBLUE, PIO_DATA, 0);
#endif
#if defined(HAVE_PIO_LGREEN)
  IOWR32(A_PIO_LGREEN, PIO_DATA, 0);
#endif
#if defined(HAVE_PIO_LRED)
  IOWR32(A_PIO_LRED, PIO_DATA, 0);
#endif
}


/*********************************************************** Local functions */

