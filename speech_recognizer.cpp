#include "speech_recognizer.h"

#include <Arduino.h>
#include <Wire.h>

#include "log.h"

namespace {
enum RegisterAddress : uint8_t {
  kRegisterAddressVersion = 0x00,
  kRegisterAddressCommand = 0x01,
  kRegisterAddressIdentificationIndex = 0x02,
  kRegisterAddressIdentificationData = 0x03,
  kRegisterAddressIdentificationLength = 0x35,
  kRegisterAddressMode = 0x02,
  kRegisterAddressResult = 0x36,
};

enum Command : uint8_t {
  kNone = 0,
  kReset,
  kAddIdentification,
  kSetMode,
  kStartAsr,
};
}  // namespace

SpeechRecognizer::SpeechRecognizer(const uint8_t device_i2c_address) : device_i2c_address_(device_i2c_address) {
  Wire.begin();
}

void SpeechRecognizer::Reset() {
  while (!CanSendCommand()) {
    yield();
  }
  Wire.beginTransmission(device_i2c_address_);
  Wire.write(kRegisterAddressCommand);
  Wire.write(kReset);
  auto transmission_result = Wire.endTransmission();
  if (transmission_result != 0) {
    LOG(ERROR) << "wire transmission failed:" << transmission_result;
    return;
  }
}

void SpeechRecognizer::SetMode(const SpeechRecognitionMode speech_recognition_mode) {
  while (!CanSendCommand()) {
    yield();
  }

  Wire.beginTransmission(device_i2c_address_);
  Wire.write(kRegisterAddressMode);
  Wire.write(speech_recognition_mode);
  auto transmission_result = Wire.endTransmission();
  if (transmission_result != 0) {
    LOG(ERROR) << "wire transmission failed:" << transmission_result;
    return;
  }

  Wire.beginTransmission(device_i2c_address_);
  Wire.write(kRegisterAddressCommand);
  Wire.write(kSetMode);
  transmission_result = Wire.endTransmission();
  if (transmission_result != 0) {
    LOG(ERROR) << "wire transmission failed:" << transmission_result;
    return;
  }
}

void SpeechRecognizer::AddIdentification(const uint8_t index, const String& identification) {
  while (!CanSendCommand()) {
    yield();
  }

  Wire.beginTransmission(device_i2c_address_);
  Wire.write(kRegisterAddressIdentificationIndex);
  Wire.write(index);
  auto transmission_result = Wire.endTransmission();
  if (transmission_result != 0) {
    LOG(ERROR) << "wire transmission failed:" << transmission_result;
    return;
  }

  Wire.beginTransmission(device_i2c_address_);
  Wire.write(kRegisterAddressIdentificationData);
  Wire.write(identification.c_str(), identification.length());
  transmission_result = Wire.endTransmission();
  if (transmission_result != 0) {
    LOG(ERROR) << "wire transmission failed:" << transmission_result;
    return;
  }

  Wire.beginTransmission(device_i2c_address_);
  Wire.write(kRegisterAddressIdentificationLength);
  Wire.write(identification.length());
  transmission_result = Wire.endTransmission();
  if (transmission_result != 0) {
    LOG(ERROR) << "wire transmission failed:" << transmission_result;
    return;
  }

  Wire.beginTransmission(device_i2c_address_);
  Wire.write(kRegisterAddressCommand);
  Wire.write(kAddIdentification);
  transmission_result = Wire.endTransmission();
  if (transmission_result != 0) {
    LOG(ERROR) << "wire transmission failed:" << transmission_result;
    return;
  }
}

uint8_t SpeechRecognizer::GetResult() {
  Wire.beginTransmission(device_i2c_address_);
  Wire.write(kRegisterAddressResult);
  auto transmission_result = Wire.endTransmission();
  if (transmission_result != 0) {
    LOG(ERROR) << "wire transmission failed:" << transmission_result;
    return 0xFF;
  }

  if (1 != Wire.requestFrom(device_i2c_address_, static_cast<uint8_t>(1))) {
    LOG(ERROR) << "wire requestFrom failed";
    return 0xFF;
  }

  if (Wire.available()) {
    return Wire.read();
  }

  return 0xFF;
}

bool SpeechRecognizer::CanSendCommand() {
  Wire.beginTransmission(device_i2c_address_);
  Wire.write(kRegisterAddressCommand);
  auto transmission_result = Wire.endTransmission();
  if (transmission_result != 0) {
    LOG(ERROR) << "wire transmission failed:" << transmission_result;
    return false;
  }

  if (1 != Wire.requestFrom(device_i2c_address_, static_cast<uint8_t>(1))) {
    LOG(ERROR) << "wire requestFrom failed";
    return false;
  }

  if (Wire.available()) {
    return Wire.read() == kNone;
  }

  return false;
}
