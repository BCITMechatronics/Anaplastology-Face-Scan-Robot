/*
 * timerA1UpMode.h
 *  Description: Header file for setting up the sampling timer of the ultrasonic sensor
 *  Created on: Apr. 3, 2025
 *      Author: Jonathan Ina
 */

#ifndef TIMERA1UPMODE_H_
#define TIMERA1UPMODE_H_

#define PWM_CH1 0
#define RESET_SET_MODE 7
#define EN 1
#define UEN 0
#define SAMPLING_FREQUENCY 52400.0

void timerA1UpModeChannelConfig (unsigned char channel, unsigned char outmode);
void timerA1UpModeInit(long int ta1ccr0Val);

extern unsigned char scanFlg;

#endif /* TIMERA1UPMODE_H_ */
