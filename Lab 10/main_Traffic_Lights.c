/* See Readme.txt on github for project detail */

/* Pre-processor directives*/
#include "TM4C123G_AKM.h" /* Custom header with Port B, E, and F register pointers */
#include <stdint.h>

#define SENSOR                  (*((volatile uint32_t *)0x4002401C)) /* Inputs from PE0, PE1, and PE2 */
#define LIGHT                   (*((volatile uint32_t *)0x400050FC)) /* Port B data register for PB5-PB0 */
#define LIGHTBOARD              (*((volatile uint32_t *)0x400253FC)) /* Green & red LEDs on board (PF1 & PF3) */
#define SYSCTL_RCC_R            (*((volatile uint32_t *)0x400FE060)) /* PLL */
#define SYSCTL_RCC2_R           (*((volatile uint32_t *)0x400FE070)) /* PLL */	
#define SYSCTL_RIS_R            (*((volatile uint32_t *)0x400FE050)) /* PLL */
#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010)) /* Systick */
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014)) /* Systick */
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018)) /* Systick */
#define goW                     0                                    /* Allow cars to move from west direction- west light is green */
#define waitW                   1                                    /* Slow cars from west direction- west light is yellow then red */
#define goS                     2                                    /* Allow cars to move from south direction- south light is green */
#define waitS                   3                                    /* Slow cars from south direction- south light is yellow then red */
#define goPed                   4                                    /* Allow pedestrians to cross street- west & south lights are red */
#define hurryPed                5                                    /* Let pedestrians know to hurry across the street, initial light-up (onboard LED is red) */
#define Pause1                  6                                    /* Let pedestrians know to hurry across the street, initial blink (onboard LED is off) */
#define hurryPed2               7                                    /* Let pedestrians know to hurry across the street, LED light-up (onboard LED is red) */
#define Pause2                  8                                    /* Let pedestrians know to hurry across the street, second blink (onboard LED is off) */
#define hurryPed4               9                                    /* Let pedestrians know to hurry across the street, LED light-up (onboard LED is red) */

/* Linked data structure */
struct State {
	uint32_t Out; /* Output for traffic lights */
	uint32_t OutBoard; /* Output for crosswalk */
	uint32_t Time; /* Delay before moving to next state */
	uint32_t Next[8];}; /* Next state, based on input */

typedef const struct State State_t;
	
State_t FSM[10]={
{0x21,0x02,50,{goW,waitW,goW,waitW,waitW,waitW,waitW,waitW}},
{0x22,0x02,50,{goS,goS,goS,goS,goPed,goS,goPed,goS}},
{0x0C,0x02,50,{goS,goS,waitS,waitS,waitS,waitS,waitS,waitS}},
{0x14,0x02,50,{goPed,goPed,goW,goW,goPed,goPed,goPed,goPed}},
{0x24,0x08,50,{goPed,hurryPed,hurryPed,hurryPed,goPed,hurryPed,hurryPed,hurryPed}},
{0x24,0x02,50,{Pause1,Pause1,Pause1,Pause1,Pause1,Pause1,Pause1,Pause1}},
{0x24,0x00,50,{hurryPed2,hurryPed2,hurryPed2,hurryPed2,hurryPed2,hurryPed2,hurryPed2,hurryPed2}},
{0x24,0x02,50,{Pause2,Pause2,Pause2,Pause2,Pause2,Pause2,Pause2,Pause2}},
{0x24,0x00,50,{hurryPed4,hurryPed4,hurryPed4,hurryPed4,hurryPed4,hurryPed4,hurryPed4,hurryPed4}},
{0x24,0x02,50,{goW,goS,goW,goW,goW,goS,goW,goW}}};

 uint32_t S; /* Index to the current state */
 uint32_t Input; /* Used to read button inputs */

/* Function prototypes */
void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait(uint32_t delay);
void SysTick_Wait10ms(uint32_t delay);

void PLL_Init(void){
  SYSCTL_RCC2_R |=  0x80000000;  /* USERCC2 */
  SYSCTL_RCC2_R |=  0x00000800;  /* BYPASS2, PLL bypass */
  SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)   /* Clear XTAL field, bits 10-6 */
                 + 0x00000540;   /* 10101, configure for 16 MHz crystal */
  SYSCTL_RCC2_R &= ~0x00000070;  /* configure for main oscillator source */
  SYSCTL_RCC2_R &= ~0x00002000;
  SYSCTL_RCC2_R |= 0x40000000;   /* use 400 MHz PLL */
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  /* clear system clock divider */
                  + (4<<22);      /* configure for 80 MHz clock */
  while((SYSCTL_RIS_R&0x00000040)==0){}  /* wait for PLLRIS bit */
  SYSCTL_RCC2_R &= ~0x00000800;
}

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}

/* The delay parameter is in units of the 80 MHz core clock. (12.5 ns) */
void SysTick_Wait(uint32_t delay){
  NVIC_ST_RELOAD_R = delay-1;  /* number of counts to wait */
  NVIC_ST_CURRENT_R = 0;       /* any value written to CURRENT clears */
  while((NVIC_ST_CTRL_R&0x00010000)==0){ /* wait for count flag */
  }
}

/* 800000*12.5ns equals 10ms */
void SysTick_Wait10ms(uint32_t delay){
  uint32_t i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  /* wait 10ms */
  }
}

int main(void){ volatile uint32_t delay;
	PLL_Init();
	SysTick_Init();
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
			SysTick_Wait10ms(FSM[S].Time); /* Delay based on time specified in struct */
			Input = SENSOR; /* Read input */
			S = FSM[S].Next[Input]; /* Move to next state based on input (button pressed) */
     }
	}
