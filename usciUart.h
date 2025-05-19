/*************************************************************************************************
 * usciUart.h
 * - - C interface file for MSP430 USCI UART A1, A0
 *
 *  Author: Greg Scutt
 *  Created on: March 1, 2017
 *
 *  Pinouts for UART Communication
 *  P4.4-TXD
 *  P4.5-RXD
 **************************************************************************************************/

#ifndef USCIUART_H_
#define USCIUART_H_

#define testString "BCIT Mechatronics"

#define     _UART_A1PSEL    P4SEL |= PM_UCA1TXD | PM_UCA1RXD // use macros for short expressions.

#define     TXD_A1          BIT4            //Transmit Data on P4.4
#define     RXD_A1          BIT5            //Recieve Data on P4.5
#define BUFF_SZ 100
#define NEW_LINE 0x0A
#define CARR_RET 0x0D

void usciA1UartInit();
void usciA1UartTxChar(char txChar);
void usciA1UartTxString(char * txChar);
int usciA1UartTXBuffer(char * buffer, int bufLen);
char* usciA1UartGets(char *rxString);
int usciA1UartGetsNum(char *rxString);

extern char rxBuffer[BUFF_SZ];
extern int buffIndex;

#endif /* USCIUART_H_ */
