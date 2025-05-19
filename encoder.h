/*
 * encoder.h
 *  Description: Header file for handling with encoders of the Ring motor and the Leadscrew motor
 *  Created on: Mar. 22, 2025
 *      Author: Jonathan Ina
 */

#ifndef ENCODER_H_
#define ENCODER_H_

//Encoder Pin Definitions
#define CHA0 BIT2 //Encoder 0 Channel A (P2.2)
#define CHA1 BIT6 //Encoder 1 Channel A(P2.6)
#define CHB0 BIT3 //Encoder 0 Channel B(P6.3)
#define CHB1 BIT4 //Encoder 1 Channel B(P6.4)

//for size of struct array
#define NUMENC 2 // we are only dealing with two potential encoders therefore we need two

typedef struct ENCODER
{
    int count[NUMENC];
}ENCODER;

extern ENCODER enc;

void initEncoder();
int getPos(unsigned int motor, int count,float*posData);



#endif /* ENCODER_H_ */
