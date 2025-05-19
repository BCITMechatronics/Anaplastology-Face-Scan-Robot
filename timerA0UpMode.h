/*
 * timerA0UpMode.h
 *  Description: Header file for setting up TimerA0, used for PWM signals
 *  Created on: Nov. 30, 2024
 *      Author: Jonathan Ina
 */

#ifndef TIMERA0UPMODE_H_
#define TIMERA0UPMODE_H_

//Channels for each of the timer pins
#define CH1 BIT2    //P1.2
#define CH2 BIT3    //P1.3
#define CH3 BIT4    //P1.4
#define CH4 BIT5    //P1.5

//Output mode for producing a PWM signal
#define RESET_SET_MODE 7

#define EN 1
#define UEN 0

#define PWM_FREQUENCY 19999.0  //50Hz

void blinkLED();
int timerA0UpModeChannelConfig (unsigned int channel,unsigned char outmode, unsigned int numCycles);
void timerA0UpModeInit(int ta0ccr0Val);

extern unsigned char toggle;

#endif /* TIMERA0UPMODE_H_ */
