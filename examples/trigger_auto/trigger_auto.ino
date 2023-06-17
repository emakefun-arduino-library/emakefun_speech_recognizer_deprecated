/**
 * @example trigger_auto.ino
 */

#include <Arduino.h>
#include <Wire.h>

#include "speech_recognizer.h"

SpeechRecognizer speech_recognizer(0x30);

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  speech_recognizer.Setup();
  speech_recognizer.SetTrigger(SpeechRecognizer::kTriggerAuto);
  speech_recognizer.AddIdentification(0, F("xiao yi xiao yi"));
  speech_recognizer.AddIdentification(1, F("bei jing"));
  speech_recognizer.AddIdentification(2, F("chong qin"));
  speech_recognizer.AddIdentification(3, F("cheng du"));
  Serial.println("setup done");
}

void loop() {
  switch (speech_recognizer.ReadEvent()) {
    case SpeechRecognizer::kEventRecognizeSuccess: {
      Serial.println(F("kEventRecognizeSuccess"));
      auto result = speech_recognizer.ReadResult();
      Serial.print("result:");
      Serial.println(result);
      // do something
      break;
    }
    case SpeechRecognizer::kEventStartRecognizing: {
      Serial.println(F("kEventStartRecognizing"));
      // do something
      break;
    }
    default:
      break;
  }
}
