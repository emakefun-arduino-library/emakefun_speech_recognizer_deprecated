#pragma once

#include <WString.h>
#include <stdint.h>

#include "utility/I2cDeviceController.h"

/**
 * @brief 语音识别模块控制类
 */
class SpeechRecognizer : public I2cDeviceController {
 public:
  enum : uint8_t {
    /**
     * @brief 语音识别模块默认I2C地址
     */
    kDeviceI2cAddress = 0x30,
  };

  /**
   * @brief 激活模式
   */
  enum ActivationMode : uint8_t {
    /**
     * @brief 自动激活模式
     */
    kAutoStart = 0,
    /**
     * @brief 关键词触发激活模式
     */
    kKeyWordTrigger,
    /**
     * @brief 按钮触发激活模式
     */
    kButtonTrigger,
  };

  /**
   * @brief 语音识别结果
   */
  enum Result : int8_t {
    /**
     * @brief 超时
     */
    kResultTimeout = -2,
    /**
     * @brief 无识别结果
     */
    kResultNone = -1,
  };

  /**
   * @brief 构造函数
   * @param device_i2c_address 语音识别模块I2C地址，默认为0x30
   */
  SpeechRecognizer(const uint8_t device_i2c_address = kDeviceI2cAddress);

  /**
   * @brief 析构函数
   */
  virtual ~SpeechRecognizer() = default;

  /**
   * @brief 初始化函数
   * @return 初始化结果，
   * @retval true 成功
   * @retval false 失败
   */
  bool Setup() override;

  /**
   * @brief 设置激活模式
   * @param[in] speech_recognition_mode 激活模式，参考@ref ActivationMode
   */
  void SetActivationMode(const ActivationMode speech_recognition_mode);

  /**
   * @brief 设置操作超时时间
   * @param[in] timeout_ms 超时时间，单位毫秒
   */
  void SetTimeout(const uint32_t timeout_ms);

  /**
   * @brief 添加语音识别索引词
   * @details 最多可以添加15个词，每个词最大长度为50个字节
   * @param[in] index 索引，范围0 ~ 255，多个词可以共用一个索引，
   * @param[in] identification 索引词，String类型，最大长度为50个字节
   */
  void AddIdentification(const uint8_t index, const String& identification);

  /**
   * @brief 获取语音识别结果
   * @return 语音识别结果，kResultNone表示无结果，kResultTimeout表示超时
   * @retval SpeechRecognizer::kResultTimeout 识别超时
   * @retval SpeechRecognizer::kResultNone 无结果
   * @retval >=0 索引，由 @ref AddIdentification 设置
   */
  int8_t GetResult();

  /**
   * @brief 查询是否已激活
   * @details 每次通过该接口查询之后，系统会自动清除该标记位将其置为false，下次重新激活才会自动置为true；
   *          该接口在自动激活模式( @ref kAutoStart )下无效
   * @retval true 已经激活
   * @retval false 未激活
   */
  bool IsActivated();

 private:
  SpeechRecognizer(const SpeechRecognizer&) = delete;
  SpeechRecognizer& operator=(const SpeechRecognizer&) = delete;
  bool CanSendCommand();
  void CommandSendCompleted();
};