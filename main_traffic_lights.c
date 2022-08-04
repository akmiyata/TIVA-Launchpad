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

// Pre-processor directives
#include "TM4C123G_AKM.h" // Custom header with Port B, E, and F register pointers
#include <stdint.h>

#define PB7                 (*((volatile uint32_t *)0x4000E010))
#define SYSCTL_RCC2_R       (*((volatile uint32_t *)0xE000E010))

#define NVIC_ST_CTRL_R      (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile uint32_t *)0xE000E018))

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}

// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(uint32_t delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}
// 800000*12.5ns equals 10ms
void SysTick_Wait10ms(uint32_t delay){
  uint32_t i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 10ms
  }
}

//  Subroutines
void PLL_Init(void){
  SYSCTL_RCC2_R |=  0x80000000;  // USE RCC2 
  SYSCTL_RCC2_R |=  0x00000800;  // BYPASS2, PLL bypass
  SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)   // clear XTAL field, bits 10-6
                 + 0x00000540;   // 10101, configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~0x00000070;  // configure for main oscillator source
  SYSCTL_RCC2_R &= ~0x00002000;  // activate PLL by clearing PWRDN
  SYSCTL_RCC2_R |= 0x40000000;   // set the desired system divider; use 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  // clear system clock divider
                  + (4<<22);      // configure for 80 MHz clock
  while((SYSCTL_RIS_R&0x00000040)==0){};  // wait for the PLL to lock by polling PLLRIS
  SYSCTL_RCC2_R &= ~0x00000800;  // enable use of PLL by clearing BYPASS
}



int main(void){ volatile uint32_t delay;
	PLL_Init();
	SysTick_Init();
  SYSCTL_RCGC2_R |= 0x00000002; // Enable clock for Port B
	delay = SYSCTL_RCGC2_R; // delay
	GPIO_PORTB_DIR_R |= 0x3F; // PB7 & 6 input, PB5-0 output
	GPIO_PORTB_DEN_R |= 0x3F; // Enable digital I/O on PB5 through PB0
	GPIO_PORTE_DIR_R &= 0x03; // PE0 & PE1 are inputs
	GPIO_PORTE_DEN_R |= 0x03; // PE0 & PE1 enable digital I/O
	while(1){
		GPIO_PORTB_DATA_R = 0x0C; // Initially, west traffic lights are red, south are green, and onboard LED is Red
		SysTick_Wait10ms(500);
		GPIO_PORTB_DATA_R = 0x14; // When car is sensed (button pushed), transition to west light red, south light yellow
		SysTick_Wait10ms(500);
		GPIO_PORTB_DATA_R = 0x21; // When car is sensed (button pushed), to finish transition, west light is green, south light red
		SysTick_Wait10ms(500);
	}
}

int main(void){
  PortB_Init();
  PortF_Init();	
}
