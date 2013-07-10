/**
 * $Id: led.h 478 2011-04-19 08:06:54Z bradatch $
 */

/******************************************************************************

File: ir.h

Project: Roomba Embedded Systems Training

Description: Driver for IR sender

Author: Florian Haas <florian.haas@student.uni-augsburg.de>
        Universitaet Augsburg

Created: 23.03.2012

*******************************************************************************

Modification history:
---------------------

23.03.2012 Created from led.h
16.05.2012 Small changes
10.06.2013 (MG) Modified for NIOSII
*/

#ifndef _IR_H
#define _IR_H 1

/****************************************************************** Includes */

#include <stdint.h>

#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************* Defines */

#define IR_TIMER_MAX_VAL	  	0x0400
#define IR_NUM_LEDS			   4
#define IR_NUM_SENSORS			   3

#define IR_RECEIVER_OMNI		   0
#define IR_RECEIVER_LEFT		   1
#define IR_RECEIVER_RIGHT		   2

#define IR_SENDER_MASK			0xC0
#define IR_SENDER_ROOMBA		0xC0
#define IR_SENDER_REMOTE		0x80
#define IR_DATA_MASK			0x0F
#define IR_ROOMBA_MASK			0x18

#define IR_LED_NONE			0x00
#define IR_LED_LEFT			0x01
#define IR_LED_CENTER			0x02
#define IR_LED_RIGHT			0x04
#define IR_LED_NEAR			0x08

#define IR_REMOTE_NONE			0x00
#define IR_REMOTE_UP			0x02
#define IR_REMOTE_LEFT			0x01
#define IR_REMOTE_DOWN			0x0D
#define IR_REMOTE_RIGHT			0x03
#define IR_REMOTE_DOCK			0x0F
#define IR_REMOTE_SPOT			0x04
#define IR_REMOTE_CHANNEL_UP		0x08

#define IR_SENDER_CTRL			0x00
#define IR_SENDER_DATA			0x04

#define SENSOR_IR_OMNI			17
#define SENSOR_IR_LEFT			52
#define SENSOR_IR_RIGHT			53

#define ROOMBA_ID 			0x1 // 2 Bits available


/****************************************************************** Typedefs */


/************************************************************** Global const */

#ifdef HAVE_IR
/* only available if roomba fpga board supports IR sending */

/// set data for each IR LED.  sending has to be started explicit
void ir_sender_set(unsigned char left, unsigned char center, unsigned char right, unsigned char near);

/// start or continue sending IR data on all LEDs
void ir_sender_on();

/// stop sending IR data on all LEDs (data is not lost, only sending is stopped)
void ir_sender_off();
#endif /* __BOARD==? */



/********************************************************** Global variables */

/// if data is received from another roomba, its ID is stored here
extern uint8_t ir_roomba_id;

/// each byte stores the raw data received by its corresponding IR receiver
extern uint8_t ir_roomba_leds[IR_NUM_SENSORS];

/// if a remote command is received, it is stored here
extern uint8_t ir_remote_command;

/************************************************ Global function prototypes */

/// receive IR data sent by another roomba or by roomba remote
/*
 *  all of roomba's IR sensors are checked if a byte is received that has been
 *  sent from another roomba or from the roomba remote.  The raw data is stored
 *  in ir_roomba_leds if the data is sent from a roomba or in ir_remote_command
 *  if it's sent from a remote.  In case the data is received from a roomba, its
 *  ID is stored in ir_roomba_id, otherwise it is set to 0.
 *
 *  To access the received data, use the macros ir_get_roomba and ir_sensor_has
 *  (see below for description).
 *
 *  Note: Don't forget to call ir_timers_increment() every 1ms for correct
 *        timeout detection!
 *        The received data has to be read before the next call to this method,
 *        because the data will be overwritten.
 */
void ir_receive();

/*************************************************** Global inline functions */


/******************************************************************** Macros */

/// setup IR sender LEDs
/* 
 *  each led sends a single byte which consists of 3 parts: sender type, sender
 *  id and led id:
 *  byte being send by led: IIRRLLLL:
 *    I: "11",
 *    R: roomba-id(0-3),
 *    L: 1 (left), 2 (center), 4 (right), 8 (near)
 *
 */
#define ir_sender_setup() ir_sender_set( \
					(ROOMBA_ID&0x03)<<4 | IR_SENDER_ROOMBA | IR_LED_LEFT,	\
					(ROOMBA_ID&0x03)<<4 | IR_SENDER_ROOMBA | IR_LED_CENTER,	\
					(ROOMBA_ID&0x03)<<4 | IR_SENDER_ROOMBA | IR_LED_RIGHT,	\
					(ROOMBA_ID&0x03)<<4 | IR_SENDER_ROOMBA | IR_LED_NEAR);

/// get id of roomba if IR-omni-sensor receives a byte from a roomba IR sender
#define ir_get_roomba()   (ir_roomba_id&0x03)

/// check IR-sensor s if it receives IR-LED l
#define ir_sensor_has(s,l)  (ir_roomba_leds[s] & l)


#ifdef __cplusplus
}
#endif

#endif /* !_LED_H */
