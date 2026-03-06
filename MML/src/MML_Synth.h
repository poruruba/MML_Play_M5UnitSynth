//
// MMLクラスライブラリ V1.0
// 作成日 2019/03/24 by たま吉さん
// 最終更新日 2019/05/23 by たま吉さん
// 変更  2026/03/01,M5Unit Synth対応
//

#ifndef ___MML_Synth_h___
#define ___MML_Synth_h___

#include "Arduino.h"

// エラーメッセージ定義
#define ERR_MML 1

// デフォルト設定値
#define MML_tempo     120 // テンポ(32～255)
#define MML_len       4   // 長さ(1,2,4,8,16,32)
#define MML_oct       4   // 音の高さ(1～8)
#define MML_vol       15  // 音の大きさ(1～15)

// 引数範囲
#define MML_MAX_VOL  15   // 音の大きさ(1～15)
#define MML_MAX_OCT  8    // 音の高さ(1～8)
#define MML_MAX_TMPO 255  //  テンポ(32～255)
#define MML_MIN_TMPO 32   //  テンポ(32～255)

// 演奏モード(play()の引数)
#define MML_BGM         0b001  // バックグラウンド演奏
#define MML_RESUME      0b010  // 中断位置から演奏
#define MML_REPEAT      0b100  // 繰り返し演奏

class MML_Synth {
  private:
    uint8_t idx = 0;
    uint8_t   err = 0;                      // Error message index
    uint16_t  common_tempo = MML_tempo ;    // 共通テンポ
    uint16_t  common_len   = MML_len   ;    // 共通長さ
    uint8_t   common_oct   = MML_oct   ;    // 共通高さ
    uint8_t   common_vol   = MML_vol   ;    // 音の大きさ
    uint8_t   flgdbug = 0;                  // デバッグ出力フラグ

    char      *mml_text = 0;                // MML文テキスト
    char      *mml_ptr = 0;                 // 再生位置
    uint16_t  playduration = 0;             // 処理中の音符・休符の長さ（ミリ秒）
    uint8_t   flgR = 0;                     // 休符演奏フラグ
    uint32_t  endTick = 0;                  // 再生終了システム時間（ミリ秒）
    uint32_t  repeat  = 0;                  // 繰り返し演奏
    volatile uint8_t   flgRun;              // 実行中状態
    volatile uint8_t   playMode = 0;        // 演奏状態 0:停止 1:フォアグランド演奏 2:バックグラウンド演奏

    void (*func_instrument)(uint8_t idx, uint8_t value) = 0;
    void (*func_tone)(uint8_t idx, uint8_t pitch, uint16_t vol) = 0;
    void (*func_notone)(uint8_t idx) = 0;
    void (*func_init)(uint8_t idx) = 0;
    void (*func_putc)(uint8_t idx, uint8_t c) = 0;
  
  private:
	  int16_t getParamLen();
    int16_t getParam();
    void instrument(uint8_t value);
    void tone(uint8_t pitch, uint16_t vol=15);
    void notone();
    void debug(uint8_t c);
    void init();   

  public:
    MML_Synth(uint8_t index = 0) : idx(index) {}

    // ハードウェア依存関数のアタッチ
    void init(
       void (*f1)(uint8_t idx), 
       void (*f2)(uint8_t idx, uint8_t pitch, uint16_t vol),
       void (*f3)(uint8_t idx), 
       void (*f4)(uint8_t idx, uint8_t value) = 0,
       void (*f5)(uint8_t idx, uint8_t c) = 0    ) 
    { func_init = f1; func_tone = f2; func_notone = f3; func_instrument = f4; func_putc = f5; init(); }; 


    void setText(const char* text)  // MML文の登録
    { mml_text = (char *)text; };

    void tempo(int16_t tempo);

    void playTick(uint32_t tick, uint8_t flgTick = true);
    void play(uint8_t mode = 0);
    void playBGM(uint8_t mode = 0) {play(MML_BGM|mode);};
    uint8_t isBGMPlay()  {return ((playMode == 2) && flgRun); };
    uint8_t isPlay()  {return ((playMode == 1) && flgRun); };
    uint8_t available(uint32_t tick); 
    uint8_t isError()  {return err; };
    void stop() { notone(); flgRun = 0;};
    void resume() {if (*mml_text) flgRun = 1; };
    void reset();
};

#endif
