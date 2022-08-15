/* This program uses UART0 on the TM4C123G to communicate with with my PC using PuTTY to connect to my serial COM port. Entering "r" as input
* from the keyboard will turn on the red onboard LED, "g" turns on the green LED, "b" turns on the blue LED, and any other input will turn the 
LEDs off.
*/

/* Pre-processor directives */
#include "TM4C123.h"

/* Function prototypes */
char readChar(void);
void printChar(char c);
void printString(char * string);

int main(void)
{
	  char c;
	  SYSCTL->RCGCUART |= (1<<0);  /* Enable clock to UART0 */
    SYSCTL->RCGCGPIO |= (1<<0);  /* Enable clock for PORTA */
	
    GPIOA->AFSEL = (1<<1)|(1<<0);          /* Turn on alternate functions of PA0 and PA1 */
	  GPIOA->PCTL = (1<<0)|(1<<4);
	  GPIOA->DEN = (1<<0)|(1<<1);
	  UART0->CTL &= ~(1<<0);                 /* Disable UART0  */
    UART0->IBRD = 104;                     /* For a 9600 baud rate, integer BRD = 104 */
    UART0->FBRD = 11;                      /* For a 9600 baud rate, fractional BRD = 11 */
    UART0->CC = 0x0;                       /* Select system clock */
    UART0->LCRH = (0x3<<5);                /* Data lenght 8-bit, no parity bit, one stop bit */
    UART0->CTL = (1<<0)|(1<<8)|(1<<9);     /* Enable UART0 module, Rx and Tx */

    SYSCTL->RCGCGPIO |= (1<<5);
    GPIOF->DIR = (1<<1)|(1<<2)|(1<<3);      /* Set PF1 PF2 & PF3 as outputs */
    GPIOF->DEN = (1<<1)|(1<<2)|(1<<3);    /* Enable digital on LED pins */
    GPIOF->DATA &= ~((1<<1)|(1<<2)|(1<<3));    /* Turn off LEDs*/
    
	while(1)
 {
	printString("Enter \"r\", \"g\", or \"b\":\n\r");

  c = readChar();
	printChar(c);
	printString("\n\r");
	switch(c)
	{
		case 'r':
			GPIOF->DATA = (1<<1); /* Turn on red LED */
		  break;
		case 'b':
			GPIOF->DATA = (1<<2); /* Turn on blue LED*/
		  break;
		case 'g':
			GPIOF->DATA = (1<<3); /* Turn on green LED */
		  break;
		default:
			GPIOF->DATA = ~((1<<1)|(1<<2)|(1<<3)); /* Turn off LEDs */
		  break;
	}
 }
}

char readChar(void)
{
	char c;
	while((UART0->FR & (1<<4)) != 0);
	c = UART0->DR;
	return c;
}

void printChar(char c)
{
	while((UART0->FR & (1<<5)) != 0);
	UART0->DR = c;
}

void printString(char * string)
{
	while(*string)
		printChar(*(string++));
}
