# MSP430 PS/2 Klavye Okuma ve OLED Ekranda Gösterim

Bu proje, MSP430 mikrodenetleyicisine bağlanan bir **PS/2 klavyeden** gelen verileri okuyarak, **SSD1306 OLED ekran** üzerinde görüntüler. Terminal veya UART kullanılmaz, bastığınız tuşlar doğrudan OLED’de görünür.  

---

## Özellikler
- PS/2 klavyeden **Set-2 scancode** okuma  
- Make/Break (basma/bırakma) kodlarını ayırt etme  
- Shift tuşu desteği (büyük harf için)  
- Boşluk, Enter ve Backspace gibi temel kontrol tuşlarını destekleme  
- I²C üzerinden SSD1306 OLED’e yazdırma  
- 5x7 piksel font kullanılarak ASCII karakterlerin ekrana basılması  

---

## Donanım Gereksinimleri
- MSP430 LaunchPad (ör. MSP430G2231 / MSP430G2553)  
- PS/2 klavye ve PS/2 Mini-DIN-6 konnektör  
- SSD1306 OLED ekran (I²C arayüzlü)  
- Breadboard ve bağlantı kabloları  
- 3.3V besleme (OLED ve MSP430 uyumluluğu için)  

---

## PS/2 Konnektör Pinleri (Mini-DIN-6, **erkek önden görünüm**)
- **Pin 1 → DATA**  → MSP430 P1.5  
- **Pin 2 → n/c**   → kullanılmaz  
- **Pin 3 → GND**   → MSP430 GND ile ortak  
- **Pin 4 → +5V**   → Klavye beslemesi  
- **Pin 5 → CLK**   → MSP430 P1.4  
- **Pin 6 → n/c**   → kullanılmaz  

⚠️ Renk kodları değişebilir. Doğru bağlantı için **pin numaraları** esas alınmalıdır.  

---

## OLED Bağlantıları (SSD1306 I²C)
- **SDA** → MSP430 P1.7  
- **SCL** → MSP430 P1.6  
- **VCC** → 3.3V  
- **GND** → GND  
- **Not:** SCL ve SDA hatlarında 4.7k–10k pull-up direnç olmalıdır.  

---

## Çalışma Prensibi
1. Klavyeden gelen **CLK** düşen kenarında **DATA** pini okunur.  
2. 11 bitlik PS/2 paketi (start, 8 veri, parity, stop) toplanır.  
3. Veri kısmı çözümlenerek **scancode** elde edilir.  
4. Scancode → ASCII dönüşümü yapılır.  
5. ASCII karakter, I²C üzerinden SSD1306 OLED’e gönderilerek ekrana yazdırılır.  
6. Satır sonu geldiğinde otomatik olarak alt satıra geçilir.  

---

## Örnek Kullanım
- Klavyeden **`a`** tuşuna basıldığında OLED ekranında `a` görünür.  
- **Space** → boşluk ekler.  
- **Enter** → yeni satıra geçer.  
- **Backspace** → son karakteri siler.    

---

## Simülasyon Notu
- Proteus’ta hazır PS/2 klavye modeli bulunmadığı için doğrudan gerçek donanım üzerinde test edilmiştir.  
- OLED ekranda karakterler birebir görüntülenir, UART veya terminal gerekmez.  

---

📌 Bu proje, MSP430 ile **PS/2 protokolünü** ve **I²C üzerinden OLED kontrolünü** öğrenmek amacıyla hazırlanmıştır.  
