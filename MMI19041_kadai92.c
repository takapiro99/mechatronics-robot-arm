#include <3664.h>

// 40cm以内だったらステッピングモーターを回す

/***AD 変換用構造体設定***/
typedef struct {
  _WORD DATA: 10;
  _WORD DUM: 6;
}

TDumADDR;

/***H8/3664 内部 IO 初期設定***/
void InitH8(void) {
  AD.ADCSR.BYTE = 0; // A/D 変換停止
  AD.ADCSR.BIT.ADIE = 0; // A/D 変換割り込み禁止
  AD.ADCSR.BIT.SCAN = 1; // スキャンモード
  AD.ADCSR.BIT.CKS = 1; // 高速変換
  AD.ADCSR.BIT.CH = 3; // AN3
  AD.ADCSR.BIT.ADST = 1; // A/D 変換スタート
  TA.TMA.BYTE = 0x03; /* 時計用クロック */
  TW.TMRW.BIT.CTS = 0; //TCNT-W カウント停止
  TW.TCRW.BIT.CCLR = 1; //コンペアマッチ A により TCNT をクリア
  TW.TCRW.BIT.CKS = 3; //内部クロック φ/8 をカウント
  TW.TCRW.BIT.TOA = 1; //最初のコンペアマッチが発生するまで FTIOA=1
  TW.TCRW.BIT.TOB = 1; //最初のコンペアマッチが発生するまで FTIOB=1
  TW.TCRW.BIT.TOC = 0; //最初のコンペアマッチが発生するまで FTIOC=1
  TW.TCRW.BIT.TOD = 0; //最初のコンペアマッチが発生するまで FTIOD=0
  TW.TIOR0.BYTE = 0x33; //GRA コンペアマッチで FTIOA 端子をトグル出力とする
  //GRB コンペアマッチで FTIOB 端子をトグル出力とする
  TW.TIOR1.BYTE = 0x33; //GRC コンペアマッチで FTIOC 端子をトグル出力とする
  //GRD コンペアマッチで FTIOD 端子をトグル出力とする
}

void main(void) {
  long raw;
  TDumADDR * ADBuf = (TDumADDR * ) & AD.ADDRD; // AN3=ADDRD に注意
  InitH8();
  EI;
  TW.GRA = 50000;
  TW.GRB = 25000;
  TW.GRC = 50000;
  TW.GRD = 25000;
  while (1) {
    raw = ADBuf -> DATA; // 0-1023 -> adv
    if (raw > 145) { // approximately within 40cm
      TW.TMRW.BIT.CTS = 1;
    } else {
      TW.TMRW.BIT.CTS = 0;
    }
  }
}
