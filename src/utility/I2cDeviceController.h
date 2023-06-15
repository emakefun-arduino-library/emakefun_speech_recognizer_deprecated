#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>

class I2cDeviceController {
 public:
  I2cDeviceController(const uint8_t device_i2c_address) : device_i2c_address_(device_i2c_address) {
  }

  virtual ~I2cDeviceController() = default;

  virtual bool Setup() {
    Wire.begin();
    return true;
  }

  uint8_t Address() const {
    return device_i2c_address_;
  }

 protected:
  int8_t Write(const uint8_t write_address, const uint8_t* data, const uint8_t length) {
    uint8_t sent_length = 0;
    while (sent_length < length) {
      uint8_t segment_length = min(length - sent_length, BUFFER_LENGTH - 1);
      Wire.beginTransmission(device_i2c_address_);
      Wire.write(write_address + sent_length);
      Wire.write(data + sent_length, segment_length);
      auto transmission_result = Wire.endTransmission();
      if (transmission_result != 0) {
        return -transmission_result;
      }
      sent_length += segment_length;
    }

    return 0;
  }

  int8_t Write(const uint8_t write_address, const uint8_t data) {
    Wire.beginTransmission(device_i2c_address_);
    Wire.write(write_address);
    Wire.write(data);
    auto transmission_result = Wire.endTransmission();
    return -transmission_result;
  }

  template <size_t size>
  int8_t Write(const uint8_t write_address, const uint8_t (&array)[size]) {
    return Write(write_address, array, size);
  }

  int8_t Read(const uint8_t read_address, uint8_t* const buffer, const uint8_t length) {
    if (buffer == nullptr || length == 0) {
      return -1;
    }
    Wire.beginTransmission(device_i2c_address_);
    Wire.write(read_address);
    auto transmission_result = Wire.endTransmission();
    if (transmission_result != 0) {
      return -transmission_result;
    }

    if (length != Wire.requestFrom(device_i2c_address_, length)) {
      return -1;
    }

    if (length != Wire.readBytes(buffer, length)) {
      return -1;
    }

    return 0;
  }

  uint8_t Read(const uint8_t read_address, const uint8_t default_vale = 0) {
    uint8_t data = default_vale;
    if (0 != Read(read_address, &data, 1)) {
      return default_vale;
    }
    return data;
  }

 private:
  const uint8_t device_i2c_address_;
};