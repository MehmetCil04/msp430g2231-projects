#include <msp430.h>

// Pin tanımlamaları
#define TRIG_PIN BIT0    // P1.0
#define ECHO_PIN BIT1    // P1.1
#define BUZZER_PIN BIT5  // P1.5

// Değişkenler
volatile unsigned int echo_time = 0;
volatile unsigned int distance_cm = 0;
volatile unsigned char echo_received = 0;

void clock_init(void) {
    // DCO'yu 1MHz'e ayarla
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
}

void gpio_init(void) {
    // Port yönlendirmeleri
    P1DIR |= TRIG_PIN | BUZZER_PIN;  // TRIG ve BUZZER çıkış
    P1DIR &= ~ECHO_PIN;              // ECHO giriş
    
    // Başlangıç durumları
    P1OUT &= ~(TRIG_PIN | BUZZER_PIN); // TRIG ve BUZZER LOW
    
    // ECHO pin'i için interrupt ayarları
    P1IES &= ~ECHO_PIN;  // Rising edge
    P1IFG &= ~ECHO_PIN;  // Interrupt flag temizle
    P1IE |= ECHO_PIN;    // Interrupt etkinleştir
}

void timer_init(void) {
    // Timer A0 ayarları
    TA0CTL = TASSEL_2 + MC_2 + TACLR;  // SMCLK, continuous mode, clear
    TA0CCTL0 = 0;
}

void delay_us(unsigned int us) {
    while(us--) {
        __delay_cycles(1);  // 1MHz için 1 cycle = 1us
    }
}

void delay_ms(unsigned int ms) {
    while(ms--) {
        __delay_cycles(1000);  // 1MHz için 1000 cycle = 1ms
    }
}

void trigger_pulse(void) {
    // 10us trigger pulse gönder
    P1OUT |= TRIG_PIN;
    delay_us(10);
    P1OUT &= ~TRIG_PIN;
}

unsigned int measure_distance(void) {
    echo_received = 0;
    echo_time = 0;
    
    // Trigger pulse gönder
    trigger_pulse();
    
    // Echo yanıtını bekle (timeout ile)
    unsigned int timeout = 30000;
    while(!echo_received && timeout--) {
        delay_us(1);
    }
    
    if(echo_received) {
        // Mesafeyi hesapla (cm cinsinden)
        // echo_time mikronsaniye cinsinden
        // Mesafe = (echo_time * 0.034) / 2
        // 0.034 cm/us = ses hızı
        distance_cm = (echo_time * 17) / 1000;  // Basitleştirilmiş hesaplama
        return distance_cm;
    }
    
    return 999;  // Hata durumu
}

void buzzer_tone(unsigned int frequency, unsigned int duration_ms) {
    // Frekansa göre buzzer sesi
    unsigned int half_period = 500000 / frequency;  // Yarım periyot hesabı (us)
    unsigned int cycles = (frequency * duration_ms) / 1000;
    
    unsigned int i;
    for(i = 0; i < cycles; i++) {
        P1OUT |= BUZZER_PIN;
        delay_us(half_period);
        P1OUT &= ~BUZZER_PIN;
        delay_us(half_period);
    }
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Watchdog timer'ı durdur
    
    // Sistem başlatma
    clock_init();
    gpio_init();
    timer_init();
    
    __enable_interrupt();  // Global interrupt etkinleştir
    
    while(1) {
        unsigned int distance = measure_distance();
        
        // Sadece çok yakın mesafelerde buzzer çal (15cm ve altı)
        if(distance <= 15 && distance >= 2) {  // 2-15cm arası
            
            unsigned int frequency;
            unsigned int beep_duration;
            unsigned int pause_duration;
            
            if(distance <= 3) {
                // Çok çok yakın (0-3cm): Çok yüksek frekans, sürekli çalar
                frequency = 3000;      // 3kHz
                beep_duration = 50;
                pause_duration = 20;
            }
            else if(distance <= 5) {
                // Çok yakın (3-5cm): Yüksek frekans, hızlı bip
                frequency = 2500;      // 2.5kHz
                beep_duration = 80;
                pause_duration = 50;
            }
            else if(distance <= 8) {
                // Yakın (5-8cm): Orta yüksek frekans
                frequency = 2000;      // 2kHz
                beep_duration = 100;
                pause_duration = 100;
            }
            else if(distance <= 12) {
                // Orta yakın (8-12cm): Orta frekans
                frequency = 1500;      // 1.5kHz
                beep_duration = 120;
                pause_duration = 200;
            }
            else {
                // Yaklaşıyor (12-15cm): Düşük frekans, yavaş bip
                frequency = 1000;      // 1kHz
                beep_duration = 100;
                pause_duration = 400;
            }
            
            buzzer_tone(frequency, beep_duration);
            delay_ms(pause_duration);
        }
        else {
            // 15cm'den uzakta hiç ses çıkarma
            delay_ms(300);  // Ölçüm arası bekleme
        }
    }
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    if(P1IFG & ECHO_PIN) {
        if(P1IN & ECHO_PIN) {
            // Rising edge - ölçüm başlat
            TA0R = 0;  // Timer'ı sıfırla
        }
        else {
            // Falling edge - ölçüm bitir
            echo_time = TA0R;  // Timer değerini oku
            echo_received = 1;
        }
        
        P1IES ^= ECHO_PIN;  // Edge direction'ı değiştir
        P1IFG &= ~ECHO_PIN; // Interrupt flag'ı temizle
    }
}