/*
 * ultrasonic.c
 *  Description: Source file for transmitting and receiving data from Ultrasonic Sensor
 *  Created on: Mar. 19, 2025
 *      Author: Jonathan Ina
 */

#include <msp430.h>
#include "ultrasonic.h"


/*-------------------------------------------------------------------------------------
 * Function Name: initUltrasonic
 * Description: sets up UART commmunication with Ultrasonic sensor
 * Arguments: N/A
 * Type: void
 * Returns: nothing
 --------------------------------------------------------------------------------------*/
void initUltrasonic()
{
    P3SEL |= ULTRATX | ULTRARX;
    P3DIR |= ULTRATX;
    UCA0CTL1 |= UCSWRST;            // USCI A0  Software Reset Enabled
    //********************

    UCA0CTL1    |=  UCSSEL_2;       // select SMCLK. User is responsible for setting this rate.

    UCA0CTL0     =  0;              // RESET UCA1CTL0 before new configuration
    UCA0CTL0    &=  ~UCPEN          // No Parity
                &   ~UCMSB          // LSB First
                &   ~UC7BIT         // 8 bits of data
                &   ~UCSPB          // 1 stop bit
                &   ~UCSYNC;        // UART Mode

    //Must change depending on baud rate. Ultrasonic requires 9600bps. (Is the clock frequency 1MHz or 1.048MHz?)
    UCA0BR1 = 0x00; // calculate pre-scaler
    UCA0BR0 = 0x06; // calculate pre-scaler
    UCA0MCTL = UCBRF_13 | UCOS16;  // calculate first and second stage modulation and put into oversampling mode
    //UCA1MCTL = 0x63;   // calculate first and second stage modulation

    UCA0CTL1    &= ~UCSWRST;        //  configured. take state machine out of reset.

}

/*-------------------------------------------------------------------------------------
 * Function Name: getDistance
 * Description: gets the distance data from the ultrasonic sensor
 * Arguments: slvAdd - address of the slave(ultrasonic sensor) device
 *            distance - char pointer to hold the distance data
 * Type: int
 * Returns: check - ensures an accurate distance was measured. 0->good, -1->bad
 --------------------------------------------------------------------------------------*/
int getDistance(char slvAdd, unsigned char * distance)
{
    int check = 0;

    //transmit the slave address
    while(!(UCTXIFG&UCA0IFG));
    UCA0TXBUF = slvAdd;

    //get the first byte of data
    while(!(UCRXIFG&UCA0IFG));
    *(distance+1) = UCA0RXBUF; //store the incoming data into distance

   while(!(UCRXIFG&UCA0IFG));

   *(distance) = UCA0RXBUF; //store the incoming data into distance

    return check;

}
