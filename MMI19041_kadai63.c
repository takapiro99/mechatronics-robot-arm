#include <3664.h>
#include <math.h>

#include <sci_emaru.c>

// 距離センサーで kadai62 の計測から求めた近似に基づいた距離を表示する

/***AD 変換用構造体設定***/
typedef struct {
  _WORD DATA : 10;
  _WORD DUM : 6;
} TDumADDR;

/***H8/3664 内部 IO 初期設定***/
void InitH8(void) {
  AD.ADCSR.BYTE = 0;      // A/D 変換停?
  AD.ADCSR.BIT.ADIE = 0;  // A/D 変換割り込み禁?
  AD.ADCSR.BIT.SCAN = 1;  // スキャンモード
  AD.ADCSR.BIT.CKS = 1;   // ?速変換
  AD.ADCSR.BIT.CH = 3;    // AN3
  AD.ADCSR.BIT.ADST = 1;  // A/D 変換スタート
  TA.TMA.BYTE = 0x03;     /* 時計用クロック */
}

void TAwait(int ms) {
  int i;
  for (i = 0; i <= ms; i++) {
    while (IRR1.BIT.IRRTA == 0)
      ;  // TCA(TCNT) =オーバーフロー(GRA)になるまで待つ
    IRR1.BIT.IRRTA = 0;  // 検知ﾌﾗｸﾞを戻して再開
  }
}

void main(void) {
  long adv;
  long raw;
  TDumADDR *ADBuf = (TDumADDR *)&AD.ADDRD;  // AN3=ADDRD に注意
  int d;
  char c;
  InitH8();
  InitSCI3(br57600);
  EI;
  while (1) {
    raw = ADBuf->DATA;  // 0-1023 -> adv
    // trendline:  433 + -6.24x + 0.0356x^2 + -8.94E-05x^3 + 8.12E-08x^4
    adv = (int)(8.12 * pow(10, -8) * pow(raw, 4) -
                8.94 * pow(10, -5) * pow(raw, 3) + 0.0356 * pow(raw, 2) -
                6.24 * raw + 433);
    int third;
    int second;
    int first;
    // when trendline takes a negative value
    if (raw > 440 || adv < 1) {
      rs_puts("too near\n\r");
    } else {
      // no zero padding
      if (adv >= 100) {
        third = (int)((adv / 100) % 10);
        rs_putc('0' + third);
      }
      if (adv >= 10) {
        second = (int)((adv / 10) % 10);
        rs_putc('0' + second);
      }
      first = (adv % 10);
      rs_putc('0' + first);
      rs_puts("cm\n\r");
    }
    // wait for some time
    TAwait(7);
  }
}
