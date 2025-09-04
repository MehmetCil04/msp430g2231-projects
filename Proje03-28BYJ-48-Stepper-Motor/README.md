# MSP430G2231 ile 28BYJ-48 Step Motor Kontrolü

Bu proje, **MSP430G2231** mikrodenetleyicisi kullanılarak **ULN2003A** sürücü entegresi üzerinden **28BYJ-48 step motorunun** yarım adım (half-step) modunda sürülmesini göstermektedir. Devre **Proteus** üzerinde simüle edilmiştir.

---

## Donanım Bağlantıları

- **MSP430G2231 → ULN2003A → 28BYJ-48 Step Motor**

| MSP430 Pin | ULN2003A Girişi | ULN2003A Çıkışı | Motor Bobini |
|------------|-----------------|-----------------|--------------|
| P1.0       | IN1             | OUT1            | IN1 (Mavi)   |
| P1.1       | IN2             | OUT2            | IN2 (Pembe)  |
| P1.2       | IN3             | OUT3            | IN3 (Sarı)   |
| P1.3       | IN4             | OUT4            | IN4 (Turuncu)|
| VCC (5V)   | COM             | Ortak uç        | Kırmızı      |
| GND        | GND             | GND             | Güç GND      |

> **Not:** MSP430’un çıkış akımı yetersiz olduğundan mutlaka **ULN2003A** gibi bir darlington sürücü entegresi kullanılmalıdır.

---

## Devre Şeması (Proteus)

<img width="1919" height="1018" alt="image" src="https://github.com/user-attachments/assets/8a77901f-b789-45e6-bc8e-16a923ebecf6" />


---

## Çalışma Prensibi

- **Gerçek 28BYJ-48 motoru**:  
  - Temel adım açısı: **5.625°/64 ≈ 0.0879°**  
  - Tam tur için ≈ **4096 yarım adım** gerekir.  

- **Proteus simülasyon modeli**:  
  - Proteus’ta kullanılan motor modeli “basitleştirilmiş iç step motor” olarak davranır.  
  - Bu yüzden simülasyonda görülen adım açısı ve tur başına adım sayısı **gerçek motor değerleriyle birebir uyuşmaz**.  
  - Kod mantığı doğru çalışsa da motorun dönüş açısı simülasyonda farklı görünebilir.  

- Kodda motor **64 adım ileri → kısa bekleme → 64 adım geri** şeklinde sonsuz döngüyle sürülür.  
- `delay_ms(10)` değeri değiştirilerek hız ayarlanabilir.  

---

## Gereksinimler

- Code Composer Studio 
- Proteus 9 (simülasyon için)  
- MSP430G2231 MCU  
- ULN2003A Darlington Sürücü  
- 28BYJ-48 Step Motor  
- 5V Güç Kaynağı  

---

## Notlar

- Bu proje **eğitim amaçlıdır**. Gerçek donanımda adım açısı ve hız değerleri Proteus simülasyonundan farklı olacaktır.  
- Proteus, motoru basit modelle simüle ettiği için **gerçek 1 tur = 4096 adım davranışı görülmez**.  

