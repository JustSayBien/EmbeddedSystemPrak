#ifndef _WORKBENCH_H
#define _WORKBENCH_H


#include "uart.h"
#include "mymath.h"

#define MAX_COUNT_WORKBENCHES 0x6
#define DOCK_ANGLE_INCREMENT 0x2D
#define ANGLE_UNKNOWN 0xFFF
#define DISTANCE_UNKNOWN -1
#define DIFFERENCE_TO_BASE 0xC8
#define SEEKDOCK_TRIGGER_DISTANCE 0x3E8

typedef struct{
	uint8_t id;
	int8_t distance_to_base_x;
	int8_t distance_to_base_y;
	uint8_t dock_angle_multiplier;
	int16_t angle_to_workbench[MAX_COUNT_WORKBENCHES];
	int16_t distance_to_workbench[MAX_COUNT_WORKBENCHES];
} workbench;


extern workbench DEFAULT_WORKBENCH;
workbench workbenches[MAX_COUNT_WORKBENCHES];



void workbenches_init();

int16_t get_angle(uint8_t id_from, uint8_t id_to);

int16_t get_distance(uint8_t id_from, uint8_t id_to);

	










#endif