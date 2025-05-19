/*PROJECT FACE SCAN
 * Description: This is the main code for the face scanner robot project
 *
 * Author: Jonathan Ina
 * Created: March 12 2025
 * Modified: April 16 2025
 *
 * Changes made:
 *
 * March 15, 2025:
 *  - Defined all the pins to include all interrupts that will be needed
 *  - Added motorDriver header and source file with necessary functions
 *
 *  March 18 2025:
 *  -Started writing actions in the functions for motorDriver.c
 *  -Confirm whether there needs to be a delay to change modes and what happens if there is an error in changeMode
 *
 *  March 19 2025:
 *  -Redefined the timerA0UpModeChannelConfig to configure each channel seperately
 *  -Added source and header file for servo and ultrasonic
 *  -Will need to test servo to ensure we know the ranges that it can work under
 *  -Next time when you modify, make sure to start by setting up the ultrasonic sensor correctly
 *
 *  March 22 2025+:
 *  *+ indicates that I missed a day of documentation between last modification and most recent modification*
 *  -Made header and source file for all of the buttons and the encoders
 *  -Created a source file for the ISR
 *  -Buttons/switches are working with ISR and Ultrasonic is now communicating with MSP430
 *  -Some more testing of Ultrasonic may prove beneficial as I was not entirely satisfied with its results (timing may be off)
 *  -Sample test code for Ultrasonic can be found in comment below main function
 *  - May also need to consider which buttons/encoders have higher priority
 *
 *  April 3 2025
 *  -Edited the encoder header and source file
 *  -Added actions for the encoders under the Port2 ISR for the encoders
 *  -Do we want the ISRs in one source file or in each of their respective source files?
 *
 *  April 4 2025
 *  -Made definitions for the starting states of the state machine
 *  -Started writing out the state machine while loops in main and their accompanying actions
 *  -Finished writing state 0 for the state machine
 *  -Added the CMDIntrepreter Lab source and header files and started adjusting them to this project
 *
 *  April 14 2025
 *  -Ran through the start up sequence and fixed some bugs involving the limit switches and the command interpreter
 *  -Debating about whether to use the motor driver in phase mode, more for sake of time
 *
 *  April 16 2025
 *  - Added timerA1 for sampling with the ultrasonic sensor along with a scan state flag
 *  - Also implemented an emergency flag and am currently trying to set up the emergency state
 *  - A little unsure where and when amongst the other states to make the program jump into the emergency state
 *
 *  April 23 2025
 *  -Implemented braking using the changeMode function. To brake a motor, simply change the mode to INPUT mode. When going back to PHASE mode, direction will need to be set before switching modes
 *  -Successfully ran through homing and scanning sequence with only the ring motor and putting the ring limit switch pin to ground
 *  -Noticing irregular ISR behaviour where ISRs are triggered as soon as all interrupts are enabled
 *
 *  April 29 2025
 *  -Implemented two switch statements for the state machine (one for the actions to take in the state machine, and another for transitioning
 *  between states)
 *
 *  May 1 2025
 *  -Added more states for miscellaneous information such as getting the distance from the ultrasonic and tilting the servo
 *  -Added another function to the UART serial source file and header for getting numbers from the user
 *
 * May 2 2025
 * -Edited the command interpreter to include commands that we want the user to be able to tell to the controller
 * -Adjusted the drive function to allow for direction rather than using a whole other function for direction
 * -Added a state machine definition header file to allow for nextState to be seen in ISRs
 *
 * May 16 2025
 *  -Due to being busy trying to get the project ready for the expo, notes about what has been modified have not been written down
 *  -This version is the latest updated version, prepared for the final report
 */

