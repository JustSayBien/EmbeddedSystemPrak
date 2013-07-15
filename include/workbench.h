#ifndef _WORKBENCH_H
#define _WORKBENCH_H

/****************************************************************** Includes */
#include <stdint.h>

/******************************************************************* Defines */
#define MAX_COUNT_WORKBENCHES 0x5 // maximum count of workbenches
#define DOCK_ANGLE_INCREMENT 0x2D // a workbench base can be turned in increments of 45 degrees
#define ANGLE_UNKNOWN 0xFFF // the angle value if an angle between two workbenches is not set or calculated yet
#define DISTANCE_UNKNOWN -1 // the distance value if a distance between two workbenches is not set or calculated yet
#define DIFFERENCE_TO_BASE 0xC8 // the difference which the roomba needs to undock from a base before starting regular approach
#define SEEKDOCK_TRIGGER_DISTANCE 0x5DC // the distance value which triggers the roomba's infrared sensors to be turned on

/******************************************************************* Structures */

/**
 * A structure which represents a workbench station
 */
typedef volatile struct {
	uint8_t id; /**< The id of the workbench */
	int8_t distance_to_base_x; /**< The distance on x-axis relating to the home base (base with id 0)  */
	int8_t distance_to_base_y; /**< The distance on y-axis relating to the home base (base with id 0)  */
	uint8_t dock_angle_multiplier; /**< The multiplier of the DOCK_ANGLE_INCREMENT relating to home base (base with id 0) */
	int16_t angle_to_workbench[MAX_COUNT_WORKBENCHES]; /**< An array which acts like a cache to store the angle between two discrete workbenches(index == id-1), in order to calculate it only once */
	int16_t distance_to_workbench[MAX_COUNT_WORKBENCHES]; /**< An array which acts like a cache to store the distance between two discrete workbenches (index == id-1), in order to calculate it only once */
} workbench;

/******************************************************* Function prototypes */

/**
 * \brief  init workbenches function
 *
 *	This function initializes all workbenches to the default value (DEFAULT_WORKBENCH)
 *
 *
 */
void workbenchInit(void);

/**
 * \brief  Get the angle between two workbenches
 *
 *         This function returns the angle between the workbenches. It either gets the previously calculated angle value out of the 'cache' or calculates the angle and stores it in the ’cache’ for the next time.
 *
 * \param   id_from 	the id of the current base
 * \param 	id_to	the id of the base to drive to
 * \return the angle between the the specified workbenches. The value is in degrees and signed (+ counter clockwise, - clockwise)
 *
 */
int16_t workbenchGetAngle(uint8_t id_from, uint8_t id_to);

/**
 * \brief  Get the distance between two workbenches
 *
 *         This function returns the distance between the workbenches. It either gets the previously calculated distance value out of the 'cache' or calculates the distance and stores it in the ’cache’ for the next time.
 *
 * \param   id_from 	the id of the current base
 * \param 	id_to	the id of the base to drive to
 * \return the distance between the the specified workbenches (in millimeters).
 *
 */
int16_t workbenchGetDistance(uint8_t id_from, uint8_t id_to);

/************************************************************** Global const */
// default value which is used to initialize all workbenches
extern workbench DEFAULT_WORKBENCH;

// array of workbench structures containing all possible workbenches
workbench workbenches[MAX_COUNT_WORKBENCHES];

#endif
