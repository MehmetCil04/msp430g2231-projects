# MSP430 PS/2 Klavye Okuma ve Yazılımsal UART ile Gösterim

Bu proje, MSP430 mikrodenetleyicisine bağlanan bir **PS/2 klavyeden** gelen verileri okuyarak, yazılımsal UART üzerinden bilgisayara göndermektedir. Terminal programı (Tera Term, PuTTY vb.) üzerinden bastığınız tuşlar **ASCII karakterleri** olarak görüntülenebilir.  

---

## Özellikler
- PS/2 klavyeden **Set-2 scancode** okuma  
- Make/Break (basma/bırakma) kodlarını ayırt etme  
- Shift tuşu desteği (büyük harf için)  
- Yazılımsal UART üzerinden ASCII karakter gönderme  
- Terminal ekranında bastığınız tuşların doğrudan görünmesi  

---

## Donanım Gereksinimleri
- MSP430 LaunchPad (ör. MSP430G2231 / MSP430G2553)  
- PS/2 klavye ve PS/2 Mini-DIN-6 konnektör  
- Breadboard ve bağlantı kabloları  
- 5V besleme (örn. pil kutusu veya USB)  
- Bilgisayarda terminal programı (Tera Term, PuTTY vb.)  

---

## PS/2 Konnektör Pinleri (Mini-DIN-6, **erkek önden görünüm**)
- **Pin 1 → DATA**  → MSP430 P1.5  
- **Pin 2 → n/c**   → kullanılmaz  
- **Pin 3 → GND**   → MSP430 GND ile ortak  
- **Pin 4 → +5V**   → Klavye beslemesi  
- **Pin 5 → CLK**   → MSP430 P1.4  
- **Pin 6 → n/c**   → kullanılmaz  

⚠️ Renk kodları üreticiden üreticiye değişebilir. Doğru bağlantı için **pin numarasına** güvenilmelidir.  

---

## UART Çıkışı
- TX = P1.1 (kodda `#define TXD BIT1`)  
- RX kullanılmaz (sadece gönderme yapılır).  
- USB bağlantısı üzerinden bilgisayara sanal COM port oluşur.  

---

## Terminal Ayarları
- **Baud Rate**: 1200  
- **Data Bits**: 8  
- **Parity**: None  
- **Stop Bits**: 1  
- **Flow Control**: None  

---

## Çalışma Prensibi
1. Klavyeden gelen CLK düşen kenarında DATA pini okunur.  
2. 11 bitlik PS/2 paketi (start, 8 veri, parity, stop) yakalanır.  
3. Veri kısmı çözümlenerek **scancode** elde edilir.  
4. Scancode → ASCII tablosu ile çevrilir.  
5. Karakter terminale gönderilir.  

---

## Simülasyon Notu
- Proteus’ta hazır PS/2 klavye modeli bulunmadığından Fritzing ile bağlantı gösterilmiştir.  
- Gerçek donanımda PS/2 klavye ile çalışır.  
- Terminal ekranında bastığınız karakterler doğrudan görünür:   

---

📌 Bu proje, MSP430 ile **PS/2 protokolünü** anlamak, bit seviyesinde veri yakalamayı öğrenmek ve yazılımsal UART kullanarak bilgisayara veri göndermeyi göstermek amacıyla hazırlanmıştır.  
