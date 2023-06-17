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
   * @brief 触发器
   */
  enum Trigger : uint8_t {
    kTriggerAuto = 0,         /**< 自动触发，循环识别 */
    kTriggerButton = 1 << 0,  /**< 按键触发 */
    kTriggerKeyword = 1 << 1, /**< 关键词触发 */
  };

  /**
   * @brief 事件类型
   */

  enum Event : uint8_t {
    kEventNone = 0,           /**< 无任何事件 */
    kEventWaitForTrigger,     /**< 等待触发唤醒 */
    kEventStartRecognizing,   /**< 开始识别 */
    kEventRecognizeSuccess,   /**< 识别成功 */
    kEventRecognizeTimeout,   /**< 识别超时*/
    kEventTriggeredByButton,  /**< 被按键触发唤醒 */
    kEventTriggeredByKeyword, /**< 被关键词触发唤醒 */
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
   * @retval false 失败，如I2C无法与语音识别模块通讯
   */
  bool Setup() override;

  /**
   * @brief 设置触发器
   * @param[in] trigger 触发器，参考枚举: @ref Trigger
   */
  void SetTrigger(const Trigger trigger);

  /**
   * @brief 设置识别超时时间, 对自动触发( @ref SpeechRecognizer::kTriggerAuto )无效
   * @param[in] timeout_ms 超时时间，单位毫秒
   */
  void SetTimeout(const uint32_t timeout_ms);

  /**
   * @brief 添加语音识别索引词
   * @details 最多可以添加15个词，每个词最大长度为50个字节
   * @param[in] index 索引，范围0 ~
   * 255，多个词可以共用一个索引，当在关键字触发( @ref SpeechRecognizer::kTriggerKeyword )模式下，索引0会被当做关键词
   * @param[in] identification 索引词，String类型，最大长度为50个字节
   */
  void AddIdentification(const uint8_t index, const String& identification);

  /**
   * @brief 获取语音识别结果，通过 @ref ReadEvent() 检测到识别成功( @ref SpeechRecognizer::kEventRecognizeSuccess
   * )的事件后再调用
   * @return 索引词的索引值，由 @ref AddIdentification 设置
   */
  uint8_t ReadResult();

  /**
   * @brief 读取事件
   * @return 事件类型，参考枚举: @ref Event
   */
  Event ReadEvent();

 private:
  SpeechRecognizer(const SpeechRecognizer&) = delete;
  SpeechRecognizer& operator=(const SpeechRecognizer&) = delete;
  bool CanSendCommand();
  void CommandSendCompleted();
};