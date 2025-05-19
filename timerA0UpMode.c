#include <msp430.h>
#include "timerA0UpMode.h"
#include <math.h>
#include "motorDriver.h"
#include "servo.h"
/*
 * timerA0UpMode.c
 *
 *  Created on: Nov. 30, 2024
 *      Author: Jonathan Ina
 *
 *      Pinouts for timers:
 *      Timer for Servo = P1.2/TimerA0.1
 *      Timer for RackMotor = P1.3/TimerA0.2
 *      Timer for LeadScrew Motor = P1.4/TimerA0.3
 *      Timer for Ring Motor = P1.5/TimerA0.4
 */
/*-------------------------------------------------------------------------------------
 * Function Name: timerA0UpModeInit
 * Description: sets up and enables the timers on the MSP430
 * Arguments: ta0ccr0Val - value to count from
 * Type: void
 * Returns: nothing
 --------------------------------------------------------------------------------------*/
void timerA0UpModeInit(int ta0ccr0Val)
{

    //set up the timer control
    TA0CTL |= TASSEL_2|ID_0| MC_1;
    TA0CCR0 = ta0ccr0Val; //count from ta0CCR0Val
    TA0CTL |= TACLR;
    //TA0R = 0; //clear the counter for initiating the new period
}

/*-------------------------------------------------------------------------------------
 * Function Name: timerInit()
 * Description: sets up and configure the channels for timerA0 on the MSP430
 * Arguments: channel - select the desired channel that we want to configure as a timer
 *            pwm - select the desired pwm signal that we want to run at
 * Type: void
 * Returns: check - ensures Channel configuration occured. 0->good,-1->bad
 --------------------------------------------------------------------------------------*/
int timerA0UpModeChannelConfig (unsigned int channel,unsigned char outmode, unsigned int numCycles)
{
   int check = 0;
   P1DIR |= channel; //configure the timer pins as outputs

   P1SEL |= channel; //Select the pins to be used as timer pins

   //assign the initial desired value to count up to for desired duty cycle
   switch(channel)
   {
   case IN1B_ENA:
       TA0CCTL3 |= outmode;
       TA0CCR3 |= numCycles;
       break;
   case IN2B_ENB:
       TA0CCTL4 |= outmode;
       TA0CCR4 |= numCycles;
       break;
   case IN1B_ENA_2:
       TA0CCTL2 |= outmode;
       TA0CCR2 |= numCycles;
       break;
   case SERVO_PWM:
       TA0CCTL1 |= outmode;
       TA0CCR1 |= numCycles;
       break;
   default:
       check = -1;
       break;
   }
   return check;
}
