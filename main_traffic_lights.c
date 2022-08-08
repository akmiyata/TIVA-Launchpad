/* Requirements:
* 1. Button 1- West car sensor; if pressed, traffic light turns from red to green (after south lights have cycled to Red).
* 2. Button 2- South car sensor; if pressed, traffic light turns from red to green (after south lights have cycled to Red).
* 3. Button 3- Walk button; if pressed, both west and south traffic lights must cycle to red, and the led on the PCB will turn from red to green.
* 4. Lights will stay in state from "3." until a car is sensed at one of the traffic lights (i.e. button 1 or button 2 is pressed).
*
*
*  1. Setup hardware- COMPLETE
*  Needed: 3 switches (order at least two more); 2 each of green, yellow, red LEDS;  
*  several wires (detail later); six 220 ohm resistors for LEDS; 3 10K ohm resistors for switch inputs
*  
*  Red wire- +3.3V power
*  Black wire- GND 
*  
*  PB0 (green wire)- (powers green light for west street)
*  PB1 (yellow wire1)- (powers yellow light for west street)
*  PB2 (white wire1)- (powers red light for west street)
*  
*  PB3 (green wire)- powers green light for south street
*  PB4 (yellow wire1)- powers yellow light for south street
*  PB5 (white wire1)- powers red light for south street
*  
*  PE0 (blue wire)- Input for west street car sensor 
*  PE1 (blue wire)- Input for south street car sensor
*  PA7 (blue wire)- Input for crosswalk
*  2. Define/understand requirements- COMPLETE
*  3. Code- In progress
*  4. Test- After coding 
*/

/* Pre-processor directives*/
#include "TM4C123G_AKM.h" /* Custom header with Port B, E, and F register pointers */
#include <stdint.h>

#define SENSOR  (*((volatile uint32_t *)0x4002400C))
#define LIGHT   (*((volatile uint32_t *)0x400050FC))
#define goW    0
#define waitW  1
#define goS    2
#define waitS  3

/* Linked data structure */
struct State {
	uint32_t Out;
	uint32_t Time;
	uint32_t Next[4];};

typedef const struct State State_t;
	
State_t FSM[4]={
{0x21,3000,{goW,waitW,goW,waitW}},
{0x22, 500,{goS,goS,goS,goS}},
{0x0C,3000,{goS,goS,waitS,waitS}},
{0x14, 500,{goW,goW,goW,goW}}};

 uint32_t S; /* Index to the current state */
 uint32_t Input;
/* Function prototypes */
void Delay(void);

void Delay(void){
	int i = 0;
	for(i = 0;i<1000000;i++){
	}
}

int main(void){ volatile uint32_t delay;
  SYSCTL_RCGC2_R |= 0x00000013; /* Enable clock for Port A, B, & E */
	delay = SYSCTL_RCGC2_R; /* delay */
	GPIO_PORTA_DIR_R &= ~0x20; /* PA5 input */
	GPIO_PORTA_DEN_R |= 0x20; /* PA5 digital enable */
	GPIO_PORTB_DIR_R |= 0x3F; /* PB5-0 output */
	GPIO_PORTB_DEN_R |= 0x3F; /* Enable digital I/O on PB5 through PB0 */
	GPIO_PORTE_DIR_R &= ~0x03; /* PE0 & PE1 are inputs */
	GPIO_PORTE_DEN_R |= 0x03; /* PE0 & PE1 enable digital I/O */
	S = goW;
	while(1){
		LIGHT = FSM[S].Out; /* Set lights */
		Delay();
		Input = SENSOR; /* Read input */
		S = FSM[S].Next[Input];
		}
	}
