#ifndef _WORKBENCH_H
#define _WORKBENCH_H


#include "mymath.h"


#define MAX_COUNT_WORKBENCHES 0x5
#define DOCK_ANGLE_INCREMENT 0x2D
#define ANGLE_UNKNOWN 0xFFF

typedef struct{
	uint8_t id;
	int8_t distance_to_base_x;
	int8_t distance_to_base_y;
	uint8_t dock_angle_multiplier;
	int16_t angle_to_workbench[MAX_COUNT_WORKBENCHES];
	int16_t distance_to_workbench[MAX_COUNT_WORKBENCHES];
} workbench;


const workbench DEFAULT_WORKBENCH = { 0, 0, 0, 0, {ANGLE_UNKNOWN, ANGLE_UNKNOWN, ANGLE_UNKNOWN, ANGLE_UNKNOWN, ANGLE_UNKNOWN}, {0, 0, 0, 0, 0}};
workbench workbenches[MAX_COUNT_WORKBENCHES];



void workbenches_init(){
	int i;
	for(i=0; i < MAX_COUNT_WORKBENCHES; i++){
		workbenches[i] = DEFAULT_WORKBENCH;
	}	
}



int16_t get_angle(uint8_t id_from, uint8_t id_to){

	if(id_from == 0 || id_to == 0 || id_from - 1 >= MAX_COUNT_WORKBENCHES || id_to - 1 >= MAX_COUNT_WORKBENCHES){
		//base not configured -> return;
		return ANGLE_UNKNOWN;
	}

	if(workbenches[id_from-1].angle_to_workbench[id_to-1] == ANGLE_UNKNOWN){
		int8_t distance_x = workbenches[id_to-1].distance_to_base_x - workbenches[id_from-1].distance_to_base_x;
		int8_t distance_y = workbenches[id_to-1].distance_to_base_y - workbenches[id_from-1].distance_to_base_y;

		uint8_t quadrant;
		if(distance_x >= 0 && distance_y >= 0){
			quadrant = 1;
		}
		else if(distance_x >= 0 && distance_y < 0){
			quadrant = 2;
		}
		else if(distance_x < 0 && distance_y < 0){
			quadrant = 3;
		}
		else{
			quadrant = 4;	
		}


		float angle_rad = atan2(distance_y, distance_x);
		float angle_deg = radToDeg(angle_rad);
		int16_t angle = (int16_t) angle_deg;


		switch(quadrant){
			case 1:
				angle = 90 - angle;
				break;
			case 2:
				angle = 90 - angle;
				break;
			case 3:
				angle = (270 + angle) * -1;
				break;
			case 4:
				angle = 90 - angle;
				break;
	
		}


		/*int16_t dock_angle = workbenches[id_from-1].dock_angle_multiplier * DOCK_ANGLE_INCREMENT;

		
		
		
		angle = 360 - dock_angle + angle;
		if(angle > 360){
			angle %= 360;
		}

		if(angle >= 0){
			angle = 180 - angle;
		}
		else{
			angle = -180 + angle;
		}

		if(angle > 180 || angle < -180){
			angle = -(360 - angle);
		}
		*/
		workbench* pointer = (workbench*) &workbenches[id_from-1];
		pointer->angle_to_workbench[id_to-1] = angle;
		//workbenches[id_from-1].angle_to_workbench[id_to-1] = angle;
	}

	return workbenches[id_from-1].angle_to_workbench[id_to-1];
}




















#endif
