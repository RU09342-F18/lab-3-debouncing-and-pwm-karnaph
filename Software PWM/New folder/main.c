#include <msp430.h> 


/**
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    TA0CTL |= TBIE;             //enable TA0 interrupt
    TA0CCTL0 |= CCIE;              //enable CCR0 interrupt
    TA0CCTL0 &= ~TAIFG;            //clear flag
    TA0CCTL1 |= CCIE;              //enable CCR1 interrupt
    TA0CCTL1 &= ~TAIFG;            //clear flag

    TA0CTL = TASSEL_2 + MC_1 + ID_0;           // SMCLK, upmode, clk/1
    TA0CCR1 = 500;                               //50% duty to start
    TA0CCR0 = 1000;                             //1 kHz signal

    P1IE |=  BIT1;                            // P1.1 interrupt enabled
    P1IES |= BIT1;                            //falling edge
    P1REN |= BIT1;                            // Enable resistor on SW2 (P1.1)
    P1OUT |= BIT1;                             //Pull up resistor on P1.1
    P1IFG &= ~BIT1;                           // P1.1 Interrupt Flag cleared

    P1DIR |= BIT0;       // P1.0 pin output
    P1OUT |= BIT0;      //turn on LED
    P4DIR |= BIT7;       // P4.7 pin output
    P4OUT |= BIT7;//turn on led

    __bis_SR_register(GIE);  //not low power mode
    while(1){
    } //needed since not in LPM
    // Enter LPM0 w/ interrupt
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void) { //timer a0 interrupt service routine
    P1OUT |= BIT0; //turn on led 1.0
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void) { //a1 timer interrupt
    switch(TA0IV){
    case 2://CCR1
    P1OUT &= ~BIT0; //turn off led 1.0
    break;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) { //button interrupt
        P4OUT ^= BIT7; //toggle led 4.7
        if(TA0CCR1 >= 1000) { //if timer has passed 1 kHz
            TA0CCR1 = 0; //reset
        }
        else {
            TA0CCR1 = TA0CCR1 + 100; //increment by 100 hz
        }
    P1IFG &=~BIT1; //reset flag
}
