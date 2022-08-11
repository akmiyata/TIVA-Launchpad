/* Port B Definitions */
#define GPIO_PORTB_DATA_R 		(*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R 		  (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R 		(*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PUR_R 		  (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_DEN_R 		  (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R 		(*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R 		  (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R 		(*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R 		(*((volatile unsigned long *)0x4000552C))
#define SYSCTL_RCGC2_R 		    (*((volatile unsigned long *)0x400FE608))

/* Port E Definitions */
#define GPIO_PORTE_DATA_R 		(*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R 		  (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R 		(*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R 		  (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R 		  (*((volatile unsigned long *)0x4002451C))
/* No lock for Port E */
#define GPIO_PORTE_CR_R 		  (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R 		(*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R 		(*((volatile unsigned long *)0x4002452C))
#define PE0                   (*((volatile uint32_t *)0x40024004)) /* Base address = 0x40024000 (port B) + 0x0004 */
#define PE1                   (*((volatile uint32_t *)0x40024008)) /* Base address = 0x40024000 (port B) + 0x0008 */

/* Port F definitions */
#define GPIO_PORTF_DATA_R 		(*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R 		  (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R 		(*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R 		  (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R 		  (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R 		(*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R 		  (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R 		(*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R 		(*((volatile unsigned long *)0x4002552C))
