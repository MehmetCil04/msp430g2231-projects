#include <msp430.h>

/* ---- Yazılımsal UART TX: P1.1, ~1200 baud ---- */
#define TXD       BIT1
#define Bit_time  833  // ~1200 baud @ 1 MHz

static inline void uart_char(char c){
    unsigned char i;
    P1DIR |= TXD; P1OUT |= TXD;       // idle=1
    __disable_interrupt();
    P1OUT &= ~TXD;                    // start bit
    __delay_cycles(Bit_time);
    for(i=0;i<8;i++){
        if (c & 1) P1OUT |= TXD; else P1OUT &= ~TXD;
        __delay_cycles(Bit_time);
        c >>= 1;
    }
    P1OUT |= TXD;                     // stop bit
    __delay_cycles(Bit_time);
    __enable_interrupt();
}
static inline void uart_str(const char *s){ while(*s) uart_char(*s++); }

/* ---- PS/2 bağlantı tanımları ---- */
#define PS2_CLK     BIT4   // P1.4 clock
#define PS2_DATA    BIT5   // P1.5 data

volatile unsigned char bits[11];
volatile unsigned char bit_count = 0;
volatile unsigned char byte_ready = 0;

/* ---- PS/2 durum bayrakları ---- */
volatile unsigned char brk = 0;     // 0xF0 görüldü mü (release)
volatile unsigned char shL = 0;     // 0x12 (Left Shift)
volatile unsigned char shR = 0;     // 0x59 (Right Shift)

/* --- küçük tabloyla scancode -> ASCII eşleme --- */
static const unsigned char C_LET[26]={
  0x1C,0x32,0x21,0x23,0x24,0x2B,0x34,0x33,0x43,0x3B,0x42,0x4B,0x3A,
  0x31,0x44,0x4D,0x15,0x2D,0x1B,0x2C,0x3C,0x2A,0x1D,0x22,0x35,0x1A
};
static const char L_LET[26] = "abcdefghijklmnopqrstuvwxyz";
static const unsigned char C_NUM[10] = {0x16,0x1E,0x26,0x25,0x2E,0x36,0x3D,0x3E,0x46,0x45};
static const char L_NUM[10] = "1234567890";

static char map_ascii(unsigned char sc, unsigned char shift_on){
    unsigned char i;
    for(i=0;i<26;i++) if(C_LET[i]==sc){
        char ch=L_LET[i];
        if(shift_on && ch>='a' && ch<='z') ch-=32;  // büyük harfe çevir
        return ch;
    }
    for(i=0;i<10;i++) if(C_NUM[i]==sc) return L_NUM[i]; // (istersen shift sembolleri ekleriz)
    if(sc==0x29) return ' ';      // space
    if(sc==0x5A) return '\n';     // enter
    if(sc==0x66) return '\b';     // backspace
    if(sc==0x0D) return '\t';     // tab
    return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void){
    if (P1IFG & PS2_CLK){
        if (!byte_ready) {
            bits[bit_count] = (P1IN & PS2_DATA) ? 1 : 0;
            bit_count++;
            if (bit_count >= 11) {
                bit_count = 0;
                byte_ready = 1;
            }
        }
        P1IFG &= ~PS2_CLK;  // bayrak temizle
    }
}

int main(void){
    WDTCTL  = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;      // 1 MHz
    DCOCTL  = CALDCO_1MHZ;

    // UART pinini idle yap
    P1DIR |= TXD; P1OUT |= TXD;

    // P1.4 ve P1.5 giriş + pull-up
    P1DIR &= ~(PS2_CLK | PS2_DATA);
    P1REN |=  (PS2_CLK | PS2_DATA);
    P1OUT |=  (PS2_CLK | PS2_DATA);

    // Clock düşen kenar tetikleme
    P1IES |=  PS2_CLK;
    P1IFG &= ~PS2_CLK;
    P1IE  |=  PS2_CLK;

    __enable_interrupt();

    uart_str("PS/2 -> ASCII (1200 baud)\r\n");

    while(1){
        if (byte_ready) {
            unsigned char i;
            /* 11 bitten veri baytını çıkar (bits[1..8], LSB->MSB geliyor) */
            unsigned char sc = 0;
            for (i = 1; i <= 8; i++) {  // veri bitleri
                sc >>= 1;
                if (bits[i]) sc |= 0x80;
            }

            /* --- PS/2 protokol akışı --- */
            if (sc == 0xE0) {           // extended kodlar: şimdilik atla
                byte_ready = 0; 
                continue;
            }
            if (sc == 0xF0) {           // release prefix
                brk = 1;
                byte_ready = 0;
                continue;
            }
            /* Shift tuşları: durum tut */
            if (sc == 0x12) {           // Left Shift
                shL = brk ? 0 : 1; brk=0; byte_ready=0; continue;
            }
            if (sc == 0x59) {           // Right Shift
                shR = brk ? 0 : 1; brk=0; byte_ready=0; continue;
            }

            /* Release ise karakter yazma */
            if (brk) { brk=0; byte_ready=0; continue; }

            /* Make anında ASCII’ye çevir ve yazdır */
            {
                unsigned char shift_on = (shL || shR);
                char ch = map_ascii(sc, shift_on);
                if (ch) {
                    if (ch=='\b'){ uart_char('\b'); uart_char(' '); uart_char('\b'); }
                    else          uart_char(ch);
                }
            }

            byte_ready = 0;
        }
    }
}