#include <msp430.h>
#include "stateMachineDef.h"
#include "motorDriver.h"
#include "timerA0UpMode.h"
#include "timerA1UpMode.h"
#include "usciUart.h"
#include "servo.h"
#include "ultrasonic.h"
#include "buttons.h"
#include "encoder.h"
#include "CMDInterpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int nextState = STATE_0_INIT;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    char consoleStr[500];

    //variables for keeping track of states
    unsigned int currentState = STATE_0_INIT;
    int userArg = 0; //variable for arguments the user will input (e.g. angle of servo)
    float ringPosData = 0.0; //variable for holding the current position of the ring data (for State 13)

    //variable for getting user's commands
    char userCommand[100] = {0};

    int index = -1; //initialize the command index
    int error = -1; //initialize the error variable

    //initiate UART communication with computer console
    usciA1UartInit();

    //initiate ultrasonic sensor
    initUltrasonic();

    //initiate the buttons
    initButtons();

    //inititiate sampling timer (sampling period of 50ms for now)
    timerA1UpModeInit((long int)SAMPLING_FREQUENCY);

    //initiate PWM timer (50 Hz)
    timerA0UpModeInit((long int)PWM_FREQUENCY);

    //initiate the servo
    initServo();

    //initiate the encoder pins and interrupts
    initEncoder();

    //initiate the command interpreter
    cmdsinit();

    //initiate the motors
    initMotor();

    //enable the sampling timer interrupt
    //TA1CTL |= TAIE;

    P1IFG = 0;
    P2IFG = 0;
    TA1CTL &= ~TAIFG;



    //enable all interrupts
    __enable_interrupt();


    while(1)
    {
        //update the current state to the next state
        if(currentState != nextState)
        {
            currentState = nextState;
        }
        else
        {
            //perform actions based on the state
            switch(currentState)
            {
                case STATE_0_INIT:              //initial state that prompts user to begin homing sequence

                usciA1UartTxString("Welcome to Face Scan Robot\n"); //Welcome the user
                __delay_cycles(1000000);  //delay for about a second
                usciA1UartTxString("Press Enter to start homing the system\n"); //prompt user to press enter to continue
                break;
                case STATE_1_HOME_RING:         //state to home the ring
                P1IE &= ~SOFT_STP;
                drive(RING_MOTOR,DRIVE_DUTY,CW); //drive the ring motor

                break;

                case STATE_2_HOME_RACKREAR:    //state to home the rack and pinion to the front side
                    drive(RACKPIN_MOTOR,100,CW);     //drive the rack and pinion motor
                break;

                case STATE_3_HOME_RACKFRONT:     //state to home the rack and pinion to the rear side
                drive(RACKPIN_MOTOR, 100,CCW);     //drive the rack and pinion motor
                break;

                case STATE_4_WAIT_USERCMD:      //state for displaying the menu and waiting for the user command
                usciA1UartTxString("Face Scanning Robot Menu\n"); //prompt user to select an action
                usciA1UartTxString("Please enter the character corresponding to the command you desire\n");
                usciA1UartTxString("S -> Begin Scanning\n\rT -> Tilt Servo\n\rD -> Get Distance\n\rP -> Get Position\n\rH -> Home\n\r");
                break;

                case STATE_5_SCAN:                      //state for commencing scanning a patient
                drive(RING_MOTOR,DRIVE_DUTY,CCW);        //drive the ring motor at the appropiate duty cycle
                scanFlg = 1;                            //set the scan flag high to indicate we are scanning for the ISR
                break;

                case STATE_6_TILT_SERVO:       //state for tilting the servo to a desired angular position
                usciA1UartTxString("Please enter the desired angle (0-180)\n");
                break;

                case STATE_7_GET_DISTANCE:     //state for getting the distance from the ultrasonic sensor
                getDistance(SLAVE_ADDRESS,&distanceData);   //get the distance from the ultrasonic sensor
                break;

                case STATE_8_GET_POSITION:     //state for getting the angular position of the ring
                //compute the angle from the pulse counts of the encoder
                getPos(RING_MOTOR,enc.count[RING_MOTOR],&ringPosData);
                break;

                case STATE_900_EMERGENCY:
                brake(RING_MOTOR);              //brake the ring motor
                brake(RACKPIN_MOTOR);           //brake the rack and pinion motor
                usciA1UartTxString("Please type \"C\" to continue the scanning\n"); //prompt user to select an action
                usciA1UartTxString("Otherwise type \"E\" to stop the scanning\n"); //prompt user to select an action
                break;

            }
            //Transition between states based on conditions
            switch(currentState)
            {
                case STATE_0_INIT:              //initial state that prompts user to begin homing sequence
                    //wait for user to press enter
                    usciA1UartGets(userCommand);
                    if(userCommand[0] == 0x0D)
                    {
                        //transistion to next state
                        nextState = STATE_1_HOME_RING;
                    }
                    else
                    {
                        //display error message if enter was not pressed
                        usciA1UartTxString("Please Press Enter to Continue\n"); //prompt the user to press enter again
                    }
                    break;

                case STATE_1_HOME_RING:         //state to home the ring

                    //poll the limit switch
                    while(P2IN&RING_SW);

                    //reverese the direction of the motor and then brake
                    drive(RING_MOTOR,DRIVE_DUTY,CW);
                    __delay_cycles(125000);
                    brake(RING_MOTOR);

                    //transition to the next state
                    nextState = STATE_2_HOME_RACKREAR;

                    //reset the encoder count
                    enc.count[RING_MOTOR] = 0; //reset the encoder count
                    break;

                case STATE_2_HOME_RACKREAR:    //state to home the rack and pinion to the front side

                    //poll the rear limit switch
                    while(P2IN&RACK_PIN_REAR);

                    //reverse the direction of the motor and then brake
                    drive(RACKPIN_MOTOR,100,CCW);
                    __delay_cycles(125000);
                    brake(RACKPIN_MOTOR);

                    //transistion to the next state
                    nextState = STATE_3_HOME_RACKFRONT;
                    break;

                    case STATE_3_HOME_RACKFRONT:     //state to home the rack and pinion to the rear side

                    //poll front limit switch
                    while(P2IN&RACK_PIN_FRONT);

                    //reverse the direction of the motor and then brake
                    drive(RACKPIN_MOTOR,100,CW);
                    __delay_cycles(125000);
                    brake(RACKPIN_MOTOR);

                    //transition to the next state
                    nextState = STATE_4_WAIT_USERCMD;

                    break;

                case STATE_4_WAIT_USERCMD:      //state for displaying the menu and waiting for the user command

                    //disable the emergency panic stop interrupt
                    P1IE &= ~SOFT_STP;

                    //get the command from the user
                    if(!usciA1UartGets(userCommand))
                    {
                        //print out error message
                        usciA1UartTxString("Error"); //UART error
                    }
                    else
                    {
                        index = parseCmd(userCommand);
                        //change state in command interpreter depending on user command
                        error = executeCMD(index);
                        if(error)
                        {
                            usciA1UartTxString(parseErrMsg); //print error message if unable to execute command
                        }

                    }
                    break;

                case STATE_5_SCAN:                   //state for commencing scanning a patient

                    //enable the panic button emergency interrupt
                    P1IE |= SOFT_STP;

                    //The following sequence of actions for scanning is more for testing purposes for the expo
                    //It will need to change to follow the state machine diagram
                    //Moves the ring until it reaches 180 degree around the patient
                    while((enc.count[RING_MOTOR] < 10700) && !emergencyFlg)
                    {
                        drive(RING_MOTOR,DRIVE_DUTY,CCW);
                    }
                    brake(RING_MOTOR);
                    __delay_cycles(125000);
                    //Moves the ring in the other direction until it reaches -180 degrees
                   while((enc.count[RING_MOTOR] > -10700)  && !emergencyFlg)
                    {
                        drive(RING_MOTOR,DRIVE_DUTY,CW);
                    }
                    brake(RING_MOTOR);
                    //Move the ring until it returns to the home condition
                    while(P2IN&RING_SW)
                    {
                        drive(RING_MOTOR,DRIVE_DUTY,CW);
                    }

                    //the section below is for keeping the camera at a constant distance from the patient's head
                    /*{
                        //brake the motors if any of the limit switches on the scanner module are triggered
                        if(!(P2IN&RACK_PIN_FRONT))
                        {
                            drive(RACKPIN_MOTOR,100,CW);
                            __delay_cycles(125000);
                            brake(RACKPIN_MOTOR);
                        }
                        else if(!(P2IN&RACK_PIN_REAR))
                        {
                           drive(RACKPIN_MOTOR,100,CCW);
                            __delay_cycles(125000);
                            brake(RACKPIN_MOTOR);
                        }
                        else
                        {
                            getDistance(SLAVE_ADDRESS,&distanceData);   //get the distance from the ultrasonic sensor
                            if(distanceData < (IDEAL_DISTANCE-TOLERANCE))
                            {
                                //move the camera forward
                                drive(RACKPIN_MOTOR,100,CCW);
                            }
                            else if(distanceData > (IDEAL_DISTANCE + TOLERANCE))
                            {
                                //reverse the camera
                                drive(RACKPIN_MOTOR,100,CW);
                            }
                            else
                            {
                                //brake the motor
                                brake(RACKPIN_MOTOR);
                            }

                        }
                    }*/
                    //brake both motors
                    brake(RING_MOTOR);
                    brake(RACKPIN_MOTOR);
                    scanFlg = 0; //reset the scan flag to zero to stop moving the rack and pinion
                    if(emergencyFlg)
                    {
                        nextState = STATE_900_EMERGENCY;
                    }
                    else
                    {
                        nextState = STATE_4_WAIT_USERCMD;
                    }

                    break;

                case STATE_6_TILT_SERVO:               //state for tilting the servo

                    //get the desired angle from the user
                    userArg = usciA1UartGetsNum(userCommand);

                    //check to see if the input angle is within range
                    if(userArg > SERVO_POS_MIN && userArg < SERVO_POS_MAX)
                    {
                        //position the servo to the desired angle
                        servoPos(userArg);
                        nextState = STATE_4_WAIT_USERCMD;
                    }
                    else
                    {
                        //display error message if the requested angle is outside of the limits
                        usciA1UartTxString("Invalid angle. Please enter an angle between SERVOMIN and SERVOMAX");
                    }
                    break;

                case STATE_7_GET_DISTANCE:             //state for getting the distance data from the ultrasonic

                    //prepare the string to be transferred to the console
                    sprintf(consoleStr, "Distance in mm: ");
                    sprintf(consoleStr + strlen(consoleStr), "%lf",(float)distanceData);
                    //write the string to the console
                    usciA1UartTxString(consoleStr);
                    usciA1UartTxChar('\n');
                    usciA1UartTxChar('\r');

                    //transition to the next state
                    nextState = STATE_4_WAIT_USERCMD;
                    break;

                case STATE_8_GET_POSITION:             //state for getting the angular position of the ring

                    //prepare the string to be transferred to the console
                    sprintf(consoleStr, "Angular position of ring: ");
                    sprintf(consoleStr + strlen(consoleStr), "%lf", ringPosData);
                    //write the string to the console
                    usciA1UartTxString(consoleStr);
                    usciA1UartTxChar('\n');
                    usciA1UartTxChar('\r');

                    //transition to the next state
                    nextState = STATE_4_WAIT_USERCMD;
                    break;

                case STATE_900_EMERGENCY:               //emergency state

                    //get the user command
                    if(!usciA1UartGets(userCommand))
                    {
                        //print out error message
                        usciA1UartTxString("Error"); //UART error
                    }
                    else
                    {
                        index = parseCmd(userCommand);
                        //only execute if the command is for continuing the scan or exiting the scan
                        if(index == 1 || index == 2)
                        {
                            //change state in command interpreter depending on user command
                            error = executeCMD(index);
                            if(error)
                            {
                                usciA1UartTxString(parseErrMsg); //print error message if unable to execute command
                            }
                        }
                        else
                        {
                            //Prompt the user again to either exit or continue the scan
                            usciA1UartTxString("Please enter \"E\" to exit or \"C\" to continue scanning\n");
                        }
                    }
                    break;

            }
        }
    }
    return 0;
}
