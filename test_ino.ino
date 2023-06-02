#include <Arduino.h>
#include <Wire.h>

#include "log.h"
#include "speech_recognizer.h"

SpeechRecognizer speech_recognizer;

void setup() {
  Serial.begin(115200);
  speech_recognizer.Reset();
  speech_recognizer.SetMode(SpeechRecognizer::kKeyWord);
  speech_recognizer.AddIdentification(0, F("xiao yi xiao yi"));
  speech_recognizer.AddIdentification(1, F("bei jing"));
  speech_recognizer.AddIdentification(2, F("chong qin"));
  speech_recognizer.AddIdentification(3, F("cheng du"));
}

void loop() {
  auto result = speech_recognizer.GetResult();
  if (result != 0xFF) {
    LOG(INFO) << result;
  }
}
