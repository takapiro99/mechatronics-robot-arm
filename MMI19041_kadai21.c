#include <3664.h>

// 赤と緑のLEDがスイッチを押している間だけ消える

void InitH8(void) {
  IO.PCR8 |= 0xC0;       // Port8-Bit6,7 を出力ポートに設定
  IO.PDR8.BIT.B6 = 1;    // Port8-Bit6: 赤色 LED の初期値=1 に設定
  IO.PDR8.BIT.B7 = 1;    // Port8-Bit7: 緑色 LED の初期値=1 に設定
  IO.PMR1.BIT.IRQ1 = 0;  // Port1-Bit5 を汎用出力ポートとして利用
  IO.PMR1.BIT.IRQ3 = 0;  // Port1-Bit7 を汎用出力ポートとして利用
  IO.PCR1 &= 0x5F;       // Port1-Bit5,7 を入力ポートに設定
}

void main(void) {
  InitH8();
  while (1) {
    if (IO.PDR1.BIT.B5 == 0) {
      IO.PDR8.BIT.B6 = 0;
    } else {
      IO.PDR8.BIT.B6 = 1;
    }

    if (IO.PDR1.BIT.B7 == 0) {
      IO.PDR8.BIT.B7 = 0;
    } else {
      IO.PDR8.BIT.B7 = 1;
    }
  }
}
