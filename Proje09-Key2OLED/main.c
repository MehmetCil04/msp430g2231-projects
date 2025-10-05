#include <msp430.h>
#include "usi_i2c.h"

#define PS2_CLK   BIT4  // P1.4
#define PS2_DATA  BIT5  // P1.5
#define OLED_ADDR 0x3C

volatile unsigned char bits11[11];
volatile unsigned char bit_count = 0;
volatile unsigned char byte_ready = 0;
volatile unsigned char brk = 0;
volatile unsigned char sc = 0;
volatile char direction = 0;  // 1: yukarı, 2: aşağı, 3: sol, 4: sağ

unsigned char cur_x = 0;
unsigned char cur_y = 0;

// Komut gönder
void oled_cmd(const unsigned char *c, unsigned int n){
  unsigned int seq[10], i, m;
  while(n){
    m = (n > 8) ? 8 : n;
    seq[0] = (OLED_ADDR << 1);
    seq[1] = 0x00;
    for(i = 0; i < m; i++) seq[i + 2] = c[i];
    i2c_send_sequence(seq, m + 2, 0, 0);
    while(!i2c_done());
    c += m; n -= m;
  }
}

// Veri gönder
void oled_data(const unsigned char *d, unsigned int n){
  unsigned int seq[10], i, m;
  while(n){
    m = (n > 8) ? 8 : n;
    seq[0] = (OLED_ADDR << 1);
    seq[1] = 0x40;
    for(i = 0; i < m; i++) seq[i + 2] = d[i];
    i2c_send_sequence(seq, m + 2, 0, 0);
    while(!i2c_done());
    d += m; n -= m;
  }
}

// OLED başlat
void oled_init(){
  const unsigned char init_seq[] = {
    0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,
    0x8D, 0x14, 0x20, 0x00, 0xA1, 0xC8, 0xDA, 0x12,
    0x81, 0x7F, 0xA4, 0xA6, 0xAF
  };
  oled_cmd(init_seq, sizeof(init_seq));
}

//Ekranı Temizle
static void clear_all(void){
  const unsigned char set_cols[] = {0x21, 0, 127};
  const unsigned char set_pages[] = {0x22, 0, 7};
  unsigned char zero = 0x00;
  int i;
  oled_cmd(set_cols, 3);
  oled_cmd(set_pages, 3);
  for(i = 0; i < 1024; i++){
    oled_data(&zero, 1);
  }
}


// Ekrana nokta çiz
void draw_dot(unsigned char x, unsigned char y){
  unsigned char page = y / 8;
  unsigned char bit = y % 8;
  unsigned char cmd[] = {0xB0 | page, 0x00 | (x & 0x0F), 0x10 | (x >> 4)};
  unsigned char data[] = {1 << bit};
  oled_cmd(cmd, 3);
  oled_data(data, 1);
}

// PS/2 Clock ISR
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void){
  if (P1IFG & PS2_CLK){
    if (!byte_ready){
      bits11[bit_count] = (P1IN & PS2_DATA) ? 1 : 0;
      bit_count++;
      if (bit_count >= 11){
        bit_count = 0;
        byte_ready = 1;
      }
    }
    P1IFG &= ~PS2_CLK;
  }
}

// PS/2 Başlat
void setup_ps2(){
  P1DIR &= ~(PS2_CLK | PS2_DATA);
  P1REN |=  (PS2_CLK | PS2_DATA);
  P1OUT |=  (PS2_CLK | PS2_DATA);
  P1SEL &= ~(PS2_CLK | PS2_DATA);
  P1IES |=  PS2_CLK;
  P1IFG &= ~PS2_CLK;
  P1IE  |=  PS2_CLK;
}

void main(void){
  WDTCTL = WDTPW | WDTHOLD;
  i2c_init(USIDIV_7, USISSEL_2); // SMCLK / 128
  setup_ps2();
  __enable_interrupt();
  oled_init();
  clear_all();  // Geçmişi temizle

unsigned char prev_x = 0;
unsigned char prev_y = 0;

while(1){
  if (byte_ready){
    unsigned char raw = 0;
    unsigned char i;
    for (i = 1; i <= 8; i++){
      raw >>= 1;
      if (bits11[i]) raw |= 0x80;
    }

    if (raw == 0xF0){ brk = 1; byte_ready = 0; continue; }
    if (raw == 0xE0){ byte_ready = 0; continue; }

    if (!brk){
      sc = raw;
      prev_x = cur_x;
      prev_y = cur_y;

      
      if (sc == 0x1D) cur_y = (cur_y == 0) ? 63 : cur_y - 1;        // W
      else if (sc == 0x1B) cur_y = (cur_y == 63) ? 0 : cur_y + 1;   // S
      else if (sc == 0x1C) cur_x = (cur_x == 0) ? 127 : cur_x - 1;  // A
      else if (sc == 0x23) cur_x = (cur_x == 127) ? 0 : cur_x + 1;  // D

      // Önce eski konumu sil
      unsigned char page_old = prev_y / 8;
      unsigned char bit_old = prev_y % 8;
      unsigned char cmd_old[] = {
        0xB0 | page_old,
        0x00 | (prev_x & 0x0F),
        0x10 | (prev_x >> 4)
      };
      unsigned char data_old[] = {0x00};
      oled_cmd(cmd_old, 3);
      oled_data(data_old, 1);

      // Sonra yeni konumu çiz
      draw_dot(cur_x, cur_y);
    } else {
      brk = 0;
    }

    byte_ready = 0;
  }
}


}