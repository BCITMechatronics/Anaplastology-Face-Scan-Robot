/*
 * servo.h
 *  Description: Header file for setting up the servo and functions for controlling the servo
 *  Created on: Mar. 19, 2025
 *      Author: Jonathan Ina
 */

#ifndef SERVO_H_
#define SERVO_H_

#define SERVO_PWM BIT2              //Timer pin for the servo
#define INIT_POS 0                  //change to pwm signal of starting position
#define SERVO_POS_MAX 180           //maximum position of servo
#define SERVO_POS_MIN 0             //minimum position of servo

void initServo();
int servoPos(unsigned int angleInDeg);
#endif /* SERVO_H_ */
