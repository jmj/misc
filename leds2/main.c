
#ifdef __GNUC__
#include <io.h>
#include <signal.h>
#else
#include  <msp430g2231.h>
#endif

/* Global for delay timer */
volatile int delay_multiplyer = 5;
volatile int dir = 1;

/*
 * basic init function.  Use this for almost every project
 */
void init_board(int button_enable) {
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

int main(void)
{
	unsigned long i;
	int mode = 0;
	int dir2 = 1;

	
	init_board(1);
	
	P1IES |= BIT3;   // high -> low is selected with IES.x = 1.

	/*
     * Clear the irq flag for P1.3, so we don't throw an immediate irq
	 * Then turn on IRQs for P1.3
	 */
	P1IFG &= ~BIT3;
	P1IE |= BIT3;
	
#ifdef __GNUC__
    WRITE_SR(GIE);
#else
	_enable_interrupt(); // umm, what it says
#endif

    BCSCTL3 |= LFXT1S_2;
    TACTL = TASSEL_1 | MC_1;
    TACCTL0 = CCIE;
    TACCR0 = 2400 * delay_multiplyer;

    while(1){
        //nothing
    }
}

#ifdef __GNUC__
interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void) {
#else
#pragma vector = TIMERA0_VECTOR
__interrupt void TIMERA0_ISR(void) {
#endif
    P1OUT ^= BIT0;
}

#ifdef __GNUC__
interrupt(PORT1_VECTOR) P1_ISR(void) {
#else
#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void) {
#endif
    switch(P1IFG&BIT3) {
        case BIT3:
            P1IFG &= ~BIT3; // clear
            TACCR0 = 2400 * delay_multiplyer--;
            return;
        default:
            P1IFG = 0;
            return;
    }
}
            
