/*
 * servo.c
 *  Description: Source file for controlling the servo motor
 *  Created on: Mar. 19, 2025
 *      Author: Jonathan Ina
 */

#include "servo.h"
#include <msp430.h>
#include "timerA0UpMode.h"
#include <math.h>


/*-------------------------------------------------------------------------------------
 * Function Name: initServo
 * Description: initializes the servo
 * Arguments: N/A
 * Type: void
 * Returns: void
 --------------------------------------------------------------------------------------*/
void initServo()
{
    //configure the timer pin so that it starts in starting position (currently set to zero)
    timerA0UpModeChannelConfig(SERVO_PWM,OUTMOD_7, INIT_POS);
}

/*-------------------------------------------------------------------------------------
 * Function Name: servoPos
 * Description: changes the servo position to a desired position
 * Arguments: angleInDeg - the desired angle in degrees
 * Type: int
 * Returns: check - ensures that the servo has moved to desired position. 0->good,-1->bad
 --------------------------------------------------------------------------------------*/
int servoPos(unsigned int angleInDeg)
{
    int check = 0;
    long int cycles = 0;
    //servo needs a 50Hz signal to work
    
    //This is the equation that we derived to work for the servo we used
    //Another servo will most likely need to be purchased and therfore this equation will change as well
    cycles = -11 *angleInDeg+1500;
    //test input
    TA0CCR1 = cycles;

    return check;

}
