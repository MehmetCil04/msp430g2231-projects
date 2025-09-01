# Proje 02 – 7 Segment Display (MSP430G2231)

Bu projede **MSP430G2231** mikrodenetleyici ile **common anode 7-segment display** kullanılarak **0–9 arası rakamlar gösterilmektedir**.  
Butona (P1.7) her basıldığında sayı 1 artar. 9’dan sonra tekrar 0’a döner.

---

## İçerik
- `src/main.c` — C kaynak kodu
- `proteus/7-Segment-Display.pdsprj` — Proteus proje dosyası

---

## Donanım
- MSP430G2231
- 1× common anode 7-segment display
- 7× 330Ω direnç (segmentler için)
- 1× buton (P1.7’ye bağlı)
- Pull-up direnci (dahili, MSP430 üzerinde aktif ediliyor)

---

## Kod Açıklaması
- `digits[]` dizisi → 0–9 arasındaki rakamların segment kodlarını tutar. (Aktif düşük, çünkü **common anode** display kullanılıyor.)
- **Port ayarları:**
  - `P1DIR |= 0x7F;` → P1.0–P1.6 çıkış (segmentler).
  - `P1DIR &= ~BIT7;` → P1.7 giriş (buton).
  - `P1REN |= BIT7; P1OUT |= BIT7;` → P1.7 için dahili pull-up aktif.
- **Kesme ayarları:**
  - `P1IES |= BIT7;` → düşen kenarda kesme (butona basıldığında).
  - `P1IE  |= BIT7;` → P1.7 kesmesi aktif.
- **Main döngüsü:**
  - `P1OUT = (P1OUT & BIT7) | digits[index];`  
    Butonun durumu korunur, segmentlere aktif rakam yazılır.
- **Kesme fonksiyonu:**
  - `index = (index + 1) % 10;` → her kesmede index bir arttırılır, 0–9 arasında döner.
  - `P1IFG &= ~BIT7;` → kesme bayrağı temizlenir.

---

## Zamanlama
- CPU frekansı kalibrasyon değerleriyle **1 MHz** ayarlandı:
  ```c
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  ```
  ---
  <img width="1919" height="988" alt="image" src="https://github.com/user-attachments/assets/dfe8766e-1501-47c8-9768-43134e23b5a1" />

