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
*  PE2 (blue wire)- Input for crosswalk
*  2. Define/understand requirements- COMPLETE
*  3. Code- In progress
*  4. Test- After coding 
*/

/* Pre-processor directives*/
#include "TM4C123G_AKM.h" /* Custom header with Port B, E, and F register pointers */
#include <stdint.h>

#define SENSOR     (*((volatile uint32_t *)0x4002401C)) /* Inputs from PE0, PE1, and PE2 */
#define LIGHT      (*((volatile uint32_t *)0x400050FC))
#define LIGHTBOARD (*((volatile uint32_t *)0x400253FC)) /* Green & red LEDs on board (PF1 & PF3) */
#define PE2        (*((volatile uint32_t *)0x40024010)) /* PE2 input */	
#define goW       0
#define waitW     1
#define goS       2
#define waitS     3
#define goPed     4
#define hurryPed  5
#define hurryPed1 6
#define hurryPed2 7
#define hurryPed3 8
#define hurryPed4 9

/* Linked data structure */
struct State {
	uint32_t Out; /* Output for traffic lights */
	uint32_t OutBoard; /* Output for crosswalk */
	uint32_t Time;
	uint32_t Next[8];};

typedef const struct State State_t;
	
State_t FSM[10]={
{0x21,0x02,3000,{goW,waitW,goW,waitW,waitW,waitW,waitW,waitW}},
{0x22,0x02,500,{goS,goS,goS,goS,goPed,goS,goPed,goS}},
{0x0C,0x02,3000,{goS,goS,waitS,waitS,waitS,waitS,waitS,waitS}},
{0x14,0x02,500,{goPed,goPed,goW,goW,goPed,goPed,goPed,goPed}},
{0x24,0x08,3000,{goPed,hurryPed,hurryPed,hurryPed,goPed,hurryPed,hurryPed,hurryPed}},
{0x24,0x02,500,{hurryPed1,hurryPed1,hurryPed1,hurryPed1,hurryPed1,hurryPed1,hurryPed1,hurryPed1}},
{0x24,0x00,500,{hurryPed2,hurryPed2,hurryPed2,hurryPed2,hurryPed2,hurryPed2,hurryPed2,hurryPed2}},
{0x24,0x02,500,{hurryPed3,hurryPed3,hurryPed3,hurryPed3,hurryPed3,hurryPed3,hurryPed3,hurryPed3}},
{0x24,0x00,500,{hurryPed4,hurryPed4,hurryPed4,hurryPed4,hurryPed4,hurryPed4,hurryPed4,hurryPed4}},
{0x24,0x02,500,{goW,goS,goW,goW,goW,goS,goW,goW}}};

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
  SYSCTL_RCGC2_R |= 0x00000032; /* Enable clock for Port B, E & F */
	delay = SYSCTL_RCGC2_R; /* delay */
	GPIO_PORTB_DIR_R |= 0x3F; /* PB5-0 output */
	GPIO_PORTB_DEN_R |= 0x3F; /* Enable digital I/O on PB5 through PB0 */
	GPIO_PORTE_DIR_R &= ~0x07; /* PE0, PE1, & PE2 are inputs */
	GPIO_PORTE_DEN_R |= 0x07; /* PE0, PE1, & PE2 enable digital I/O */
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   /* 2) unlock GPIO Port F */
  GPIO_PORTF_CR_R = 0x1F;           /* allow changes to PF4-0 */
	GPIO_PORTF_DIR_R |= 0x0A; /* PF1 & PF3 are outputs (red/green crosswalk light) */
	GPIO_PORTF_DEN_R |= 0x0A; /* PF1 & PF3 enable digital I/O */
	S = goW;
	while(1)
		 {
			LIGHTBOARD = FSM[S].OutBoard; /* 0x02 for red */
			LIGHT = FSM[S].Out; /* Set lights */
			Delay();
			Input = SENSOR; /* Read input */
			S = FSM[S].Next[Input];
     }
	
	}
