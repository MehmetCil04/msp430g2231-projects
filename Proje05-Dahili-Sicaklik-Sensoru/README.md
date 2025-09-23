# MSP430G2231 Dahili Sıcaklık Sensörü ile LED Göstergesi

## Proje Hakkında
Bu projede **MSP430G2231 mikrodenetleyicisinin dahili sıcaklık sensörü** kullanılarak sıcaklık değeri okunur ve belirli aralıklara göre **farklı LED’ler yakılır**.  

Amaç, sıcaklık bilgisini kolay bir şekilde **renkli LED göstergesi** ile ifade etmektir.  

⚠️ **Not:** Proteus simülasyon ortamında MSP430’un sıcaklığı **sabit kalır**. Yani simülasyonda LED’ler tek bir değere göre yanar, sıcaklığı değiştirmek mümkün değildir. Bu devre gerçek donanımda test edildiğinde farklı sıcaklıklara göre LED’ler yanacaktır.  

## Çalışma Prensibi
- MSP430’un dahili **ADC10** modülü ile **kanal 10 (temperatür sensörü)** okunur.
- ADC değeri aşağıdaki yaklaşık formülle sıcaklığa çevrilir:
  \[
  Temp (°C) = \frac{(ADC - 673) \times 423}{1024}
  \]
- Hesaplanan sıcaklığa göre LED’ler yakılır:
  - `< 30°C` → **Mavi LED (Çok soğuk)**
  - `30–35°C` → **Yeşil LED (Soğuk)**
  - `35–40°C` → **Sarı LED (Ilık)**
  - `40–45°C` → **Turuncu LED (Sıcak)**
  - `> 45°C` → **Kırmızı LED (Çok sıcak)**

## Devre Bağlantıları
- **LED0 (Mavi)** → P1.0  
- **LED1 (Yeşil)** → P1.1  
- **LED2 (Sarı)** → P1.2  
- **LED3 (Turuncu)** → P1.3  
- **LED4 (Kırmızı)** → P1.4  
- Her LED için **100Ω direnç** seri bağlıdır.  

## Proteus Simülasyonu
- Kullanılan elemanlar:  
  - MSP430G2231  
  - 5 adet LED (Mavi, Yeşil, Sarı, Turuncu, Kırmızı)  
  - 5 adet 100Ω direnç  

> ⚠️ **Önemli:** Proteus MSP430’un dahili sıcaklık sensörünü dinamik olarak değiştirmeye izin vermez. Bu nedenle simülasyonda tek bir LED yanar. Gerçek devrede sıcaklık değiştikçe LED’ler de değişecektir.  

## Gereksinimler
- Code Composer Studio veya IAR Embedded Workbench  
- MSP430G2231 LaunchPad  
- LED ve dirençler  

## Dosyalar
- **main.c** → MSP430 için kaynak kod  
- **Dahili-Sicaklik-Sensoru.pdsprj** → Proteus devre dosyası (örnek isim)  
- **README.md** → Bu açıklama dosyası  

## Lisans
Bu proje eğitim amaçlıdır. İsteyen herkes geliştirebilir ve kullanabilir.
