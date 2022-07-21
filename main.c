// 1. Pre-processor directives
#define GPIO_PORTF_DATA_R 		(*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R 		  (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R 		(*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R 		  (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R 		  (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R 		(*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R 		  (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R 		(*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R 		(*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R 		    (*((volatile unsigned long *)0x400FE108))
// 2. Declarations section


unsigned long In; // Input from PF4
unsigned long Out; // Output to PF2 (blue LED)
// 3. Subroutines

void Delay(void){
	unsigned long j;
	for(j=0;j<500000;j++); //Delay for given amount of cycles
	
}

void PortF_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020; // Port F clock
	delay = SYSCTL_RCGC2_R; // delay
	GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock PortF PF0
	GPIO_PORTF_CR_R = 0x1F; // Allow changes to PF4-0
	GPIO_PORTF_AMSEL_R = 0x00; // Disable analog function
	GPIO_PORTF_PCTL_R = 0x00000000; // GPIO clear bit PCTL
	GPIO_PORTF_DIR_R = 0x0E; // PF4, PF0 input, PF3, PF2, PF1 output
	GPIO_PORTF_AFSEL_R = 0x00; // No alternate function
	GPIO_PORTF_PUR_R = 0x11; // Enable pullup resistors on PF4, PF0
	GPIO_PORTF_DEN_R = 0x1F; // Enable digital pins PF4-Pf0
}
int main(void){
	PortF_Init(); // Call initialization of PortF
	while(1){
		In = GPIO_PORTF_DATA_R&0x10; // Read PF4 into In
		In = In>>2; // Shift into position of PF2
		GPIO_PORTF_DATA_R = In; //Write output to PF2
		Delay();
		GPIO_PORTF_DATA_R = In>>2; //Shift 1 bit to PF0 (the bottom right button)
		Delay();
	}
	
}
