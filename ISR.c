/*
 * ISR.c
 *  Description: ISR module for all of the interrupts being used in this system
 *  Created on: Mar. 22, 2025
 *      Author: Jonathan Ina
 */
#include <msp430.h>
#include "stateMachineDef.h"
#include "motorDriver.h"
#include "buttons.h"
#include "usciUart.h"
#include "ultrasonic.h"
#include "encoder.h"
#include "timerA0UpMode.h"
#include "timerA1UpMode.h"

ENCODER enc;
unsigned char scanFlg = 0;
unsigned char distanceData = 0;
unsigned char emergencyFlg = 0;




//ISR for the sampling timer
#pragma vector = TIMER1_A1_VECTOR
__interrupt void SAMPLE(void)
{
    //perform sampling state machine here
    if(scanFlg && !emergencyFlg)
    {
        getDistance(SLAVE_ADDRESS, &distanceData);

        if(distanceData > (IDEAL_DISTANCE + TOLERANCE))
        {
            //move the camera forward
            drive(RACKPIN_MOTOR,FULL_DUTY,CCW);

        }
        else if (distanceData < (IDEAL_DISTANCE - TOLERANCE))
        {
            //move the camera backward
            drive(RACKPIN_MOTOR,FULL_DUTY,CW);
        }
        else
        {
            //do not move the camera
            brake(RACKPIN_MOTOR);
        }

    }

    TA1CTL &= ~TAIFG; //clear the flag to continue

}

//ISR for the soft stop button
#pragma vector = PORT1_VECTOR
__interrupt void SOFT_STOP(void)
{
    //set the emergency flag
    emergencyFlg = 1;
    //clear the interrupt flag
    P1IFG = 0;
}

//ISR for the limit switches and encoders
#pragma vector = PORT2_VECTOR
__interrupt void LIMITSW_ENCODER(void)
{
    switch(_even_in_range(P2IV,16))
    {
    case 0x02: //limit switch for scanner module front
        //if interrupts are being used for the limit switches, put the code for taking action here
        break;

    case 0x06:  //Encoder0 Channel A
        if(P6IN&CHB0)
        {
            enc.count[RING_MOTOR] = enc.count[RING_MOTOR] + 1; //increase the pulse count of encoder0
        }
        else
        {
            enc.count[RING_MOTOR] = enc.count[RING_MOTOR] -1; //decrease the pulse count of encoder0
        }
        break;

        //will need to implement an interrupt for the second encoder for the leadscrew encoder

    case 0x08:  //limit switch for scanner module rear
        //if interrupts are being used for the limit switches, put the code for taking action here
        break;

    case 0x0C:  //limit switch for lead screw bottom
        //if interrupts are being used for the limit switches, put the code for taking action here
        break;

    case 0x10:  //limit switch for homing the ring
        //if interrupts are being used for the limit switches, put the code for taking action here

        break;

    }





}
