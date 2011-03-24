#include  <msp430g2231.h>

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

void main(void){
	_init_board(0);	
	//P1OUT = 0xff;
	
	/* turn on LED1 */
	P1DIR |= BIT2;
	P1SEL |= BIT2;             // P1.2 to TA0.1
	//P1OUT &= ~BIT0; //using external LED config

	
	/* use the extenal occilator */
	BCSCTL3 |= LFXT1S_0;
	
	P1OUT ^= BIT6;
	/* reset occilator fault until the external crystal is up and running */
	while(IFG1 & OFIFG) {
		IFG1 &= ~OFIFG;
		_delay_cycles(100000); // only need 50 uS, but....
	}
	
	/* Turn off LED1 - crystal startup complete */
	P1OUT ^= BIT6;
	
	/* Stop the DCO */
	//_bis_SR_register(SCG1+SCG0);
	//BCSCTL2 |= SELM_3 + DIVM_3; //MCLK divider (by 3)

	
	//CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = CCR1 = 792;								// one second - period
	//CCR1 = 32768;								// 1/2 sec - duty cycle
	TACTL = TASSEL_1 + MC_1;                  // ACLK, up mode
	CCTL1 = OUTMOD_4;				//

	P1IFG &= ~BIT5;
	P1IE |= BIT5;
	
	_enable_interrupt(); // umm, what it says	
	while(1){
		P1OUT ^= BIT6;
	}
	
//	while (1) {
//		P1OUT ^= BIT6;
//		_delay_cycles(100);
//		P1OUT ^= BIT6;
//		_delay_cycles(5000);
//	}
	
	
}

//#pragma vector=TIMERA0_VECTOR
//__interrupt void Timer_A (void)

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void) {
	P1IFG &= ~BIT5; // clear
	P1OUT ^= BIT0;
	
}
