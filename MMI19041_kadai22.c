#include <3664.h>

// 赤と緑の LED を XOR に光らせる

void InitH8(void) {
  IO.PCR8 |= 0xC0;     // Port8-Bit6,7 を出?ポートに設定
  IO.PDR8.BIT.B6 = 0;  // Port8-Bit6: ?? LED の初期値=0 に設定(消灯)
  IO.PDR8.BIT.B7 = 0;  // Port8-Bit7: 緑? LED の初期値=0 に設定(消灯)
  IO.PMR1.BIT.IRQ1 = 0;  // Port1-Bit5 を汎??出?ポートとして利?
  IO.PMR1.BIT.IRQ3 = 0;  // Port1-Bit7 を汎??出?ポートとして利?
  IO.PCR1 &= 0x5F;       // Port1-Bit5,7 を??ポートに設定
}

void main(void) {
  InitH8();
  while (1) {
    if (IO.PDR1.BIT.B5 == 0 && IO.PDR1.BIT.B7 == 0) {
      IO.PDR8.BIT.B6 = 0;
      IO.PDR8.BIT.B7 = 0;
    } else {
      if (IO.PDR1.BIT.B5 == 0) {
        IO.PDR8.BIT.B6 = 1;
      } else {
        IO.PDR8.BIT.B6 = 0;
      }

      if (IO.PDR1.BIT.B7 == 0) {
        IO.PDR8.BIT.B7 = 1;
      } else {
        IO.PDR8.BIT.B7 = 0;
      }
    }
  }
}
