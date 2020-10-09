#include <3664.h>
#include <sci.c>

/***AD 変換用構造体設定***/
typedef struct {
  _WORD DATA: 10;
  _WORD DUM: 6;
} TDumADDR;


/***H8/3664 内部 IO 初期設定***/
void InitH8(void){
  AD.ADCSR.BYTE= 0; // A/D 変換停?
  AD.ADCSR.BIT.ADIE = 0; // A/D 変換割り込み禁止
  AD.ADCSR.BIT.SCAN = 1; // スキャンモード
  AD.ADCSR.BIT.CKS = 1; // ?速変換
  AD.ADCSR.BIT.CH = 3; // AN3
  AD.ADCSR.BIT.ADST = 1; // A/D 変換スタート
  TA.TMA.BYTE=0x03;   /* 時計用クロック時間0.5ｓ     0000 1001 */
}

void TAwait(int ms){
  int i;
  for(i=0;i<=ms;i++){
    while(IRR1.BIT.IRRTA == 0); // TCA(TCNT) =オーバーフロー(GRA)になるまで待つ
    IRR1.BIT.IRRTA = 0; // 検知ﾌﾗｸﾞを戻して再開 
  }
} 

void main(void){
  long adv;
  TDumADDR *ADBuf = (TDumADDR *)&AD.ADDRD;//AN3=ADDRD に注意
  InitH8 ();
  InitSCI3 (br57600);
  EI;
  while(1){
    adv=ADBuf-> DATA;//0-1023 の値が adv に代入される
    char buffer [5];
    IntToDec(adv, 5, buffer);
    rs_puts(buffer);
    rs_puts("\n");
    TAwait(7);
  }
}
