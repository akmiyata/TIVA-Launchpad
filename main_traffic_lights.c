/*Requirements:
/* Requirements:
* 1. Button 1- West car sensor; if pressed, traffic light turns from red to green (after south lights have cycled to Red).
* 2. Button 2- South car sensor; if pressed, traffic light turns from red to green (after south lights have cycled to Red).
* 3. Button 3- Walk button; if pressed, both west and south traffic lights must cycle to red, and the led on the PCB will turn from red to green.
* 4. Lights will stay in state from "3." until a car is sensed at one of the traffic lights (i.e. button 1 or button 2 is pressed).
*/
// Steps & Pseudo
/* 1. Setup hardware- In progress (waiting on buttons to arrive)
*  Needed: 3 switches (order at least two more); 2 each of green, yellow, red LEDS;  
*  several wires (detail later); six 220 ohm resistors for LEDS; 3 10K ohm resistors for switch inputs
*  Red wire- +3.3V power
*  PB5 (White wire)- (powers green light for west street)
*  PB0 (Green wire1)- (powers green light for west street)
*  PB1 (Yellow wire1)- (powers yellow light for west street)
*  
*  2. Define/understand requirements- Complete
*  3. Code- In progress
*  4. Test- After coding */

// 1. Pre-processor directives
// Initialize Ports B & F
void PortB_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00100010; // Port B AND F clock
	delay = SYSCTL_RCGC2_R; // delay
	//GPIO_PORTA_CR_R = 0x1F; // Allow changes to PA4-0
	//GPIO_PORTA_AMSEL_R &= ~0x80; // Disable analog function on PA7
	//GPIO_PORTA_PCTL_R &= ~0xF0000000; // PCTL GPIO PA7
	GPIO_PORTB_DIR_R = 0x3F; // PB7 & 6 input, PB5-0 output
	//GPIO_PORTA_AFSEL_R &= ~0x80; // No alternate function on PA7
	GPIO_PORTB_DEN_R = 0xFF; // Enable digital I/O on PA7 & PA5
}

//  Subroutines

int main(void){
  POrtB_Init(); 
}
