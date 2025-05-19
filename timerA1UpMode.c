/*
 * timerA1UpMode.c
 *
 *  Created on: Apr. 3, 2025
 *      Author: Jonathan Ina
 */

#include <msp430.h>
#include "timerA1UpMode.h"

/*Function: timerA1UpModeInit
 *Description: function for initializing the timerA1 for the PWM signal
 *Arguments: ta1ccr0Val - number of cycles to set frequency of PWM signal
 *Returns: void
 */
void timerA1UpModeInit(long int ta1ccr0Val)
{
    TA1CTL |= TASSEL_2|ID_0| MC_1;
    TA1CCR0 = ta1ccr0Val; //count from ta0CCR0Val
}

/*Function: timerA1UpModeChannelConfig
 *Description: function for configuring I/O pin as a timer pin for PWM
 *Arguments: channel - channel to select as PWM pin
 *           outmode - desired output mode
 *           intEn - enable the interrupt or not
 *Returns: void
 */
void timerA1UpModeChannelConfig (unsigned char channel, unsigned char outmode)
{
    P2DIR |= BIT0; //configure the timer pins as outputs


    //configure the timer for the desire
    switch(channel)
    {
    case PWM_CH1:
        P2SEL|= BIT0; //configure I/O pin as timer
        TA1CCTL1 |= outmode; //set outmode
        TA1CCR1 = 1; //for initialization purposes let's say 0 for now. Once the sampling frequency is determined, this can be changed
        break;
    default:
        break;
    }

}
