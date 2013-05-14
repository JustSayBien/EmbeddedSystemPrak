/******************************************************************************

File: rutil.c

Project: Roomba Embedded Systems Training

Description: Programming tools

Author: Alexander Poeppel

Created: 09.05.2013

*******************************************************************************/

#include "rutil.h"
#include "uart.h"
#include "roomba.h"
#include "tools.h"

int roomba_led_status = 0;
int roomba_power_led_color_status = 0;
int roomba_power_led_intensity_status = 0;

uint8_t roomba_digits[4] = {
	48,
	48,
	48,
	48
};

bool_t roomba_button_pressed[6] = {
	false,
	false,
	false,
	false,
	false,
	false
};

bool_t BUTTON_WAIT_RELEASE [6] = {
	false,
	false,
	false,
	false,
	false,
	false
};

uint8_t roomba_leds[4] = {
	LED_DOCK_GREEN,
	LED_SPOT_GREEN,
	LED_CHECK_ROBOT_RED,
	LED_DIRT_DETECT_BLUE
};

void Roomba_commitLEDs() {
	uart_write_byte(CMD_LEDS);
	uart_write_byte(roomba_led_status);
	uart_write_byte(roomba_power_led_color_status);
	uart_write_byte(roomba_power_led_intensity_status);
}

void Roomba_clearDigitsSilent () {
	roomba_digits[3] = 32;
	roomba_digits[2] = 48;
	roomba_digits[1] = 48;
	roomba_digits[0] = 48;
}

void Roomba_clearDigits () {
	Roomba_clearDigitsSilent();
	Roomba_commitDigits();
}

void Roomba_commitDigits () {
	uart_write_byte(CMD_7SEG_ASCII);
	
	uart_write_byte(roomba_digits[3]);
	uart_write_byte(roomba_digits[2]);
	uart_write_byte(roomba_digits[1]);
	uart_write_byte(roomba_digits[0]);
}

void Roomba_commitWeekdayLEDs (int value) {
	uart_write_byte(CMD_SCHEDULE_LED);
	uart_write_byte((uint8_t)value);
	uart_write_byte((uint8_t)0);
}

void Roomba_drive (int16_t velocity) {
	uart_write_byte(CMD_DRIVE);
	uart_write_byte((uint8_t)((velocity >> 8) & 0xFF));
	uart_write_byte((uint8_t)velocity & 0xFF);
	uart_write_byte((uint8_t)0);
	uart_write_byte((uint8_t)0);
}
void Roomba_turn (int16_t velocity, int16_t radius) {
	uart_write_byte(CMD_DRIVE);
	uart_write_byte((uint8_t)((velocity >> 8) & 0xFF));
	uart_write_byte((uint8_t)velocity & 0xFF);
	uart_write_byte((uint8_t)((radius >> 8) & 0xFF));
	uart_write_byte((uint8_t)radius & 0xFF);
}
void Roomba_stop () {
	uart_write_byte(CMD_DRIVE);
	uart_write_byte((uint8_t)0);
	uart_write_byte((uint8_t)0);
	uart_write_byte((uint8_t)0);
	uart_write_byte((uint8_t)0);
}
void Roomba_refreshButtons () {
	uart_write_byte(CMD_BUTTONS_GET);
	uart_write_byte(BUTTONS_PACKET_ID);
	
	uint8_t temp = uart_read_byte();
	roomba_button_pressed[ROOMBA_BUTTON_DAY] = temp & BTN_DAY;
	roomba_button_pressed[ROOMBA_BUTTON_HOUR] = temp & BTN_HOUR;
	roomba_button_pressed[ROOMBA_BUTTON_MINUTE] = temp & BTN_MINUTE;
	roomba_button_pressed[ROOMBA_BUTTON_CLEAN] = temp & BTN_CLEAN;
	roomba_button_pressed[ROOMBA_BUTTON_SPOT] = temp & BTN_SPOT;
	roomba_button_pressed[ROOMBA_BUTTON_DOCK] = temp & BTN_DOCK;
}

void Roomba_queryList (uint8_t length, const packet *query[], int result[]) {
	Roomba_writeQueryList(length, query);
	Roomba_readQueryList(length, query, result);
}

void Roomba_writeQueryList (uint8_t length, const packet *query[]) {
	uart_write_byte(CMD_QUERY_LIST);
	uart_write_byte(length);
	int i;
	for (i=0; i<length; i++) {
		uart_write_byte(query[i]->id);
	}
}

void Roomba_readQueryList (uint8_t length, const packet *query[], int out[]) {
	int i;
	for (i=0; i<length; i++) {		
		uint8_t first_byte = uart_read_byte();
		uint8_t bytes = query[i]->length;
		uint8_t sign = query[i]->sign;
		if (bytes == 1) {
			if (sign) {
				out[i] = (int32_t) ((int8_t) first_byte);
			} else {
				out[i] = (int32_t) first_byte;
			}
		} else if (bytes == 2) {
			uint8_t second_byte = uart_read_byte();
			if (sign) {
				out[i] = (int32_t) ((int16_t) ((first_byte << 8) | second_byte));
			} else {
				out[i] = (int32_t) ((first_byte << 8) | second_byte);
			}
		}
	}
}

void Roomba_LEDCounter (uint8_t value) {
	if (value > 4)
		value = 4;
	roomba_led_status = 0;
	int i;
	for (i=0; i<value; i++) {
		roomba_led_status |= roomba_leds[i];
	}
	Roomba_commitLEDs();
}
