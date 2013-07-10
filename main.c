/*! \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * etc...
 */
/******************************************************************************

File: main.c

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Andreas Sabitzer
        Universität Augsburg

Created: 08.05.2013


*******************************************************************************

Modification history:
---------------------

*/


/****************************************************************** Includes */

#include "program.h"
#include "roomba.h"
#include "button.h"

/******************************************************************* Defines */


/******************************************************* Function prototypes */



/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */


/*********************************************************** Local functions */

int main(int argc, char *argv[]) {

	//init program
	led_set_blue(ledb_vals[0] | ledb_vals[1]);

	button_wait(0);

	init_roomba();

	program_run();
	/*while (true) {
		intToAscii(query_sensor(PACKET_INFRARED_CHARACTER_OMNI), roomba_sevenseg_digits);
		write_sevenseg_digits();
		my_msleep(50);
	}*/
	
  	return 0;
}




