/*************************************************************************************************
 * usciUart.c
 * - C implementation or source file for MSP430 UCSI UART A1
 *
 *  Author: Greg Scutt
 *  Created on: March 1, 2017
 *  Modified: February 26th, 2018
 **************************************************************************************************/

#include <msp430.h>
#include "usciUart.h"
#include <string.h>
#include <stdlib.h>

char rxBuffer[BUFF_SZ];
int buffIndex = 0;

/************************************************************************************
* Function: usciA1UartInit
* - configures UCA1 UART to use SMCLK, no parity, 8 bit data, LSB first, one stop bit
*  BAUD rate = 19.2Kbps with 16xoversampling.
*  assumes SMCLK = 2^20 Hz.
* argument:
* Arguments: none, but baud rate would be useful
*
* return: none
* Author: Greg Scutt
* Date: March 1st, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void usciA1UartInit(){

    // READ THIS --> You must use UCSI Control BITS defined in msp430.h !!
    // In fact you must use pre-defined control bits whenever possible. no obscure HEX codes allowed anymore.

    P4SEL |= TXD_A1 | RXD_A1;
    UCA1CTL1 |= UCSWRST;            // USCI A1  Software Reset Enabled
    //********************

    UCA1CTL1    |=  UCSSEL_2;       // select SMCLK. User is responsible for setting this rate.

    UCA1CTL0     =  0;              // RESET UCA1CTL0 before new configuration
    UCA1CTL0    &=  ~UCPEN          // No Parity
                &   ~UCMSB          // LSB First
                &   ~UC7BIT         // 8 bits of data
                &   ~UCSPB          // 1 stop bit
                &   ~UCSYNC;        // UART Mode

    //Must change depending on baud rate. Ultrasonic requires 9600bps
    UCA1BR1 = 0x00; // calculate pre-scaler
    UCA1BR0 = 0x03; // calculate pre-scaler
    UCA1MCTL = UCBRF_6| UCBRS_1  | UCOS16;  // calculate first and second stage modulation
    //UCA1MCTL = 0x63;   // calculate first and second stage modulation

    UCA1CTL1    &= ~UCSWRST;        //  configured. take state machine out of reset.
    }


/************************************************************************************
* Function: usciA1UartTxChar
* - writes a single character to UCA1TXBUF if it is ready
* argument:
* Arguments: txChar - byte to be transmitted
*
* return: none
* Author: Greg Scutt
* Date: March 1st, 2017
************************************************************************************/
void usciA1UartTxChar(char txChar) {

    while (!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = txChar;  // if TXBUF ready then transmit a byte by writing to it

}


/************************************************************************************
* Function: usciA1UartTxString
* - writes a C string of characters, one char at a time to UCA1TXBUF by calling
*   usciA1UartTxChar. Stops when it encounters  the NULL character in the string
*   does NOT transmit the NULL character
* argument:
* Arguments: txChar - pointer to char (string) to be transmitted
*
* return: number of characters transmitted
* Author: Greg Scutt
* Date: March 1st, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void usciA1UartTxString(char *txChar){
    volatile int i = 0;

    // transmit a character with usciA1UartTxChar and increment the pointer
    while(txChar[i] != 0)
    {

        while (!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = txChar[i];  // if TXBUF ready then transmit a byte by writing to it
        i++;
        //add a new line and carriage return when the string has been printed out
        if(txChar[i] == 0)
        {
            while (!(UCA1IFG & UCTXIFG));
            UCA1TXBUF = NEW_LINE;
            while (!(UCA1IFG & UCTXIFG));
            UCA1TXBUF = CARR_RET;
        }
    }


    }

/************************************************************************************
* Function: usciA1UartTXBuffer
* - transmit a certain amount of characters and return the number of bytes transmitted
* argument:
* Arguments: buffer - string that will be buffered for transmission
*            bufLen- amount of characters that will be transferred
*
* return: number of characters transmitted
* Author: Greg Scutt
* Date: March 1st, 2017
************************************************************************************/

int usciA1UartTXBuffer(char * buffer, int bufLen){

    volatile int i = 0; //counter for tracking the number of characters specified by bufLen
    while(i < bufLen)
    {
        while (!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = buffer[i];
        i++;
    }

    //Create a new line
    while (!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = 0x0A;
    while (!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = 0x0D;

    //check if the desired amount of characters were transmitted
    if(i != bufLen)
    {
        i= 0;
    }
    return i;
}


/************************************************************************************
* Function: usciA1UartGets
* - Poll for user input and then transmit
* argument:
* Arguments: buffer - string that will be buffered for transmission
*            bufLen- amount of characters that will be transferred
*
* return: pointer to rxString or Null
* Author: Greg Scutt
* Date: March 1st, 2017
************************************************************************************/
char* usciA1UartGets(char *rxString){
    volatile int i = 0;

    do{
        while(!(UCA1IFG & UCRXIFG)); //wait until the receive flag is set
        rxBuffer[i++] = UCA1RXBUF;  //put the character received into the array
        usciA1UartTxChar(UCA1RXBUF); //transmit the character onto the terminal

    }while((UCA1RXBUF != CARR_RET) && (i < BUFF_SZ)); //loop until user presses enter or size of buffer exceeds it's limit


    rxBuffer[i] = 0x00; //insert a null character to make it a string

    strcpy(rxString,rxBuffer);

    return rxString;
}


/************************************************************************************
* Function: usciA1UartGetsNum
* - Poll for user input number and transmits it
* argument:
* Arguments: rxString - string containing the user command
* return: number - int containing the number the user inputted
* Author: Jonathan Ina
* Date: May 1 2025
************************************************************************************/
int usciA1UartGetsNum(char *rxString)
{
    volatile int i = 0; //array index
    int number = 0; //number to return

    do
    {
        while(!(UCA1IFG & UCRXIFG));    //wait until it has received a character
        rxBuffer[i++] = UCA1RXBUF;      //put the received character into the buffer
        usciA1UartTxChar(UCA1RXBUF);    //transmit the character for display purposes
    }while((UCA1RXBUF != CARR_RET) && (i < BUFF_SZ)); //loop until user presses enter or size of buffer exceeds it's limit

    rxBuffer[i] = 0x00; //insert a null to make it a string
    number = atoi(rxBuffer); //convert the string into an integer

    return number; //return the number
}



/*#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;
  case 2:
      //while (!(UCA1IFG & UCTXIFG));
      //UCA1TXBUF = UCA1RXBUF;
      rxBuffer[buffIndex] = UCA1RXBUF;
      buffIndex++;
    break;
  case 4:break;
  default: break;
  }
}
*/

