# Ultrasonik Mesafe Sensörü ile Buzzer Uyarı Sistemi (MSP430G2231)

## Proje Hakkında
Bu proje, **MSP430G2231 mikrodenetleyici** kullanılarak **HC-SR04 ultrasonik sensör** ile mesafe ölçümü yapar ve mesafe belirli bir eşiğin altına düştüğünde **buzzer ile uyarı verir**.  
Kısacası, mesafe kısaldıkça buzzer sesi hem **frekans hem de hız bakımından değişir**.

## Çalışma Prensibi
- HC-SR04 sensörü TRIG pini ile tetiklenir ve ECHO pini üzerinden mesafe bilgisi alınır.
- Mikrodenetleyici, ECHO süresini **Timer_A** ile ölçer.
- Mesafe şu formül ile hesaplanır:  
  \[
  Mesafe (cm) = \frac{echo\_time \times 0.034}{2}
  \]
- Ölçülen mesafe:
  - **15 cm ve üzeri**: Sessiz
  - **12–15 cm**: Düşük frekansta yavaş bip
  - **8–12 cm**: Orta frekansta bip
  - **5–8 cm**: Orta-yüksek frekansta daha hızlı bip
  - **3–5 cm**: Yüksek frekansta çok hızlı bip
  - **0–3 cm**: Çok yüksek frekansta sürekli alarm  

## Donanım Bağlantıları
- **HC-SR04 → MSP430G2231**
  - VCC → 5V  
  - GND → GND  
  - TRIG → P1.0  
  - ECHO → P1.1  
- **Buzzer → MSP430G2231**
  - Pozitif → P1.5  
  - Negatif → GND (seri direnç ile)  

## Proteus Simülasyonu
- Proje **Proteus 8** üzerinde tasarlanmıştır.  
- Kullanılan elemanlar:  
  - MSP430G2231  
  - HC-SR04  
  - Buzzer (Sounder)  
  - Direnç (120Ω)  

> ⚠️ Not: Proteus’taki HC-SR04 modeli basitleştirilmiş olduğundan gerçek donanımda elde edilen mesafelerle küçük farklar olabilir.  

## Gereksinimler
- Code Composer Studio / IAR Embedded Workbench  
- MSP430G2231 LaunchPad veya Proteus simülasyonu  
- HC-SR04 Ultrasonik Sensör  
- Buzzer + Direnç  

## Dosyalar
- **main.c** → MSP430 için yazılmış kod  
- **HC-SR04.pdsprj** → Proteus devre dosyası (örnek isim)  
- README.md → Bu açıklama dosyası  

## Lisans
Bu proje eğitim amaçlıdır. İsteyen herkes kullanabilir ve geliştirebilir.  

<img width="1917" height="987" alt="image" src="https://github.com/user-attachments/assets/d9fd6732-0241-4197-8409-723c2913d774" />
