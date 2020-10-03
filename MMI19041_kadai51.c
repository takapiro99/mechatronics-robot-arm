#include <3664.h>

#include <sci_emaru.c>

// 可変抵抗の値を 0V ~ 5V で表示する

/***AD 変換用構造体設定***/
typedef struct {
  _WORD DATA : 10;
  _WORD DUM : 6;
} TDumADDR;

/***H8/3664 内部 IO 初期設定***/
void InitH8(void) {
  AD.ADCSR.BYTE = 0;      // A/D 変換停止
  AD.ADCSR.BIT.ADIE = 0;  // A/D 変換割り込み禁止
  AD.ADCSR.BIT.SCAN = 1;  // スキャンモード
  AD.ADCSR.BIT.CKS = 1;   // ?速変換
  AD.ADCSR.BIT.CH = 1;    // AN1
  AD.ADCSR.BIT.ADST = 1;  // A/D 変換スタート
}

void main(void) {
  /*AD 変換用変数*/
  long adv;
  TDumADDR *ADBuf = (TDumADDR *)&AD.ADDRB;
  int d;
  char c;
  InitH8();
  InitSCI3(br57600);
  rs_puts("\n\r<AD Converter SMPL start>\n\r");
  EI;
  while (1) {
    adv = ADBuf->DATA;          // 0-1023
    d = (int)(adv * 0.048876);  // adv*(5./1023)*10
    // 0-1023 を 0-50 に変換
    c = (char)(d / 10);
    rs_putc(c + '0');  // 1 の位の数字を送信
    rs_putc('.');      // 小数点を送信
    c = (char)(d % 10);
    rs_putc(c + '0');  // 小数点 1 位の数字を送信
    rs_putc('V');      // 'V'を送信
    rs_putc('\n');     // 改行コードを送信
  }
}
