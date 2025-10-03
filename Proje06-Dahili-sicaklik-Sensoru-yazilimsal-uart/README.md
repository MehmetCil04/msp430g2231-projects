# MSP430 Sıcaklık Sensörü UART İletişimi

Bu proje, MSP430 mikrodenetleyicisinin dahili sıcaklık sensöründen alınan verileri ADC üzerinden okuyup yazılımsal UART ile seri port üzerinden göndermektedir. Gerçek donanımda ölçülen sıcaklık değeri, bir terminal programı (Tera Term, PuTTY vb.) üzerinden bilgisayarda görüntülenebilir.

## Özellikler
- Dahili sıcaklık sensöründen ADC ile veri okuma
- °C cinsinden sıcaklık hesaplama
- Yazılımsal UART ile 2400 baud hızında veri gönderme
- Her saniye sıcaklık değeri ve ADC ham değeri seri porttan aktarılır

## Donanım Gereksinimleri
- MSP430 LaunchPad (ör. MSP430G2553)
- USB bağlantı kablosu
- Bilgisayarda yüklü terminal programı (Tera Term, PuTTY vb.)

## Devre Bağlantısı
Ekstra donanıma gerek yoktur. UART çıkışı, LaunchPad üzerindeki dahili USB-UART dönüştürücü üzerinden bilgisayara aktarılır.

- **TX pin:** P1.1 (kodda `#define TXD BIT1` olarak tanımlı)
- **RX pinine gerek yoktur**, çünkü yalnızca veri gönderimi yapılmaktadır.
- USB bağlantısı üzerinden sanal COM port bilgisayarda otomatik olarak oluşur.

## Terminal Ayarları
Bilgisayarda Tera Term veya benzeri bir seri port terminali açarak aşağıdaki ayarları yapın:
- **Baud Rate:** 2400
- **Data Bits:** 8
- **Parity:** None
- **Stop Bits:** 1
- **Flow Control:** None

## Çalışma Prensibi
1. ADC ile sıcaklık sensöründen ham değer okunur.
2. Ham ADC değeri yaklaşık sıcaklık formülü ile °C’ye dönüştürülür:
3. Sonuçlar UART üzerinden bilgisayara `"Sicaklik: XX C"` formatında gönderilir.
4. Ayrıca ham ADC değeri `"ADC: XXXX"` formatında görüntülenir.
5. Her saniye güncel değerler tekrar gönderilir.


## Simülasyon Notu
Proteus veya benzeri simülasyon ortamlarında MSP430’un dahili sıcaklık sensörünün sıcaklık değeri sabit kalmaktadır. Bu nedenle simülasyonda değer değişmez. Gerçek donanımda ölçülen sıcaklık, ortam sıcaklığına bağlı olarak değişecektir.

---
Bu proje MSP430 ile dahili sıcaklık sensörünü kullanarak UART üzerinden veri gönderimini öğrenmek için hazırlanmıştır.
---
<img width="1918" height="985" alt="image" src="https://github.com/user-attachments/assets/39f33b8f-5b0c-4aad-a3b0-9e0053179972" />

