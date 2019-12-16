/*----------------------------------------------------------------------------
楽器の音の波形をフーリエ変換して，音色の違いを解析するプログラム。

[音源ファイルの仕様]
 (1) 波形データを 16bit の整数で記録。数値の範囲は -32768〜32767 (0:無音状態)
 (2) サンプルレートは 44.1 kHz (= 時間間隔 1/44100 秒。音楽 CD と同じ規格)
 (3) 1行に1点のサンプルデータ。全部で 22050 行 (0.5 秒)

coded by Y.Kiyofuji (Jun.24, 2018)
----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* 記号定数の定義 */
#define SAMPLE_RATE 44100 /* 音の波形データのサンプルレート(Hz) */
#define NUM_DATA 22050    /* 音の波形データのサンプル点数 N */
#define NUM_FREQ 11025    /* k の数（周波数の分割数）(= N/2) */

/* プロトタイプ宣言 */
void sinWave( int N, double xt[] );
void windowing( int N, double xt[] );
void fourierTrans( int N, double xt[], double Re_F[], double Im_F[] );

int main(void) {
    /* 変数宣言（配列の要素数は「定数」を使って指定する）*/
    double x_n[NUM_DATA], f_k[NUM_FREQ], I_k[NUM_FREQ], a_k[NUM_FREQ], b_k[NUM_FREQ];

    /* 波形データを生成 */
    sinWave(NUM_DATA, x_n);

    /* 波形データに窓関数を掛ける */
    windowing(NUM_DATA, x_n);

    /* フーリエ変換 */
    fourierTrans(NUM_DATA, x_n, a_k, b_k);

    /* スペクトル密度 I と周波数fを計算 */
    for(int k=0; k<NUM_FREQ; k++){
        I_k[k] = sqrt(a_k[k]*a_k[k] + b_k[k]*b_k[k]);
        f_k[k] = (k / (double)NUM_DATA) * (double)SAMPLE_RATE;
    }

    /* スペクトルを出力 */
    printf("#-------------------------------------------------------------\n");
    printf("# freq(Hz)   Spectrum   Re_F(k)   Im_F(k)\n");
    printf("#-------------------------------------------------------------\n");
    for (int k = 0; k < NUM_FREQ; k++){
        printf("%9.3f   %12.5e   %12.5e   %12.5e\n", f_k[k], I_k[k], a_k[k], b_k[k]);
    }
    printf("#-------------------------------------------------------------\n");

    return 0;
}
/*----------------------------------------------------------------------------
テスト用の正弦波の波形データを生成する関数

(in)  N       : 波形データ xt[] の要素数

(out) xt[i]   : 波形データ

(仮引数の表記について)
    double xt[] は double *xt と書いてもよい。配列の先頭アドレスを受け取る。
----------------------------------------------------------------------------*/
void sinWave( int N, double xt[] ) {
    /* 変数宣言 */
    int i;
    double freq = 440.0;    /* 440 Hz */

    /* 波形データを生成 */
    for (i=0; i<N; i++) {
        xt[i] = 10000.0 * sin( 2.0 * M_PI * freq * i / (double) SAMPLE_RATE );
    }

    return;         /* 関数の戻り値は無い（void 型関数）*/
}
/*----------------------------------------------------------------------------
波形データに "窓関数" をかける関数

(in)  N      : 波形データ xt[] の要素数
      xt[i]  : 波形データ

(out) xt[i]  : 波形データ（窓関数をかけたもの）
----------------------------------------------------------------------------*/
void windowing( int N, double xt[] ) {

/*各波形データに窓関数をかける*/
    for (int i = 0; i < N; i++){
        xt[i] = xt[i]*(1.0/2.0)*(1.0 - cos((2.0*M_PI*i)/(double)N));
    }
    return;		 /* 関数の戻り値は無い（void 型関数）*/
}
/*----------------------------------------------------------------------------
離散 Fourier 変換(DFT)を行う関数

(in)  N      : 元の波形データ xt[] の要素数（サンプル点の数）
      xt[i]  : 元の波形データ

(out) Re_F[k]: フーリエ変換後の関数 X_k の実部 (a_k)
      Im_F[k]: フーリエ変換後の関数 X_k の虚部 (b_k)
----------------------------------------------------------------------------*/
void fourierTrans( int N, double xt[], double Re_F[], double Im_F[] ) {
    /*a_kとb_kを計算*/
    for (int k = 0; k < N/2; k++){
        for (int i = 0; i < N; i++){
            Re_F[k] += xt[i]*cos((2.0*M_PI*k*i)/(double)N);
            Im_F[k] += xt[i]*sin((2.0*M_PI*k*i)/(double)N);
        }
    }
    return;	 /* 関数の戻り値は無い（void 型関数）*/
}
