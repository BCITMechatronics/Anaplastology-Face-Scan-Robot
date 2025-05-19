/*
 * encoder.c
 *  Description: Source file for dealing with encoders
 *  Created on: Mar. 22, 2025
 *      Author: Jonathan Ina
 */
#include <msp430.h>
#include "encoder.h"
#include "motorDriver.h"

/*-------------------------------------------------------------------------------------
 * Function Name: initEncoder
 * Description: initiates the encoder pins
 * Arguments: N/A
 * Type: void
 * Returns: nothing
 --------------------------------------------------------------------------------------*/
void initEncoder()
{
    //initiate the pins that are using encoders as inputs
    P2DIR &= ~(CHA0|CHA1);
    P6DIR &= ~(CHB0|CHB1);

    P2IES &= ~(CHA0|CHA1); //select low to high transition for edge detection


    //initiate the struct
    enc.count[RING_MOTOR] = 0;

    P2IE |= CHA0|CHA1; //enable the interrupts of the encoders
}

/*-------------------------------------------------------------------------------------
 * Function Name: getPos
 * Description: gets the positoin data from the encoders
 * Arguments: motor - the motor that we are reading the pulses from
 *            count - value of pulse counts
 *            posData - pointer to the variable containing the position data
 * Type: int
 * Returns: check - ensures that function was executed successfully 0-good, -1-bad
 --------------------------------------------------------------------------------------*/
int getPos(unsigned int motor, int count, float*posData)
{
    int check = 0; //check variable
    switch(motor)
    {
    case RING_MOTOR:
        //calculate using the conversion factors that you have/calculated
        *posData = (1/(12.0*98.78*504.0/28.0/360.0))*enc.count[RING_MOTOR];  //calculate the position of the ring
        break;
    //Leadscrew motor has not been implemented. This section is blocked out until the leadscrew section is implemented
    /*case LEADSCREW_MOTOR:
        //calculate using the conversion factors that you have/calculated
        break;*/
    default:
        check = -1; //error occurred in getting the position data, make the check variable = -1
        break;
    }

    return check;
}
