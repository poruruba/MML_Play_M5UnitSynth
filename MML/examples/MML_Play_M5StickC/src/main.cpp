#include <Arduino.h>
#include "MML_Synth.h"

#include <M5UnitSynth.h>

#define SYNTH_DEFAULT_CHANNEL 0

static M5UnitSynth synth;
MML_Synth mml(0);

// 単音出力関数
void dev_tone(uint8_t idx, uint8_t pitch, uint16_t vol) {
  synth.setNoteOn(SYNTH_DEFAULT_CHANNEL, pitch, vol / 15.0 * 127.0);
}

void dev_instrument(uint8_t idx, uint8_t value) {
  synth.setInstrument(0, SYNTH_DEFAULT_CHANNEL, value);
}

// 単音出力停止関数
void dev_notone(uint8_t idx) {
}

// 猫ふんじゃった
const char * mmltext =
  "v1?O6t160v15l16d+c+r8f+rf+rd+c+r8f+rf+rd+c+l8rf+r"
  "f+rl16frfrd+c+r8frfrd+c+r8frfrd+c+l8"
  "rfrfrl16f+rf+rd+c+r8f+rf+rd+c+r8f+r"
  "f+rd+c+l8rf+rf+rl16frfrd+c+r8frfrd+c+r8fr"
  "frd+c+l8rfrfrl16f+rf+rd+c+l8rf+rf+rf+r"
  "f+rf+rf+rl16frfrd+c+l8rfrfrfrfrfrfrl16"
  "f+rf+rd+c+r8f+rf+rd+c+r8f+rf+rd+c+l8r"
  "f+rf+rl16frfrd+c+r8frfrd+c+r8frfrd+c+l8"
  "rfrfrl16f+rf+r8.f+rc+c+d8c+8.frf+?"
  ;
  
// デバッグ出力用
void debug(uint8_t idx, uint8_t c) {
  Serial.write(c);
}

void setup() {
  Serial.begin(115200);

  synth.begin(&Serial2, UNIT_SYNTH_BAUD, 33, 32);

  mml.init(nullptr, dev_tone, dev_notone, dev_instrument, debug);
  mml.setText(mmltext);
  mml.playBGM();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (mml.isBGMPlay()) {
    // 演奏状態で演奏継続可能なら１音再生
    uint32_t tick = millis();
  	if (mml.available(tick))
      mml.playTick(tick);
  }
}
