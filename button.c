/*
 * button.c
 *  Description: Source file containing buttons and their ISRs
 *  Created on: Mar. 22, 2025
 *      Author: Jonathan Ina
 */
#include <msp430.h>
#include "buttons.h"

/************************************************************************************
* Function: initButtons
* Description: inititates the pins for the buttons. Pull the pins high
* Arguments: none
* Return: nothing
************************************************************************************/
void initButtons()
{
    //set up the pins as inputs
    P2DIR &= ~(RACK_PIN_FRONT|RACK_PIN_REAR|RING_SW|LEAD_SCREW_TOP|LEAD_SCREW_BOT);
    P1DIR &= ~SOFT_STP;

    //Enable the pullup resistor
    P2REN |= RACK_PIN_FRONT|RACK_PIN_REAR|RING_SW|LEAD_SCREW_TOP|LEAD_SCREW_BOT;
    P1REN |= SOFT_STP;
    P2OUT |= (RACK_PIN_FRONT|RACK_PIN_REAR|RING_SW|LEAD_SCREW_TOP|LEAD_SCREW_BOT);
    P1OUT |= SOFT_STP;

    //Set Edge detection for high to low transition because the pins are pulled up
    P2IES |= (RACK_PIN_FRONT|RACK_PIN_REAR|LEAD_SCREW_TOP|LEAD_SCREW_BOT|RING_SW);
    P1IES |= SOFT_STP;

    //Enable the interrupts (uncomment if you want to use interrupts for the limit switches)
    //P2IE |= RACK_PIN_FRONT|RACK_PIN_REAR|RING_SW|LEAD_SCREW_TOP|LEAD_SCREW_BOT;
}

