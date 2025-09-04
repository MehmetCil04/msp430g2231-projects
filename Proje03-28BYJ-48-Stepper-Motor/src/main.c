#include <msp430.h>

#define IN1 BIT0   // P1.0 → ULN2003 giriş 1
#define IN2 BIT1   // P1.1 → ULN2003 giriş 2
#define IN3 BIT2   // P1.2 → ULN2003 giriş 3
#define IN4 BIT3   // P1.3 → ULN2003 giriş 4

// Yarım adım sürüş dizisi (28BYJ-48 için)
static const unsigned char halfStepSeq[8] = {
   IN4 | IN1,  // 1001
   IN1,        // 0001
   IN1 | IN2,  // 0011
   IN2,        // 0010
   IN2 | IN3,  // 0110
   IN3,        // 0100
   IN3 | IN4,  // 1100
   IN4         // 1000
};

static unsigned char idx = 0;  // global faz indeksi

// ms cinsinden gecikme
void delay_ms(unsigned int ms) {
    while (ms--) {
        __delay_cycles(1000); // 1ms @ 1MHz
    }
}

// step sayısı ve yön parametresiyle motoru sür
void stepMotor(unsigned int steps, int dir) {
    while (steps--) {
        if (dir > 0) 
            idx = (idx + 1) & 7;   // ileri (0..7)
        else 
            idx = (idx + 7) & 7;   // geri

        P1OUT = (P1OUT & 0xF0) | halfStepSeq[idx]; // sadece P1.0–P1.3 değişsin
        delay_ms(10); // adımlar arası gecikme
    }
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Watchdog kapat
    BCSCTL1 = CALBC1_1MHZ;      // DCO → 1MHz
    DCOCTL  = CALDCO_1MHZ;

    P1DIR |= IN1 | IN2 | IN3 | IN4;   // P1.0–P1.3 çıkış
    P1OUT &= ~(IN1 | IN2 | IN3 | IN4); // başlangıçta LOW

    for (;;) {
        stepMotor(64, +1);  // ileri yönde 64 adım
        delay_ms(300);
        stepMotor(64, -1);  // geri yönde 64 adım
        delay_ms(300);
    }
}
