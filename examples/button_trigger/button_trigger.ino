/**
 * @example button_trigger.ino
 */
#include <Arduino.h>

#include "speech_recognizer.h"

SpeechRecognizer speech_recognizer(0x30);

void setup() {
  Serial.begin(115200);
  speech_recognizer.Setup();
  speech_recognizer.SetActivationMode(SpeechRecognizer::kButtonTrigger);
  speech_recognizer.SetTimeout(5000);  // 5s
  speech_recognizer.AddIdentification(0, F("xiao yi xiao yi"));
  speech_recognizer.AddIdentification(1, F("bei jing"));
  speech_recognizer.AddIdentification(2, F("chong qin"));
  speech_recognizer.AddIdentification(3, F("cheng du"));
  Serial.println("setup done");
}

void loop() {
  if (speech_recognizer.IsActivated()) {
    Serial.println("is activated");
    // do something
  }

  auto result = speech_recognizer.GetResult();
  if (result >= 0) {
    Serial.print("result:"), Serial.println(result);
    // do something

  } else if (result == SpeechRecognizer::kResultTimeout) {
    Serial.println("timeouted");
    // do something
  }
}
