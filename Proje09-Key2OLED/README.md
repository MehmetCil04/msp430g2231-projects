# MSP430 PS/2 Klavye ile OLED Üzerinde Piksel Kontrolü

Bu proje, **MSP430 mikrodenetleyicisine** bağlanan bir **PS/2 klavyeden** gelen verileri okuyarak,  
**SSD1306 OLED ekran** üzerindeki tek bir pikseli **W, A, S, D tuşlarıyla yönlendirir.**  
Terminal veya UART bağlantısı kullanılmaz — klavyeden girilen yön tuşları doğrudan OLED ekrana etki eder.  

---

## 🎯 Proje Amacı
Bu çalışmanın amacı, **PS/2 protokolü** ile klavyeden veri okumayı ve  
**I²C haberleşmesi** üzerinden bir OLED ekranı kontrol etmeyi öğretmektir.  
Klavye girdileriyle OLED üzerindeki bir pikselin pozisyonu değiştirilir;  
yani gerçek zamanlı olarak **bir “hareket eden nokta” (cursor/piksel)** oluşturulur.

---

## 🧩 Özellikler
- PS/2 klavyeden **Set-2 scancode** okuma  
- **Make/Break (basma/bırakma)** kodlarını doğru ayırt etme  
- **W, A, S, D** tuşlarıyla pikseli yönlendirme  
  - W → Yukarı  
  - A → Sola  
  - S → Aşağı  
  - D → Sağa  
- Piksel ekran kenarına geldiğinde **karşı tarafa sarar (wrap-around)**  
- **I²C üzerinden SSD1306 OLED** kontrolü  
- **Gerçek zamanlı kesme tabanlı PS/2 okuma**

---

## 🧠 Çalışma Prensibi
1. Klavyeden gelen **CLK** sinyalinin düşen kenarında **DATA** pini okunur.  
2. 11 bitlik PS/2 paketi (Start + 8 veri + Parity + Stop) yakalanır.  
3. Gelen veri **scancode** olarak çözülür.  
4. `0x1D`, `0x1B`, `0x1C`, `0x23` (W, S, A, D) kodları algılanır.  
5. Pikselin x-y koordinatları bu tuşlara göre değiştirilir.  
6. Önce eski konum sıfırlanır, ardından yeni konuma bir bit (1 piksel) çizilir.  
7. Piksel kenara ulaştığında otomatik olarak ekranın karşı ucuna geçer.  

Sonuç:  
> Klavyedeki W, A, S, D tuşlarıyla OLED ekran üzerinde hareket eden bir **tek piksel (cursor)** oluşur.

---

## 🖲️ Donanım Gereksinimleri
| Bileşen | Açıklama |
|----------|-----------|
| MSP430 LaunchPad | (ör. MSP430G2231 / MSP430G2553) |
| PS/2 klavye | Mini-DIN-6 konnektörlü |
| SSD1306 OLED ekran | I²C arayüzlü (128×64) |
| Breadboard ve jumper kablolar | Bağlantılar için |
| 3.3 V güç kaynağı | MSP430 ve OLED uyumluluğu için |

---

## 🔌 PS/2 Konnektör Pinleri (Mini-DIN-6, **erkek önden görünüm**)

| Pin | Sinyal | MSP430 Pin | Açıklama |
|------|---------|-------------|-----------|
| 1 | DATA | **P1.5** | Veri hattı |
| 2 | n/c | — | Kullanılmaz |
| 3 | GND | GND | Ortak toprak |
| 4 | +5 V | — | Klavye beslemesi |
| 5 | CLK | **P1.4** | Saat hattı |
| 6 | n/c | — | Kullanılmaz |

> ⚠️ Renk kodları farklı olabilir, **pin numaraları** esas alınmalıdır.

---

## 🖥️ OLED Bağlantıları (SSD1306 I²C)

| OLED Pin | MSP430 Pin | Not |
|-----------|-------------|------|
| SDA | **P1.7** | I²C veri hattı |
| SCL | **P1.6** | I²C saat hattı |
| VCC | 3.3 V | Besleme |
| GND | GND | Ortak toprak |

> ℹ️ SCL ve SDA hatlarında **4.7 kΩ – 10 kΩ** pull-up direnç bulunmalıdır.

---

## 🔧 Yazılım Bileşenleri
- `usi_i2c.h / usi_i2c.c` → MSP430 USI modülü ile I²C iletişimi  
- `main.c` → PS/2 klavye ISR’ı ve OLED kontrolü  
- Kesme tabanlı veri okuma (PORT1 ISR)  
- Basit piksel çizme fonksiyonu (`draw_dot(x, y)`)  

---

## 🧪 Simülasyon Notu
- Proteus’ta **hazır PS/2 klavye modeli** bulunmadığı için proje **gerçek donanım** üzerinde test edilmiştir.  
- OLED ekran doğrudan MSP430’un I²C pinlerinden sürülür, UART veya terminal gerekmez.

---

## 📚 Öğrenim Hedefleri
Bu proje, MSP430 kullanarak aşağıdaki konularda uygulama deneyimi kazandırır:
- **PS/2 protokolü** (bit-banging, kesme tabanlı veri okuma)
- **I²C haberleşmesi** (USI modülü kullanımı)
- **OLED kontrolü** (SSD1306 komut seti)
- **Gerçek zamanlı giriş/çıkış işlemleri**

