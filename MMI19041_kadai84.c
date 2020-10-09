#include <3664.h>
#include <sci.c>
unsigned int length; //グローバル変数の定義

// モータが脱調せずに動作する範囲を調べる

/***H8/3664 内部 IO 初期設定***/
void InitH8(void) {
  TW.TMRW.BIT.CTS = 0; //TCNT-W カウント停止
  TW.TCRW.BIT.CCLR = 1; //コンペアマッチ A により TCNT をクリア
  TW.TCRW.BIT.CKS = 3; //内部クロック φ/8 をカウント
  TW.TCRW.BIT.TOA = 1; //最初のコンペアマッチが発生するまで FTIOA=1
  TW.TCRW.BIT.TOB = 1; //最初のコンペアマッチが発生するまで FTIOB=1
  TW.TCRW.BIT.TOC = 0; //最初のコンペアマッチが発生するまで FTIOC=0
  TW.TCRW.BIT.TOD = 0; //最初のコンペアマッチが発生するまで FTIOD=0
  TW.TIOR0.BYTE = 0x33; //GRA コンペアマッチで FTIOA 端子をトグル出力とする
  //GRB コンペアマッチで FTIOB 端子をトグル出力とする
  TW.TIOR1.BYTE = 0x33; //GRC コンペアマッチで FTIOC 端子をトグル出力とする
  //GRD コンペアマッチで FTIOD 端子をトグル出力とする
  IO.PCR1 &= 0x5F;       // Port1-Bit5,7 を入力ポートに設定
  //Port1-5 を??割り込みとして利?するための設定
  // →割り込み条件が満たされると int_irq1()という関数が実行される
  IO.PMR1.BIT.IRQ1=1; //Port1-Bit5 を割り込み??ポートとして利用/IRQ1
  IEGR1.BIT.IEG1=0; //IRQ1 端子の下りを検出
  IENR1.BIT.IEN1=1; //IRQ1 割り込み要求を ON
  //Port1-7 を??割り込みとして利?するための設定
  // →割り込み条件が満たされると int_irq3()という関数が実行される
  IO.PMR1.BIT.IRQ3=1; //Port1-Bit7 を割り込み??ポートとして利用/IRQ3
  IEGR1.BIT.IEG3=0; //IRQ3 端子の下りを検出
  IENR1.BIT.IEN3=1; //IRQ3 割り込み要求を ON
  length = 50000; //グローバル変数の初期化

}

void set_freq(int freq){
  TW.GRA = freq;
  TW.GRB = freq / 2;
  TW.GRC = freq;
  TW.GRD = freq / 2;
}

// red
void int_irq1(void){ //IRQ1 割り込み条件が満たされるとここに処理がジャンプ
  length = length + 100;
  // int to *char
  char buffer [5];
  IntToDec(length, 5, buffer);
  rs_puts(buffer);
  rs_puts("\n");
  set_freq(length);
  IRR1.BIT.IRRI1=0; //IRQ1 割り込みフラグをクリア
}

// green
void int_irq3(void){ //IRQ3 割り込み条件が満たされるとここに処理がジャンプ
  length = length - 100;
  // int to *char
  char buffer [5];
  IntToDec(length, 5, buffer);
  rs_puts(buffer);
  rs_puts("\n");
  set_freq(length);
  IRR1.BIT.IRRI3=0; //IRQ3 割り込みフラグをクリア
}

void main(void) {
  InitH8();
  InitSCI3(br57600);
  rs_puts("\n\r<Serial SMPL start>\n\r");
  TW.GRA = 50000;
  TW.GRB = 25000;
  TW.GRC = 50000;
  TW.GRD = 25000;
  TW.TMRW.BIT.CTS = 1; //Timer W start
  EI;
  while (1) {}
}
