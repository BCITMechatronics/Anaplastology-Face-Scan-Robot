/*
 *  motorDriver.h
 *  Description: Header file for the TC78H660FNG Motor Driver
 *  Created on: Mar. 15, 2025
 *      Author: Jonathan Ina
 *  Modified: May 2, 2025
 *
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

//Specific motor driver pin definitions
#define IN1B_ENA BIT4   //Timer pin for motor0 (P1.4)
#define IN2B_ENB BIT5   //Timer pin for motor1 (P1.5)
#define IN1B_ENA_2 BIT3 //Timer pin for motor2 (P1.3)
#define IN1A_PHA BIT0   //Motor driver chip1's IN1A/PHA (P6.0)
#define IN2A_PHB BIT1   //Motor driver chip1's IN2A/PHB (P6.1)
#define IN1A_PHA_2 BIT2 //Motor driver chip2's IN1A/PHA (P6.2)
#define MODE_CHP1 BIT1 //Mode for chip 1 (P3.1)
#define MODE_CHP2 BIT0 //Mode for chip 2 (P3.0)
#define STBY_1 BIT6 //Standby for chip1 (P6.6)
#define STBY_2 BIT5 //Standby for chip2 (P6.5)

//Definitions for using the motor driver
#define PHASE_MODE 1
#define INPUT_MODE 0
#define RING_MOTOR 0
#define RACKPIN_MOTOR 1
#define LEADSCREW_MOTOR 2
#define CCW 0
#define CW 1

//definitions for speed of motor
#define DRIVE_DUTY 75
#define FULL_DUTY 100

void initMotor();

int brake(unsigned int motor);
int drive(unsigned int motor, unsigned int pwm, unsigned int dir);
int changeMode(unsigned int motor, unsigned int mode);

#endif /* MOTORDRIVER_H_ */
