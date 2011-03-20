#include  <msp430g2231.h>

/* Global for delay timer */
int delay_multiplyer = 5;

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
	unsigned int i;
	int mode = 1;
	
	_init_board(1);

//	WDTCTL = WDTPW + WDTHOLD;  // Stop watchdog timer
// 	P1DIR = 0;
//	P1DIR = BIT0|BIT6|BIT4|BIT5|BIT7; // Set LEDs to out
	
//	/* Turn LED1 on and LED2 off */
//	P1OUT |= BIT0;
//	P1OUT |= BIT6;
//	P1OUT |= BIT4;
//	P1OUT |= BIT5;
//	P1OUT |= BIT7;
	
	/* Stole this from somewhere.  Seems to be right for the onboard switch */
	P1IES |= BIT3;   // high -> low is selected with IES.x = 1.
	
	
	/* Clear the irq flag for P1.3, so we don't throw an immediate irq
	 * Then turn on IRQs
	 */
	P1IFG &= ~BIT3;
	P1IE |= BIT3;
	
	_enable_interrupt(); // umm, what it says
//#define foo 1
#ifndef foo

		for (;;){
			/* modified from some example */
			//i = 10000*delay_multiplyer;
			//i = 5000*delay_multiplyer;
			i = 2000*delay_multiplyer;
			do (i--);
			while (i != 0);
			
			/* flip the LEDs */
			//P1OUT ^= BIT0;
			//P1OUT ^= BIT6;
			if (mode%5 == 0){
			//	/* 0 on 6/4 off */
					P1OUT &= ~BIT0;
					P1OUT |= BIT4;
					P1OUT |= BIT5;
					P1OUT |= BIT6;
					P1OUT |= BIT7;
			}
			else if (mode%5 == 1) {
				/* 6 on 0/4 off */
					P1OUT |= BIT0;
					P1OUT &= ~BIT4;
					P1OUT |= BIT5;
					P1OUT |= BIT6;
					P1OUT |= BIT7;
			}
			else if (mode%5 ==2){
				/* 4 on 0/6 0ff */
					P1OUT |= BIT0;
					P1OUT |= BIT4;
					P1OUT &= ~BIT5;
					P1OUT |= BIT6;
					P1OUT |= BIT7;
			}
			else if (mode%5 == 3) {
					P1OUT |= BIT0;
					P1OUT |= BIT4;
					P1OUT |= BIT5;
					P1OUT &= ~BIT6;
					P1OUT |= BIT7;
			}
			else if (mode%5 == 4) {
					P1OUT |= BIT0;
					P1OUT |= BIT4;
					P1OUT |= BIT5;
					P1OUT |= BIT6;
					P1OUT &= ~BIT7;
			}
			mode++;
			if (mode > 5) {
				mode = 1;
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
			if (delay_multiplyer == 0){
				delay_multiplyer = 5;
			}
			else {
				delay_multiplyer--;
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

