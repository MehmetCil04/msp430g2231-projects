# Proje 01 – Blink LED (MSP430G2231)

Bu projede **MSP430G2231** üzerinde **P1.0** pinine bağlı LED yaklaşık **1 saniye** aralıklarla yanıp sönmektedir.  
Kod `src/main.c` içindedir, Proteus dosyaları `proteus/` klasöründedir.

---

## İçerik
- `src/main.c` — C kaynak kodu
- `proteus/BlinkLED.pdsprj` — Proteus proje dosyası
- `proteus/screenshot.png` — simülasyon ekran görüntüsü

---

## Donanım
- MSP430G2231
- LED (P1.0’a bağlı)
- 330Ω direnç

---

## Kod Açıklaması
- `WDTCTL = WDTPW | WDTHOLD;` → Watchdog Timer kapatılır.  
- `P1SEL &= ~BIT0;` → P1.0 GPIO olarak ayarlanır.  
- `P1REN &= ~BIT0;` → Dahili pull-up/down devre dışı bırakılır.  
- `P1OUT &= ~BIT0;` → Başlangıçta LED kapalıdır.  
- `P1DIR |= BIT0;` → P1.0 çıkış olarak ayarlanır.  
- Döngü içinde:
  - `P1OUT &= ~BIT0;` → LED söner.  
  - `__delay_cycles(1000000);` → ~1 saniye bekleme (1 MHz saat için).  
  - `P1OUT |= BIT0;` → LED yanar.  
  - `__delay_cycles(1000000);` → tekrar ~1 saniye bekleme.  

---

<img width="1919" height="988" alt="screenshot" src="https://github.com/user-attachments/assets/405b6be0-1918-4821-bf5b-7ed04f8673ee" />

