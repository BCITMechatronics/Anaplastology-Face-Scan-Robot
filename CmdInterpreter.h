/*
 * UsciCmdInterpreter.h
 *
 *  Description: Header file for UART Serial Console Control
 *  Created on: Jan. 9, 2022
 *      Author: Andrew Friesen
 *    Modified: March 15 2025
 *    Modified by Jonathan Ina - Added more custom commands for controlling the operation of the robot
 */

#ifndef CMDINTERPRETER_H_
#define CMDINTERPRETER_H_

#define MAX_CMDS 7
#define STARTCMD "S"
#define EXITCMD "E"
#define CONTINUECMD "C"
#define POSCMD "P"
#define TILTCMD "T"
#define DISTANCECMD "D"
#define HOMECMD "H"
#define parseErrMsg  "Invalid Command"


typedef struct CMD {
    const char *name; //command name
}CMD;


void cmdsinit();


int parseCmd(char * cmdLine);


int validateCmd(char * cmdName);


int executeCMD(int cmdIndex);

#endif /* CMDINTERPRETER_H_ */
