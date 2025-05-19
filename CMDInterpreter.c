/*
 * UsciCmdInterpreter.c
 *
 *  Created on: Jan. 9, 2022
 *      Author: Andrew Friesen
 *  Modified on: May 2 2025
 */

#include "CmdInterpreter.h"
#include <msp430.h>
#include "buttons.h"
#include "stateMachineDef.h"
#include <string.h>
#include "usciUart.h"
#include <stdlib.h>
#include <stdio.h>

//struct for interpretting the commands
CMD cmds[MAX_CMDS]={0};
/*
 * Name: cmdsinit
 *
 * Description: Initializes the list of commands for the command interpreter
 *
 * Inputs: none
 * Returns: none
 */
void cmdsinit()
{
    //Load command list elements here

    cmds[0].name = STARTCMD;

    cmds[1].name = EXITCMD;

    cmds[2].name = CONTINUECMD;

    cmds[3].name = POSCMD;

    cmds[4].name = TILTCMD;

    cmds[5].name = DISTANCECMD;

    cmds[6].name = HOMECMD;

}

/*
 * Name: parseCmd
 *
 * Description:
 * Parses string for command name and arguments. Validates command by calling "validateCmd". If the command is valid
 * the function will parse the remaining args in the string into the of args array of the CMD struct. If the number of args
 * is not the same as nArgs for the command the function will return an error as the command is invalid.
 *
 * Input: [char *] String containing command name and comma delimited arguments.
 * Returns: [int] The command number. (It's element in the array of commands.) If the command is invalid return -1.
 */
int parseCmd(char * cmdLine)
{
    volatile unsigned int i = 0;
    int cmdInd = 0; //variable for the command index
    char cmBuff[BUFF_SZ] = {0}; //buffer for the command

    //Process command string into tokens for command name and any arguments for the command.
    do{
        cmBuff[i] = cmdLine[i];
        i++;
    }while((cmdLine[i-1] != ' ') && (cmdLine[i-1] != ',') && (cmdLine[i-1] != 9) && (cmdLine[i-1] != CARR_RET));

    cmBuff[i-1] = 0; //make the last character before the argument null

    cmdInd = validateCmd(cmBuff); //validate command and determine command index

    return cmdInd;
}

/*
 * Name: validateCmd
 *
 * Function:
 * Called by "parseCmd" to compare the "command name" portion of the string being parsed against the list of commands.
 *
 * Input: [char *] The "command name" portion of a command string
 * Returns: [int] Returns the index of the command in the command list array for valid commands. Returns -1 for invalid commands.
 */
int validateCmd(char * cmdName)
{
    int i = 0; //counter for going through list of commands
    int index = 0; //index for knowing which command is being requested
    int invalidCmd = 1; //variable for comparing if the user string is the same as the command string from list

    //Compare the name string token to the list of command names in your cmds array.
    do{
        invalidCmd = strcmp(cmdName,cmds[i++].name);
    }while((i < MAX_CMDS) && invalidCmd);

    if(!invalidCmd)
    {
        index = i - 1; //subtract by 1 to get the actual command index for the array
    }
    else
    {
        index = -1; //return -1 if there is an error
    }

    return index;
}

/*
 * Name: executeCMD
 *
 * Function:
 * Executes the selected command.
 *
 * Input: [int] The command number. (It's element in the array of commands.)
 * Return: [int] Return 0 if the command executes correctly. Return 1 for an invalid command.
 */
int executeCMD(int cmdIndex)
{
    int check = 0;
    //execute commands base off of cmdIndex
    switch(cmdIndex)
    {
    case 0:
    case 2:
        nextState = STATE_5_SCAN; //transition to State 3 Scan
        emergencyFlg = 0;         //reset the emergency flag to indicate it is no longer in emergenecy
        break;
    case 1:
        nextState = STATE_4_WAIT_USERCMD; //transistion to State 2 Wait for User Command
        emergencyFlg = 0;   //reset the emergency flag to indicate it is no longer in emergency
        break;
    case 3:           //transistion to State 8 for getting the position from the encoders
        nextState = STATE_8_GET_POSITION;
        break;
    case 4:         //transistion to State 6 for changing the angle of the servo
        nextState = STATE_6_TILT_SERVO;
        break;
    case 5:         //transition to State 7 to get the distance from the ultrasonic sensor
        nextState = STATE_7_GET_DISTANCE;
        break;
    case 6:         //transistion to State 1 to home the system
        nextState = STATE_1_HOME_RING;
        break;
    default:
        usciA1UartTxString(parseErrMsg);
        break;
    }

    return check;
}
