#include <msp430.h>

// Common anode için aktif düşük (LOW) segment kodları
//         g f e d c b a
const unsigned char digits[10] = {
    0b11000000, // 0 → a,b,c,d,e,f
    0b11111001, // 1 → b,c
    0b10100100, // 2 → a,b,d,e,g
    0b10110000, // 3 → a,b,c,d,g
    0b10011001, // 4 → b,c,f,g
    0b10010010, // 5 → a,c,d,f,g
    0b10000010, // 6 → a,c,d,e,f,g
    0b11111000, // 7 → a,b,c
    0b10000000, // 8 → tüm segmentler
    0b10010000  // 9 → a,b,c,d,f,g
};


volatile unsigned char index = 0;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Watchdog Timer kapalı

    BCSCTL1 = CALBC1_1MHZ;   // Kalibrasyon değeri: DCO = 1MHz
    DCOCTL  = CALDCO_1MHZ;   // Kalibrasyon değeri: DCO = 1MHz

    // --- 7 Segment için P1.0–P1.6 çıkış ---
    P1DIR |= 0x7F;           // P1.0–P1.6 çıkış
    P1OUT |= 0x7F;           // Hepsi sönük başlasın (common anode)

    // --- Buton (P1.7) giriş + pull-up ---
    P1DIR &= ~BIT7;          // P1.7 giriş
    P1REN |= BIT7;           // Direnç aktif
    P1OUT |= BIT7;           // Pull-up (buton GND'ye çekiyor)

    // --- Kesme ayarı ---
    P1IES |= BIT7;           // Düşen kenarda kesme (HIGH → LOW)
    P1IFG &= ~BIT7;          // Bayrak temizle
    P1IE  |= BIT7;           // P1.7 kesmesi aktif

    __bis_SR_register(GIE);  // Global kesmeleri aç

    while (1)
    {
        P1OUT = (P1OUT & BIT7) | digits[index];  // P1.0–P1.6'ya rakam yaz, P1.7 değişmesin
        _no_operation();
    }
}

// --- Port 1 Kesme Vektörü ---
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    index = (index + 1) % 10;     // 0–9 arası arttır
    P1IFG &= ~BIT7;              // Bayrağı temizle
}
