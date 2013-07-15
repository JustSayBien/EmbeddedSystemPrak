#ifndef _WORKBENCH_H
#define _WORKBENCH_H

#include <stdint.h>

#define MAX_COUNT_WORKBENCHES 0x5
#define DOCK_ANGLE_INCREMENT 0x2D
#define ANGLE_UNKNOWN 0xFFF
#define DISTANCE_UNKNOWN -1
#define DIFFERENCE_TO_BASE 0xC8
#define SEEKDOCK_TRIGGER_DISTANCE 0x5DC

typedef volatile struct {
	uint8_t id;
	int8_t distance_to_base_x;
	int8_t distance_to_base_y;
	uint8_t dock_angle_multiplier;
	int16_t angle_to_workbench[MAX_COUNT_WORKBENCHES];
	int16_t distance_to_workbench[MAX_COUNT_WORKBENCHES];
} workbench;

extern workbench DEFAULT_WORKBENCH;
workbench workbenches[MAX_COUNT_WORKBENCHES];

void workbenchInit(void);
int16_t workbenchGetAngle(uint8_t id_from, uint8_t id_to);
int16_t workbenchGetDistance(uint8_t id_from, uint8_t id_to);

#endif
