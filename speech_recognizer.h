#pragma once

#include <stdint.h>
#include <WString.h>

class SpeechRecognizer final {
 public:
  enum {
    kDeviceI2cAddress = 0x10,
  };

  enum SpeechRecognitionMode : uint8_t {
    kAuto = 0,
    kKeyWord,
    kButtonTrigger,
  };

  SpeechRecognizer(const uint8_t device_i2c_address = kDeviceI2cAddress);

  void Reset();

  void SetMode(const SpeechRecognitionMode speech_recognition_mode);

  void AddIdentification(const uint8_t index, const String& identification);

  uint8_t GetResult();

 private:
  bool CanSendCommand();
  uint8_t device_i2c_address_;
};