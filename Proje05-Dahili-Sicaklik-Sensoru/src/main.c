#include <msp430.h>

// LED tanımları
#define LED0 BIT0  // Çok soğuk
#define LED1 BIT1  // Soğuk
#define LED2 BIT2  // Ilık
#define LED3 BIT3  // Sıcak
#define LED4 BIT4  // Çok sıcak

void adc_init(void) {
    ADC10CTL1 = INCH_10;  // Kanal 10: sıcaklık sensörü
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON;
    __delay_cycles(1000);  // Referans sabitlenme süresi
}

unsigned int read_temp_raw(void) {
    ADC10CTL0 |= ENC + ADC10SC;
    while (ADC10CTL1 & ADC10BUSY);
    return ADC10MEM;
}

void clear_leds(void) {
    P1OUT &= ~(LED0 | LED1 | LED2 | LED3 | LED4);
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;

    // LED pinlerini çıkış olarak ayarla
    P1DIR |= (LED0 | LED1 | LED2 | LED3 | LED4);
    clear_leds();

    adc_init();

    while (1) {
        unsigned int adc = read_temp_raw();

        // Sıcaklık derecesini hesapla (tam sayı)
        int tempC = ((int)(adc - 673) * 423) / 1024;

        clear_leds();

        // Dereceye göre LED yak
        if (tempC < 30)
            P1OUT |= LED0;   // Çok soğuk
        else if (tempC < 35)
            P1OUT |= LED1;   // Soğuk
        else if (tempC < 40)
            P1OUT |= LED2;   // Ilık
        else if (tempC < 45)
            P1OUT |= LED3;   // Sıcak
        else
            P1OUT |= LED4;   // Çok sıcak

        __delay_cycles(500000);  // ~0.5 saniye bekle
    }
}
