#include <3664.h>

#include <sci_emaru.c>

// シリアルコンソールで 'G' と 'g' で緑色LEDを操作する

void InitH8(void) {
  IO.PCR8 |= 0x80;     // Port8-Bit7 を出力ポートに設定
  IO.PDR8.BIT.B7 = 0;  // Port8-Bit7: 緑色 LED の初期値=0 に設定
}

void int_sci3(void) {
  unsigned char p;
  SCI3.SSR.BIT.RDRF = 0;  //受信割り込みフラグ クリア
  p = SCI3.RDR;
  if (p == 'G') {
    IO.PDR8.BIT.B7 = 1;
    rs_puts("green light on!\n\r");
  } else if (p == 'g') {
    IO.PDR8.BIT.B7 = 0;
    rs_puts("green light off...\n\r");
  }
}

void main(void) {
  InitH8();
  InitSCI3(br57600);
  rs_puts("\n\r<Serial SMPL start>\n\r");
  EI;  //割り込み要求許可
  while (1) {
  }
}
