#include <msp430.h>
#include "usi_i2c.h"

/* --- SSD1306 I2C adresi ve küçük blok gönderimi --- */
#define OLED_ADDR 0x3C
#define CHUNK     8

static void cmd(const unsigned char* c, unsigned int n){
  unsigned int seq[CHUNK+2], k, m;
  while(n){
    m = (n>CHUNK)?CHUNK:n;
    seq[0]=(OLED_ADDR<<1); seq[1]=0x00;
    for(k=0;k<m;k++) seq[k+2]=c[k];
    i2c_send_sequence(seq, m+2, 0, 0);
    while(!i2c_done());
    c+=m; n-=m;
  }
}
static void data(const unsigned char* d, unsigned int n){
  unsigned int seq[CHUNK+2], k, m;
  while(n){
    m = (n>CHUNK)?CHUNK:n;
    seq[0]=(OLED_ADDR<<1); seq[1]=0x40;
    for(k=0;k<m;k++) seq[k+2]=d[k];
    i2c_send_sequence(seq, m+2, 0, 0);
    while(!i2c_done());
    d+=m; n-=m;
  }
}

/* --- SSD1306 init (senin dizi) --- */
static const unsigned char init_seq[] = {
  0xAE,0xD5,0x80,0xA8,0x3F,0xD3,0x00,0x40,0x8D,0x14,0x20,0x00,
  0xA1,0xC8,0xDA,0x12,0x81,0xCF,0xD9,0xF1,0xDB,0x40,0xA4,0xA6,0x2E,0xAF
};

static void clear_all(void){
  static const unsigned char set_all_cols[] = {0x21,0,127};
  static const unsigned char set_all_pages[]={0x22,0,7};
  unsigned char zero[16]={0};
  int k;
  cmd(set_all_cols,3); cmd(set_all_pages,3);
  for(k=0;k<1024/16;k++) data(zero,16);
}
static inline void set_cursor(unsigned char col, unsigned char page){
  unsigned char c[3] = {0x21, col, 127};
  unsigned char p[3] = {0x22, page, page};
  cmd(c,3); cmd(p,3);
}

/* --- 5x7 + boşluk glifleri --- */
static const unsigned char GLYPH_a[6]     = { 0x20,0x54,0x54,0x54,0x78,0x00 };
static const unsigned char GLYPH_space[6] = { 0,0,0,0,0,0 };

/* --- İmleç --- */
static unsigned char cur_col = 0;   // 0..127
static unsigned char cur_page = 3;  // 0..7

static void newline(void){
  cur_col = 0;
  cur_page = (cur_page < 7) ? (cur_page+1) : 0;
}
static void ensure_wrap(void){
  if (cur_col > 122){ // 128-6
    newline();
  }
}
static void write_glyph6(const unsigned char g6[6]){
  ensure_wrap();
  set_cursor(cur_col, cur_page);
  data(g6, 6);
  cur_col += 6;
}
static void do_backspace(void){
  if (cur_col >= 6){
    cur_col -= 6;
  } else {
    if (cur_page>0){ cur_page--; cur_col=122; }
    else { cur_page=7; cur_col=122; }
  }
  set_cursor(cur_col, cur_page);
  data(GLYPH_space, 6);
}

/* --- PS/2 --- */
#define PS2_CLK   BIT4
#define PS2_DATA  BIT5

volatile unsigned char bits11[11];
volatile unsigned char bit_count = 0;
volatile unsigned char byte_ready = 0;
volatile unsigned char brk = 0;

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

int main(void){
  WDTCTL = WDTPW | WDTHOLD;

  /* DCO 1 MHz */
  if (CALBC1_1MHZ == 0xFF) for(;;);
  DCOCTL = 0;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;

  /* I2C başlat */
  i2c_init(USIDIV_7, USISSEL_2);
  __delay_cycles(100000);

  /* OLED init */
  cmd(init_seq, sizeof(init_seq));
  clear_all();

  /* PS/2 pin ayarı */
  P1DIR &= ~(PS2_CLK | PS2_DATA);
  P1REN |=  (PS2_CLK | PS2_DATA);
  P1OUT |=  (PS2_CLK | PS2_DATA);

  P1SEL &= ~(PS2_CLK | PS2_DATA);   // sadece P1SEL var
  P1IES |=  PS2_CLK;
  P1IFG &= ~PS2_CLK;
  P1IE  |=  PS2_CLK;
  __enable_interrupt();

  cur_page=3; cur_col=0;

  for(;;){
    if (byte_ready){
      unsigned char sc=0,i;
      for (i=1;i<=8;i++){ sc >>=1; if(bits11[i]) sc|=0x80; }

      if (sc==0xE0){ byte_ready=0; continue; }
      if (sc==0xF0){ brk=1; byte_ready=0; continue; }

      if (!brk){
        if (sc==0x1C){ write_glyph6(GLYPH_a); }     // 'a'
        else if (sc==0x29){ write_glyph6(GLYPH_space); } // space
        else if (sc==0x5A){ newline(); }
        else if (sc==0x66){ do_backspace(); }
      } else {
        brk=0;
      }
      byte_ready=0;
    }
  }
}