#include <3664.h>

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
}

void main(void) {
  InitH8();
  TW.GRA = 25000;
  TW.GRB = 12500;
  TW.GRC = 25000;
  TW.GRD = 12500;
  TW.TMRW.BIT.CTS = 1; //Timer W start
  EI;
  while (1) {}
}
