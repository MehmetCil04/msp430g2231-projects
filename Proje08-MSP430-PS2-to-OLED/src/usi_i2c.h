#ifndef USI_I2C_H
#define USI_I2C_H

// NOT: En yüksek uyumluluk için standart C tipleri kullanıldı.
// uint16_t -> unsigned int
// uint8_t  -> unsigned char

#define I2C_RESTART   (1<<8)
#define I2C_READ      (2<<8)

typedef enum i2c_state_enum {
  I2C_IDLE = 0,
  I2C_START = 2,
  I2C_PREPARE_ACKNACK = 4,
  I2C_HANDLE_RXTX = 6,
  I2C_RECEIVED_DATA = 8,
  I2C_PREPARE_STOP = 10,
  I2C_STOP = 12
} i2c_state_type;

extern i2c_state_type i2c_state;

void i2c_init(unsigned int usi_clock_divider, unsigned int usi_clock_source);

// BU DOSYA DOĞRU YERDEYSE, main.c BU FONKSİYONU TANIYACAKTIR.
void i2c_send_sequence(const unsigned int* sequence, unsigned int sequence_length, unsigned char *received_data, unsigned int wakeup_sr_bits);

inline unsigned int i2c_done() {
  return (i2c_state == I2C_IDLE);
}

#endif