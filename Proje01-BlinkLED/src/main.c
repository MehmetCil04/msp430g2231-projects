#include <msp430.h>

int main(void){
    WDTCTL = WDTPW | WDTHOLD;

    P1SEL  &= ~BIT0;   // P1.0 = GPIO
    P1REN  &= ~BIT0;   // iç pull-up/down kapalı
    P1OUT  &= ~BIT0;   // başlangıçta 0
    P1DIR  |=  BIT0;   // çıkış

    for(;;){
      P1OUT &= ~BIT0;       
        __delay_cycles(1000000);
        P1OUT |=  BIT0;  
        __delay_cycles(1000000);
    }
}