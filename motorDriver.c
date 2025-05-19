/*
 * motorDriver.c
 * Description: source file for controlling the motor driver
 *
 *  Created on: Mar. 15, 2025
 *      Author: Jonathan Ina
 *  Modified on: May 2, 2025
 */
#include <msp430.h>
#include "motorDriver.h"
#include "timerA0UpMode.h"
#include<math.h>


/*-------------------------------------------------------------------------------------
 * Function Name: initMotor
 * Description: sets up and enables the pins to control the motor drivers
 * Arguments: N/A
 * Type: void
 * Returns: nothing
 --------------------------------------------------------------------------------------*/
void initMotor()
{

    //set up the motor driver pins as outputs
    P6DIR |= IN1A_PHA|IN2A_PHB|IN1A_PHA_2|STBY_1|STBY_2;
    P3DIR |= MODE_CHP1|MODE_CHP2;

    //set the standby pins to enable mode change
    P6OUT &= ~STBY_1|STBY_2;

    //set up the timer motor driver pins. Set them to zero for zero speed and position
    timerA0UpModeChannelConfig(IN1B_ENA,OUTMOD_7,0);
    timerA0UpModeChannelConfig(IN2B_ENB,OUTMOD_7,0);
    timerA0UpModeChannelConfig(IN1B_ENA_2,OUTMOD_7,0);

    //configure the motor driver to start off in phase mode
    P3OUT |= MODE_CHP1|MODE_CHP2; //set Modes for both chips as high for phase mode
    P6OUT |= STBY_1|STBY_2; //set standby mode for both chips to finish changing modes

}

/*-------------------------------------------------------------------------------------
 * Function Name: brake
 * Description: brakes the desired motor
 * Arguments: motor -select desired motor that you want to brake
 * Type: int
 * Returns: check - ensures that a desired motor has braked. 0-good,-1 - bad.
 --------------------------------------------------------------------------------------*/
int brake(unsigned int motor)
{
    int check = 0;

    //brake the desired motor
    //changeMode(motor,INPUT_MODE);
    switch(motor)
    {
    case RING_MOTOR:
        TA0CCR3 = 0;
        break;
    case RACKPIN_MOTOR:
        TA0CCR4 = 0;
        break;
    //will need to implement another case for the leadscrew motor as well
    default:
        check = -1;
        break;

    }
    return check;
}



/*-------------------------------------------------------------------------------------
 * Function Name: drive
 * Description: drives the desired motor in phase mode
 * Arguments: motor - select the desired motor that you want to run in phase mode
 *            pwm - select the desired counter value to perform desired speed
 * Type: void
 * Returns: ensures an actual motor's has been energized. 0-good, -1 - bad
 --------------------------------------------------------------------------------------*/
int drive(unsigned int motor, unsigned int pwm, unsigned int dir)
{
    int check = 0; //variable for error checking
    volatile long int motorDutyCycle  = 0; //variable for finding the desired number of cycles for the appropiate duty cycle

    //calculate the number of cycles to set the desired PWM signal
    motorDutyCycle = round((float)pwm/100.0 *PWM_FREQUENCY);

    //if the motor driver is in input mode, set it to phase mode
    if(!(P3OUT&BIT1))
    {
        changeMode(motor, PHASE_MODE);
    }

    //set the duty cycle for the desired motor
    switch(motor)
    {
    case RING_MOTOR:

        //set the direction of the motor
        if(dir)
        {
            P6OUT |= IN1A_PHA;
        }
        else
        {
            P6OUT &= ~IN1A_PHA;
        }
        //set the PWM signal to desired duty cycle
        TA0CCR3 = motorDutyCycle;
        break;
    case LEADSCREW_MOTOR:
        //set the direction and pwm signal for the leadscrew motor here
        break;
    case RACKPIN_MOTOR:
        //set the direction
        if(dir)
        {
            P6OUT |= IN2A_PHB;
        }
        else
        {
            P6OUT &= ~IN2A_PHB;
        }
        //set the desired PWM signal to desired duty cycle
        TA0CCR4 = motorDutyCycle;
        break;
    default:
        check = -1;
        break;
    }
    return check;
}

/*-------------------------------------------------------------------------------------
 * Function Name: changeMode
 * Description: changes the desired motor's mode
 *              (originally this function was made with the idea of putting the motor driver chip into
 *              input mode to implement the electrical braking characteristic of the chip. Due to time,
 *              it wasn't implemented and instead the motor turns off by setting the duty cycle to 0%)
 * Arguments: motor - select desired motor that you want to change modes
 *            mode - select the desired mode (Phase or Input)
 * Type: valid - check to see that changing modes was completed
 * Returns: ensures an motor driver's mode has been changed. 0-good, -1 - bad
 --------------------------------------------------------------------------------------*/
int changeMode(unsigned int motor, unsigned int mode)
{
    int check = 0; //variable for error checking

    //Currently the ring motor and the rack and pinion motor on the same motor driver chip. This might change when implementing lead screw motor
    if((motor == RING_MOTOR) || (motor == RACKPIN_MOTOR)) //for motor 0 and 1 since they share the same chip
    {
        if(mode)
        {
            //set the standby pin low to change modes
            P6OUT &= ~STBY_1;
            //set mode for chip 1 as high for phase mode
            P3OUT |= MODE_CHP1;
            //set the pwm signal to zero to prevent the motor from moving
            TA0CCR3 = 0;
            //set the standby pin to high to finish changing modes
            P6OUT |= STBY_1;
        }
        else
        {
            //set the standby pin low to change modes
            P6OUT &= ~STBY_1;
            //set mode for chip 1  as low for input mode
            P3OUT &= ~MODE_CHP1;
            //set the pwm signal to 100% to ensure that motors will brake
            TA0CCR3 = 19999;
            //set the directional input pins high and turn standby on to brake motor.
            P6OUT |= IN1A_PHA|IN2A_PHB|STBY_1;
        }

    }
    else if(motor == LEADSCREW_MOTOR)
    {
        if(mode)
        {
            //set the standby pin low to change modes
            P6OUT &= ~STBY_2;
            //set mode for chip 2 as high for phase mode
            P3OUT |= MODE_CHP2;
            //set the pwm signal to zero to prevent the motor from moving
            TA0CCR2 = 0;
            //set the standby pin to high to finish changing modes
            P6OUT |= STBY_1;
        }
        else
        {
            //set the standby pin low to change modes
            P6OUT &= ~STBY_2;
            //set mode for chip 1  as low for input mode
            P3OUT &= ~MODE_CHP2;
            //set the pwm signal to 100% to ensure that motors will brake
            TA0CCR2 = 19999;
            //set the standby pin to high and the direction pin as high to brake
            P6OUT |= IN1A_PHA_2|STBY_2;
        }
    }
    else
    {
        check = -1;
    }
    return check;
}
