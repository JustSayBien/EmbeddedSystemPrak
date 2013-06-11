#ifndef _WORKBENCH_H
#define _WORKBENCH_H


#define MAX_COUNT_WORKBENCHES 0x5
#define DOCK_ANGLE_INCREMENT 0x2D
#define ANGLE_UNKNOWN 0xFFFF

typedef struct{
	uint8_t id;
	int8_t distance_to_base_x;
	int8_t distance_to_base_y;
	uint8_t dock_angle_multiplier;
	int16_t angle_to_workbench[MAX_COUNT_WORKBENCHES];
	int16_t distance_to_workbench[MAX_COUNT_WORKBENCHES];
} workbench;


const workbench DEFAULT_WORKBENCH = { 0, 0, 0, 0, {ANGLE_UNKNOWN, ANGLE_UNKNOWN, ANGLE_UNKNOWN, ANGLE_UNKNOWN, ANGLE_UNKNOWN}, {0, 0, 0, 0, 0}};

const workbench workbenches[] = {DEFAULT_WORKBENCH, DEFAULT_WORKBENCH, DEFAULT_WORKBENCH, DEFAULT_WORKBENCH, DEFAULT_WORKBENCH};


























#endif
