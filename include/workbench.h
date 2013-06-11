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





int16_t get_angle(uint8_t id_from, uint8_t id_to){

	if(id_from == 0 || id_to == 0 || id_from - 1 >= MAX_COUNT_WORKBENCHES || id_to - 1 >= MAX_COUNT_WORKBENCHES){
		//base not configured -> return;
	}

	if(workbenches[id_from-1].angle_to_workbench[id_to-1] != ANGLE_UNKNOWN){
		int8_t distance_x = workbenches[id_to-1].distance_to_base_x - workbenches[id_from-1].distance_to_base_x;
		int8_t distance_y = workbenches[id_to-1].distance_to_base_y - workbenches[id_from-1].distance_to_base_y;

		//TODO
	}	
	
}




















#endif
