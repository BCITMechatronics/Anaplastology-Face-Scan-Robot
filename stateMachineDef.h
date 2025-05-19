/*
 * stateMachineDef.h
 *  Description: Definitions for the states of the state machine
 *  Created on: May 2, 2025
 *      Author: Jonathan Ina
 *  Modified on : May 2, 2025
 */

#ifndef STATEMACHINEDEF_H_
#define STATEMACHINEDEF_H_

//definitions for all of the states (will need to implement at least two more states for a homing state for leadscrew at some later time)
#define STATE_0_INIT 0 //initialization state
#define STATE_1_HOME_RING 1 //Home Ring state
#define STATE_2_HOME_RACKREAR 2 //Home RackPin (rear side) state
#define STATE_3_HOME_RACKFRONT 3 //Home RackPin (front side) state
#define STATE_4_WAIT_USERCMD 4 // Wait for User Command state
#define STATE_5_SCAN 5 //Scanning state
#define STATE_6_TILT_SERVO 6//State for tilting the servo
#define STATE_7_GET_DISTANCE 7 //State for getting the distance from the ultrasonic
#define STATE_8_GET_POSITION 8 //State for getting the angular position of the ring
#define STATE_900_EMERGENCY 900 //Emergency state

extern unsigned int nextState;
#endif /* STATEMACHINEDEF_H_ */
