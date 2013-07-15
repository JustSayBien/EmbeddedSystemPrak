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
#include "ir_remote.h"


/******************************************************************* Defines */

#define IR_RECOG_END_TIME		3000


/*********************************************************** Local variables */

int base_id_codes[] = {
	0xB,
	0xC,
	0xE,
	0x7,
	0x3
};

uint8_t base_recog_counter[MAX_COUNT_WORKBENCHES] = {
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
	0
};

uint8_t check_base_id () {
	//int value = query_sensor(PACKET_INFRARED_CHARACTER_OMNI); => raw_ir_data in ir_remote.h
	/*int i;
	for (i=0; i<MAX_COUNT_WORKBENCHES; i++) {
		millis_t difference = global_clock - base_id_activation_times[i];
		intToAscii(raw_ir_data, roomba_sevenseg_digits);
		write_sevenseg_digits();
		if ((raw_ir_data >> 4) == base_id_codes[i]) {
			if (difference > IR_RECOG_END_TIME) {
				base_recog_counter[i] = 0;
			}
			
			if (base_id_activation_times[i] == 0 || difference <= IR_RECOG_END_TIME) {
				//intToAscii(global_clock, roomba_sevenseg_digits);
				base_id_activation_times[i] = global_clock;
				roomba_sevenseg_digits[3] = 'A';
				roomba_sevenseg_digits[2] = 'T';
				roomba_sevenseg_digits[1] = ' ';
				roomba_sevenseg_digits[0] = ((i+1) + ASCII_NUMBER_START);
				write_sevenseg_digits();
				
				base_recog_counter[i]++;
				
				if (base_recog_counter[i] > 4) {
					return (i+1);
				}
			}
		} else if (difference > IR_RECOG_END_TIME) {
			base_id_activation_times[i] = 0;
		}
	}*/
	int i;
	for (i=0; i<MAX_COUNT_WORKBENCHES; i++) {
		if ((raw_ir_data >> 4) == base_id_codes[i]) {
			return (i+1);
		}
	}
	return 0;
}

uint8_t check_discrete_base_id () {
	int i;
	for (i=0; i<MAX_COUNT_WORKBENCHES; i++) {
		millis_t difference = global_clock - base_id_activation_times[i];
		
		if ((raw_ir_data >> 4) == base_id_codes[i]) {
			if (difference > IR_RECOG_END_TIME) {
				base_recog_counter[i] = 0;
			}
			
			if (base_id_activation_times[i] == 0 || difference <= IR_RECOG_END_TIME) {
				base_recog_counter[i]++;
				
				if (base_recog_counter[i] > 3) {
					return (i+1);
				}
			}
		} else if (difference > IR_RECOG_END_TIME) {
			base_id_activation_times[i] = 0;
		}
	}
	return 0;
}

