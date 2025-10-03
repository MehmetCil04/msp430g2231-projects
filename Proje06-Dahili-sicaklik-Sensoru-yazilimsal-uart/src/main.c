#include <msp430.h>

#define TXD BIT1
#define Bit_time 410
#define ADC_CHANNEL INCH_10  // Dahili sıcaklık sensörü (internal temp sensor)

void UART_send_char(char c) {
    int i;
    P1DIR |= TXD;
    P1OUT |= TXD;

    P1OUT &= ~TXD;
    __delay_cycles(Bit_time);

    for (i = 0; i < 8; i++) {
        if (c & 0x01)
            P1OUT |= TXD;
        else
            P1OUT &= ~TXD;
        __delay_cycles(Bit_time);
        c >>= 1;
    }

    P1OUT |= TXD;
    __delay_cycles(Bit_time);
}

void UART_send_string(char *str) {
    while (*str) {
        UART_send_char(*str++);
    }
}

void UART_send_uint(unsigned int value) {
    char buffer[6];
    int i = 0;

    if (value == 0) {
        UART_send_char('0');
        return;
    }

    while (value > 0) {
        buffer[i++] = (value % 10) + '0';
        value /= 10;
    }

    while (i > 0) {
        UART_send_char(buffer[--i]);
    }
}

void ADC_init() {
    ADC10CTL1 = ADC_CHANNEL;
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON;  // Vref=1.5V
    __delay_cycles(1000);  // Referans voltajın oturması için bekle
}

unsigned int read_ADC() {
    ADC10CTL0 |= ENC + ADC10SC;
    while (ADC10CTL1 & ADC10BUSY);
    return ADC10MEM;
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= TXD;
    P1OUT |= TXD;

    ADC_init();

    while (1) {
        unsigned int adc = read_ADC();

        // Sıcaklığı yaklaşık °C'ye çevir
        // T = ((adc - 673) * 55) / 239 + 30
        int temp_c = ((long)(adc - 673) * 55) / 239 + 30;

        UART_send_string("Sicaklik: ");
        UART_send_uint(temp_c);
        UART_send_string(" C\r\n");
        UART_send_string("ADC: ");
        UART_send_uint(adc);
        UART_send_string(" \r\n");

        __delay_cycles(1000000);  // 1 saniye bekle
    }
}