/* Requirements:
* 1. Button 1- West car sensor; if pressed, traffic light turns from red to green (after south lights have cycled to Red).
* 2. Button 2- South car sensor; if pressed, traffic light turns from red to green (after south lights have cycled to Red).
* 3. Button 3- Walk button; if pressed, both west and south traffic lights must cycle to red, and the led on the PCB will turn from red to green.
* 4. Lights will stay in state from "3." until a car is sensed at one of the traffic lights (i.e. button 1 or button 2 is pressed).
*/

/* 1. Setup hardware- COMPLETE
*  Needed: 3 switches (order at least two more); 2 each of green, yellow, red LEDS;  
*  several wires (detail later); six 220 ohm resistors for LEDS; 3 10K ohm resistors for switch inputs
*  Red wire- +3.3V power
*  Black wire- GND 
*  PB0 (green wire)- (powers green light for west street)
*  PB1 (yellow wire1)- (powers yellow light for west street)
*  PB2 (white wire1)- (powers red light for west street)
*  
*  PB3 (green wire)- powers green light for south street
*  PB4 (yellow wire1)- powers yellow light for south street
*  PB5 (white wire1)- powers red light for south street
*  
*  PB6 (blue wire)- Input for west street car sensor 
*  PB7 (blue wire)- Input for south street car sensor
*  PE2 (blue wire)- Input for crosswalk
*  2. Define/understand requirements- COMPLETE
*  3. Code- In progress
*  4. Test- After coding 
*/

// 1. Pre-processor directives
#include <TM4C123G_AKM.h> // Custom header with Port B and F register pointers
// Initialize Ports B & F
void PortB_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00100010; // Enabling clocks for both Port B AND F
	delay = SYSCTL_RCGC2_R; // delay
	//GPIO_PORTA_CR_R = 0x1F; // Allow changes to PA4-0
	//GPIO_PORTA_AMSEL_R &= ~0x80; // Disable analog function on PA7
	//GPIO_PORTA_PCTL_R &= ~0xF0000000; // PCTL GPIO PA7
	GPIO_PORTB_DIR_R = 0x3F; // PB7 & 6 input, PB5-0 output
	//GPIO_PORTA_AFSEL_R &= ~0x80; // No alternate function on PA7
	GPIO_PORTB_DEN_R = 0xFF; // Enable digital I/O on PB7 through PB0
}
// REMOVE DEAD CODE ONCE WORKING; DO NOT BLUE LED FOR THIS PROJECT, REMOVE CODE RELATED TO BLUE LED
void PortF_Init(void){ volatile unsigned long delay;
//	SYSCTL_RCGC2_R |= 0x00100010; // Port B AND F clock
//	delay = SYSCTL_RCGC2_R; // delay
	//GPIO_PORTA_CR_R = 0x1F; // Allow changes to PA4-0
	//GPIO_PORTA_AMSEL_R &= ~0x80; // Disable analog function on PA7
	//GPIO_PORTA_PCTL_R &= ~0xF0000000; // PCTL GPIO PA7
	GPIO_PORTF_DIR_R = 0x0D; // PF3 through PF1 output (onboard LEDs) ELIMINATE BLUE LED
	//GPIO_PORTA_AFSEL_R &= ~0x80; // No alternate function on PA7
	GPIO_PORTB_DEN_R = 0x0D; // Enable digital I/O on PF3 through PF1
}

//  Subroutines

int main(void){
  PortB_Init();
  PortF_Init();	
}
