#include <msp430.h>
#include "usi_i2c.h"

// NOT: En yüksek uyumluluk için standart C tipleri kullanıldı.
static const unsigned int *i2c_sequence;
static unsigned int i2c_sequence_length;
static unsigned char *i2c_receive_buffer;
static unsigned int i2c_wakeup_sr_bits;
i2c_state_type i2c_state = I2C_IDLE;

static inline void i2c_prepare_stop();
static inline void i2c_prepare_data_xmit_recv();

void i2c_send_sequence(const unsigned int* sequence, unsigned int sequence_length, unsigned char *received_data, unsigned int wakeup_sr_bits) {
  while(i2c_state != I2C_IDLE);
  i2c_sequence = sequence;
  i2c_sequence_length = sequence_length;
  i2c_receive_buffer = received_data;
  i2c_wakeup_sr_bits = wakeup_sr_bits;
  i2c_state = I2C_START;
  USICTL1 |= USIIFG;
}

static inline void i2c_prepare_stop() {
  USICTL0 |= USIOE;
  USISRL = 0x00;
  USICNT |= 0x01;
  i2c_state = I2C_STOP;
}

static inline void i2c_prepare_data_xmit_recv() {
  if (i2c_sequence_length == 0) {
    i2c_prepare_stop();
  } else {
    if (*i2c_sequence == I2C_RESTART) {
      USICTL0 |= USIOE;
      USISRL = 0xff;
      USICNT = (USICNT & 0xE0) | 1;
      i2c_state = I2C_START;
    } else if (*i2c_sequence == I2C_READ) {
      USICTL0 &= ~USIOE;
      USICNT = (USICNT & 0xE0) | 8;
      i2c_state = I2C_RECEIVED_DATA;
    } else {
      USICTL0 |= USIOE;
      USISRL = (char)(*i2c_sequence);
      USICNT = (USICNT & 0xE0) | 8;
      i2c_state = I2C_PREPARE_ACKNACK;
    }
    i2c_sequence++;
    i2c_sequence_length--;
  }
}

#pragma vector = USI_VECTOR
__interrupt void USI_TXRX(void)
{
  switch(__even_in_range(i2c_state,12)) {
    case I2C_IDLE: break;
    case I2C_START:
      USISRL = 0x00;
      USICTL0 |= (USIGE|USIOE);
      USICTL0 &= ~USIGE;
     i2c_prepare_data_xmit_recv();
      break;
    case I2C_PREPARE_ACKNACK:
      USICTL0 &= ~USIOE;
      USICNT |= 0x01;
        i2c_state = I2C_HANDLE_RXTX;
      break;
    case I2C_HANDLE_RXTX:
      if ((USISRL & BIT0) != 0) {
        i2c_prepare_stop();
      } else {
        i2c_prepare_data_xmit_recv();
      }
      break;
    case I2C_RECEIVED_DATA:
      *i2c_receive_buffer = USISRL;
      i2c_receive_buffer++;
      USICTL0 |= USIOE;
      if (i2c_sequence_length > 0) {
        USISRL = 0x00;
        i2c_state = I2C_HANDLE_RXTX;
      } else {
        USISRL = 0xff;
        i2c_state = I2C_PREPARE_STOP;
      }
      USICNT |= 0x01;
      break;
    case I2C_PREPARE_STOP:
      i2c_prepare_stop();
      break;
    case I2C_STOP:
      USISRL = 0x0FF;
      USICTL0 |= USIGE;
      USICTL0 &= ~(USIGE|USIOE);
      i2c_state = I2C_IDLE;
      if (i2c_wakeup_sr_bits) {
        _bic_SR_register_on_exit(i2c_wakeup_sr_bits);
      }
      break;
  }
  USICTL1 &= ~USIIFG;
}

void i2c_init(unsigned int usi_clock_divider, unsigned int usi_clock_source) {
  _disable_interrupts();
  P1OUT |= (BIT6 | BIT7);
  P1REN |= (BIT6 | BIT7);
  P1DIR |= (BIT6 | BIT7);
  USICTL0 = USIPE6|USIPE7|USIMST|USISWRST;
  USICTL1 = USII2C|USIIE;
  USICKCTL = usi_clock_divider | usi_clock_source | USICKPL;
  USICNT |= USIIFGCC;
  USICTL0 &= ~USISWRST;
  USICTL1 &= ~USIIFG;
  _enable_interrupts();
}