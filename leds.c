#include  <msp430g2231.h>

/* Global for delay timer */
int delay_multiplyer = 5;

void main(void)
{
    unsigned int i;

    WDTCTL = WDTPW + WDTHOLD;  // Stop watchdog timer
    P1DIR = 0;
    P1DIR = BIT0|BIT6; // Set LEDs to out
    
    /* Turn LED1 on and LED2 off */
    P1OUT |= BIT0;
    P1OUT &= ~BIT6;
    
    /* Stole this from somewhere.  Seems to be right for the onboard switch */
    /* Finally looked up PnIES.  sets leading or trailing edge IRQ.  Here
     * we've set trailing edge (high to low transition)
     */
    P1IES |= BIT3;   // high -> low is selected with IES.x = 1.
    
    
    /* Clear the irq flag for P1.3, so we don't throw an immediate irq
     * Then turn on IRQs
     */
    P1IFG &= ~BIT3;
    P1IE |= BIT3;
    
    _enable_interrupt(); // umm, what it says

        for (;;){
            /* modified from some example */
            i = 10000*delay_multiplyer;
            do (i--);
            while (i != 0);
            
            /* flip the LEDs */
            P1OUT ^= BIT0;
            P1OUT ^= BIT6;
    }
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


