/*
 * ultrasonic.h
 *  Description: Header file for writing and reading to Ultrasonic sensor
 *  Created on: Mar. 19, 2025
 *      Author: Jonathan Ina
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define ULTRATX BIT3 //Pin for transmitting data to Ultrasonic sensor(P3.3)
#define ULTRARX BIT4 //Pin for receiving data from Ultrasonic sensor (P3.4)
#define DUMMY_BYTE 0xFF //dummy byte to send in order to receive a byte of data
#define SLAVE_ADDRESS 0x55  //address of ultrasonic device
#define IDEAL_DISTANCE 203 //ideal distance from patient
#define TOLERANCE 5       //allowable +/- tolerance

void initUltrasonic();
int getDistance(char slvAdd, unsigned char * distance);

extern unsigned char distanceData;


#endif /* ULTRASONIC_H_ */
