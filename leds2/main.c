#include  <msp430g2231.h>

/* Global for delay timer */
volatile int delay_multiplyer = 5;
volatile int dir = 1;

/*
 * basic init function.  Use this for almost every project
 */
void _init_board(int button_enable) {
	/* Set all p1.x and p2.x to output (saves power) */
	P1DIR = 0xff; 
	P2DIR = 0xff;
	
	/* Set all pins to low */
	P1OUT = 0;
	P2OUT = 0;
	
	/* Turn off the watchdog timer */
	WDTCTL = WDTPW + WDTHOLD;
	
	/* Enable the onboard button on P1.3 */ 
	if (button_enable) {
		P1DIR &= ~BIT3;
	}
}

void main(void)
{
	unsigned long i;
	unsigned int x;
	unsigned int shifter = 0x01;
	int mode = 0;
	int dir2 = 1;
	
	_init_board(1);
	P1OUT = 0xff;

	
	P1IES |= BIT3;   // high -> low is selected with IES.x = 1.
	
	
	/* Clear the irq flag for P1.3, so we don't throw an immediate irq
	 * Then turn on IRQs for P1.3
	 */
	P1IFG &= ~BIT3;
	P1IE |= BIT3;
	
	_enable_interrupt(); // umm, what it says


//#define foo 1
#ifndef foo

		for (;;){
			//i = 50000*delay_multiplyer;
			//i = 5000*delay_multiplyer;
			i = 2000*delay_multiplyer;

			do (i--);
			while (i != 0);

			P1OUT = 0;
			if (mode%5 == 0){
				P1OUT |= BIT0;
			}
			else if (mode%5 == 1) {
				P1OUT |= BIT4;
			}
			else if (mode%5 == 2){
				P1OUT |= BIT5;
			}
			else if (mode%5 == 3) {
				P1OUT |= BIT6;
			}
			else if (mode%5 == 4) {
				P1OUT |= BIT7;
			}
			
			if (dir2){
				mode++;
				if (mode == 4) {
					dir2 = 0;
				}
			}
			else {
				mode--;
				if (mode == 0) {
					dir2 = 1;
				}
			}
				
	}
#endif
} 


/* IRQ handler on GPIO port 1 */
#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void) {
	switch(P1IFG&BIT3) {
		case BIT3:
			P1IFG &= ~BIT3; // clear
			
			if (dir){
				delay_multiplyer--;
				if (delay_multiplyer == 1){
					dir = 0;
				}
			}
			else {
				delay_multiplyer++;
				if (delay_multiplyer == 5){
					dir = 1;
				}
			}
			return;
		default:
			 /* clear all flags -->
			  * This can cause lost IRQs, but we only have one input, so 
			  * it's a non-issue here.
			  */
			P1IFG = 0;
			return;
	}
}

