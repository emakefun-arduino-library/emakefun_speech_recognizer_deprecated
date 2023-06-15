#include "speech_recognizer.h"

#include <Arduino.h>
#include <Wire.h>

#define VERSION (1)

namespace {

enum CommandFlag : uint8_t {
  kCanSendCommand = 0,
  kCommandSendCompleted = 1,
};

enum DataAddress : uint8_t {
  kDataAddressVersion = 0x00,
  kDataAddressCommandFlag = 0x01,
  kDataAddressCommand = 0x03,
  kDataAddressIdentificationIndex = 0x04,
  kDataAddressActivationMode = 0x04,
  kDataAddressTimeout = 0x04,
  kDataAddressIdentificationData = 0x05,
  kDataAddressIdentificationLength = 0x37,
  kDataAddressResult = 0x38,
  kDataAddressIsActivated = 0x39,
};

enum Command : uint8_t {
  kCommandNone = 0,
  kReset,
  kAddIdentification,
  kSetActivationMode,
  kSetTimeout,
};
}  // namespace

SpeechRecognizer::SpeechRecognizer(const uint8_t device_i2c_address) : I2cDeviceController(device_i2c_address) {
}

bool SpeechRecognizer::Setup() {
  I2cDeviceController::Setup();
  if (VERSION != Read(kDataAddressVersion)) {
    return false;
  }

  while (!CanSendCommand()) {
    yield();
  }

  return Write(kDataAddressCommand, kReset) == 0 && Write(kDataAddressCommandFlag, kCommandSendCompleted) == 0;
}

void SpeechRecognizer::SetActivationMode(const ActivationMode speech_recognition_mode) {
  while (!CanSendCommand()) {
    yield();
  }

  Write(kDataAddressCommand, {static_cast<uint8_t>(kSetActivationMode), static_cast<uint8_t>(speech_recognition_mode)});
  Write(kDataAddressCommandFlag, kCommandSendCompleted);
}

void SpeechRecognizer::SetTimeout(const uint32_t timeout_ms) {
  while (!CanSendCommand()) {
    yield();
  }

  Write(kDataAddressCommand, kSetTimeout);
  Write(kDataAddressTimeout, reinterpret_cast<const uint8_t*>(&timeout_ms), sizeof(timeout_ms));
  Write(kDataAddressCommandFlag, kCommandSendCompleted);
}

void SpeechRecognizer::AddIdentification(const uint8_t index, const String& identification) {
  while (!CanSendCommand()) {
    yield();
  }

  Write(kDataAddressCommand, {static_cast<uint8_t>(kAddIdentification), index});
  Write(kDataAddressIdentificationData, reinterpret_cast<const uint8_t*>(identification.c_str()), min(255, identification.length()));
  Write(kDataAddressIdentificationLength, min(255, identification.length()));
  Write(kDataAddressCommandFlag, kCommandSendCompleted);
}

int8_t SpeechRecognizer::GetResult() {
  return Read(kDataAddressResult, kResultNone);
}

bool SpeechRecognizer::IsActivated() {
  return Read(kDataAddressIsActivated, 0) == 1;
}

bool SpeechRecognizer::CanSendCommand() {
  return Read(kDataAddressCommandFlag, kCommandSendCompleted) == kCanSendCommand;
}

void SpeechRecognizer::CommandSendCompleted() {
  Write(kDataAddressCommandFlag, kCommandSendCompleted);
}
