/*
 * buttons.h
 *  Description: Header file for dealing with all buttons
 *  Created on: Mar. 22, 2025
 *      Author: Jonathan Ina
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

//Limit switch pin definitions
#define RACK_PIN_FRONT BIT0  //Limit switch for front of scanner module (P2.0)
#define RACK_PIN_REAR BIT3   //Limit switch for rear of scanner module (P2.3)
#define LEAD_SCREW_TOP BIT4  //Limit switch for top of lead screw (2.4)
#define LEAD_SCREW_BOT BIT5  //Limit switch for bottom of lead screw (2.5)
#define RING_SW BIT7         //Limit switch for ring (P2.7)

//Patient E-Stop pin definitions
#define SOFT_STP BIT6 //Push button for soft e-stop (P1.6)

//flag bit for indicating when an emergency state has been triggered
extern unsigned char emergencyFlg;

void initButtons();

#endif /* BUTTONS_H_ */
