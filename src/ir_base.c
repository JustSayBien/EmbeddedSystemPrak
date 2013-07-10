/******************************************************************************

File: ir_base.c

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel
        Universität Augsburg

Created: 28.06.2013


******************************************************************************/

/****************************************************************** Includes */

#include "ir_base.h"
#include "program.h"


/******************************************************************* Defines */

#define IR_RECOG_END_TIME		2000


/*********************************************************** Local variables */

int base_id_codes[] = {
	0x8,
	0xC,
	0xE,
	0x7,
	0x3,
	0x1
};

uint8_t base_recog_counter[MAX_COUNT_WORKBENCHES] = {
	0,
	0,
	0,
	0,
	0,
	0
};

millis_t base_id_activation_times[MAX_COUNT_WORKBENCHES] = {
	0,
	0,
	0,
	0,
	0,
	0
};

uint8_t check_discrete_base_id () {
	int value = query_sensor(PACKET_INFRARED_CHARACTER_OMNI);
		
	int i;
	for (i=0; i<MAX_COUNT_WORKBENCHES; i++) {
		millis_t difference = global_clock - base_id_activation_times[i];
		if ((value >> 4) == base_id_codes[i]) {
			if (difference > IR_RECOG_END_TIME) {
				base_recog_counter[i] = 0;
			}
			
			while (base_id_activation_times[i] == 0 || difference <= IR_RECOG_END_TIME) {
				intToAscii(global_clock, roomba_sevenseg_digits);
				base_id_activation_times[i] = global_clock;
				write_sevenseg_digits();
				base_recog_counter[i]++;
				
				if (base_recog_counter[i] > 10) {
					base_recog_counter[i] = 0;
					return (i+1);
				}
				value = query_sensor(PACKET_INFRARED_CHARACTER_OMNI);
			}
			base_recog_counter[i] = 0;
			base_id_activation_times[i] = 0;
		}/* else if (difference > IR_RECOG_END_TIME) {
			base_id_activation_times[i] = 0;
		}*/
	}
	return 0;
}

