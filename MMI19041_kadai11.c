#include <3664.h>

// 緑色LEDを点灯させる

void InitH8(void){
  IO.PCR8 |= 0x80; //Port8-Bit7 を出力ポートに設定
  IO.PDR8.BIT.B7=0; //Port8-Bit7: 緑色 LED の初期値=0 に設定(消灯)
}

void main(void){
  InitH8 ();
  IO.PDR8.BIT.B7=1; //Green LED ON
}
