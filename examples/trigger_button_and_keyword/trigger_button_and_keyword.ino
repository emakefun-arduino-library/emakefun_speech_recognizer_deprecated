/**
 * @example trigger_button_and_keyword.ino
 */
#include <Arduino.h>

#include "speech_recognizer.h"

SpeechRecognizer speech_recognizer(0x30);

void setup() {
  Serial.begin(115200);
  speech_recognizer.Setup();
  speech_recognizer.SetTrigger(SpeechRecognizer::kTriggerButton | SpeechRecognizer::kTriggerKeyword);
  speech_recognizer.SetTimeout(10000);  // 10000ms
  speech_recognizer.AddIdentification(0, F("xiao yi xiao yi"));
  speech_recognizer.AddIdentification(1, F("bei jing"));
  speech_recognizer.AddIdentification(2, F("chong qin"));
  speech_recognizer.AddIdentification(3, F("cheng du"));
  Serial.println("setup done");
}

void loop() {
  switch (speech_recognizer.ReadEvent()) {
    case SpeechRecognizer::kEventWaitForTrigger: {
      Serial.println(F("kEventWaitForTrigger"));
      // do something
      break;
    }
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
    case SpeechRecognizer::kEventRecognizeTimeout: {
      Serial.println(F("kEventRecognizeTimeout"));
      // do something
      break;
    }
    case SpeechRecognizer::kEventTriggeredByButton: {
      Serial.println(F("kEventTriggeredByButton"));
      // do something
      break;
    }
    case SpeechRecognizer::kEventTriggeredByKeyword: {
      Serial.println(F("kEventTriggeredByKeyword"));
      // do something
      break;
    }
    default: {
      break;
    }
  }
}
