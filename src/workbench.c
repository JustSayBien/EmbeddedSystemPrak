
#include "mymath.h"
#include "workbench.h"


workbench DEFAULT_WORKBENCH = {0, 0, 0, 0, {ANGLE_UNKNOWN, ANGLE_UNKNOWN, ANGLE_UNKNOWN, ANGLE_UNKNOWN, ANGLE_UNKNOWN}, {DISTANCE_UNKNOWN, DISTANCE_UNKNOWN, DISTANCE_UNKNOWN, DISTANCE_UNKNOWN, DISTANCE_UNKNOWN}};

void workbenchInit(){
	int i;
	for(i=0; i < MAX_COUNT_WORKBENCHES; i++){
		workbenches[i] = DEFAULT_WORKBENCH;
	}	

	//test
	(&workbenches[0])->id = 1;
	(&workbenches[1])->id = 2;
	(&workbenches[1])->distance_to_base_x = -4;
	(&workbenches[1])->distance_to_base_y = 3;
}



int16_t workbenchGetAngle(uint8_t id_from, uint8_t id_to){
	
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


		float angle_rad = mymathAtan2(distance_y, distance_x);
		float angle_deg = mymathRadToDeg(angle_rad);
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

		int16_t dock_angle = workbenches[id_from-1].dock_angle_multiplier * DOCK_ANGLE_INCREMENT;

		angle = 360 - dock_angle + angle;
		if(angle > 360){
			angle %= 360;
		}

		if(angle >= 0){
			angle = -180 + angle;
		}
		else{
			angle = 180 - angle;
		}

		if(angle > 180 || angle < -180){
			angle = -(360 - angle);
		}
		
		workbench* pointer = (workbench*) &workbenches[id_from-1];
		pointer->angle_to_workbench[id_to-1] = angle;
	}

	return -workbenches[id_from-1].angle_to_workbench[id_to-1];
}


int16_t workbenchGetDistance(uint8_t id_from, uint8_t id_to){
	if(id_from == 0 || id_to == 0 || id_from - 1 >= MAX_COUNT_WORKBENCHES || id_to - 1 >= MAX_COUNT_WORKBENCHES){
		//base not configured -> return;
		return DISTANCE_UNKNOWN;
	}
	
	if(workbenches[id_from-1].distance_to_workbench[id_to-1] == DISTANCE_UNKNOWN){
		int8_t distance_x = mymathAbs(workbenches[id_to-1].distance_to_base_x - workbenches[id_from-1].distance_to_base_x);
		int8_t distance_y = mymathAbs(workbenches[id_to-1].distance_to_base_y - workbenches[id_from-1].distance_to_base_y);


		float square_root = mymathSquareRoot(distance_y * distance_y + distance_x * distance_x);
		int16_t square_root_int = (int16_t) (square_root * 1000);
		
		workbench* pointer = (workbench*) &workbenches[id_from-1];
		pointer->distance_to_workbench[id_to-1] = square_root_int;
	}

	return workbenches[id_from-1].distance_to_workbench[id_to-1];

}
