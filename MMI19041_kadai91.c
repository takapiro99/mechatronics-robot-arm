#include <3664.h>
#include <sci.c>
unsigned int angle; //グローバル変数の定義

/***AD 変換用構造体設定***/
typedef struct {
  _WORD DATA: 10;
  _WORD DUM: 6;
}

TDumADDR;
/***H8/3664 内部 IO 初期設定***/

void InitH8(void) {
  TW.TMRW.BIT.CTS = 0;     // TCNT-W カウント停止
  TW.TMRW.BIT.PWMB = 1;    // FTIOBの出力モードを PWM に設定
  TW.TCRW.BIT.CCLR = 1;    // コンペアマッチ A により TCNT をクリア
  TW.TCRW.BIT.CKS = 3;     // 内部クロック φ/8 をカウント
  TW.TCRW.BIT.TOB = 0;     // 最初のコンペアマッチが発生するまで FTIOB=0
  // Port1-5 を割り込みとして利用するための設定
  // →割り込み条件が満たされると int_irq1()という関数が実行される
  IO.PMR1.BIT.IRQ1 = 1;    // Port1-Bit5 を割り込みポートとして利用/IRQ1
  IEGR1.BIT.IEG1 = 0;      // IRQ1 の下りを検出
  IENR1.BIT.IEN1 = 1;      // IRQ1 割り込み要求を ON
  // Port1-7 を割り込みとして利用するための設定
  // →割り込み条件が満たされると int_irq3()という関数が実行される
  IO.PMR1.BIT.IRQ3 = 1;    // Port1-Bit7 を割り込みポートとして利用/IRQ3
  IEGR1.BIT.IEG3 = 0;      //IRQ3 端子の下りを検出
  IENR1.BIT.IEN3 = 1;      //IRQ3 割り込み要求を ON
  IO.PCR1 &= 0x5F;         // Port1-Bit5,7 を入力ポートに設定
  IO.PCR8 |= 0xC0;         // Port8-Bit6,7 を出力ポートに設定
  IO.PDR8.BIT.B6 = 0;      // Port8-Bit6: 赤色 LED の初期値=0 に設定(消灯)
  IO.PDR8.BIT.B7 = 0;      // Port8-Bit7: 緑色 LED の初期値=0 に設定(消灯)
  angle = 90;
}


// helper function
void set_green(char light) {
  if (light) {
    IO.PDR8.BIT.B7 = 1;
  } else {
    IO.PDR8.BIT.B7 = 0;
  }
}

void set_red(char light) {
  if (light) {
    IO.PDR8.BIT.B6 = 1;
  } else {
    IO.PDR8.BIT.B6 = 0;
  }
}

void TAwait(int ms) {
  int i;
  for (i = 0; i <= ms; i++) {
    while (IRR1.BIT.IRRTA == 0);   // TCA(TCNT) =オーバーフロー(GRA)になるまで待つ
    IRR1.BIT.IRRTA = 0;            // 検知ﾌﾗｸﾞを戻して再開 
  }
}

void set_motor(int motor_angle) {
  // 0 < motor_angle < 180
  // 26000 27000 28000?
  TW.GRB = 27000 + (motor_angle - 90) * 23;
}

// red button
void int_irq1(void) { //IRQ1 割り込み条件が満たされるとここに処理がジャンプ
  // right turn
  if (angle == 180) {
    set_red(1);
    TAwait(1);
    set_red(0);
  } else {
    angle = angle + 10;
    set_motor(angle);
  }
  IRR1.BIT.IRRI1 = 0; //IRQ1 割り込みフラグをクリア
}

// green button
void int_irq3(void) { //IRQ3 割り込み条件が満たされるとここに処理がジャンプ
  //left turn
  if (angle == 0) {
    set_green(1);
    TAwait(1);
    set_green(0);
  } else {
    angle = angle - 10;
    set_motor(angle);
  }
  IRR1.BIT.IRRI3 = 0; //IRQ3 割り込みフラグをクリア
}

void main(void) {
  InitH8();
  TW.GRA = 30000;       //GRA の値を設定(PWM の周期を設定)
  TW.GRB = 27000;       //GRB の値を設定(PWM のデューティー比を設定)
  TW.TMRW.BIT.CTS = 1;  //Timer W start
  EI;
  while (1) {}
}
