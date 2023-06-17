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
  kDataAddressEvent = 0x38,
  kDataAddressResult = 0x39,
};

enum Command : uint8_t {
  kCommandNone = 0,
  kReset,
  kAddIdentification,
  kSetTrigger,
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

void SpeechRecognizer::SetTrigger(const Trigger trigger) {
  while (!CanSendCommand()) {
    yield();
  }

  Write(kDataAddressCommand, {static_cast<uint8_t>(kSetTrigger), static_cast<uint8_t>(trigger)});
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
  Write(kDataAddressIdentificationData,
        reinterpret_cast<const uint8_t*>(identification.c_str()),
        min(255, identification.length()));
  Write(kDataAddressIdentificationLength, min(255, identification.length()));
  Write(kDataAddressCommandFlag, kCommandSendCompleted);
}

uint8_t SpeechRecognizer::ReadResult() {
  return Read(kDataAddressResult);
}

SpeechRecognizer::Event SpeechRecognizer::ReadEvent() {
  return static_cast<SpeechRecognizer::Event>(Read(kDataAddressEvent, kEventNone));
}

bool SpeechRecognizer::CanSendCommand() {
  return Read(kDataAddressCommandFlag, kCommandSendCompleted) == kCanSendCommand;
}

void SpeechRecognizer::CommandSendCompleted() {
  Write(kDataAddressCommandFlag, kCommandSendCompleted);
}
