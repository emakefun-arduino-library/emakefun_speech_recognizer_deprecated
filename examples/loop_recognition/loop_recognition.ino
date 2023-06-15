/**
 * @example loop_recognition.ino
 */

#include <Arduino.h>
#include <Wire.h>

#include "speech_recognizer.h"

SpeechRecognizer speech_recognizer(0x30);

void setup() {
  Serial.begin(115200);
  speech_recognizer.Setup();
  speech_recognizer.SetActivationMode(SpeechRecognizer::kAutoStart);
  speech_recognizer.AddIdentification(0, F("xiao yi xiao yi"));
  speech_recognizer.AddIdentification(1, F("bei jing"));
  speech_recognizer.AddIdentification(2, F("chong qin"));
  speech_recognizer.AddIdentification(3, F("cheng du"));
  Serial.println("setup done");
}

void loop() {
  auto result = speech_recognizer.GetResult();
  if (result >= 0) {
    Serial.print("result:"), Serial.println(result);
    // do something
  }
}
