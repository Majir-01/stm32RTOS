/**
  * @file       led_device.h
  * @author     拉咯比哩
  * @version    V1.0.1
  * @date       20251203
  * @brief      LED 驱动，基于 STM32 HAL 库
  * 
  * <h2><center>&copy;此文件版权归【拉咯比哩】所有.</center></h2>
  */

#ifndef _LED_DEVICE_H_
#define _LED_DEVICE_H_

#include "project_config.h"
#include "stm32f1xx_hal.h"

#define PI 3.141592653

/// @brief          设备号枚举
///
/// @note 
typedef enum
{
    emLedDevNum0        = 0,
    emLedDevNum1,               // emLedDevNum1 = 1,
    emLedDevNum2,
    emLedDevNum3,
    emLedDevNum4,
    emLedDevNum5,
    emLedDevNum6,
    emLedDevNum7,
    emLedDevNum8,
}
emLedDevNumTdf;

/// @brief          LED ON 时的电平枚举
///
/// @note 
typedef enum
{
    emLedOnLevel_Low    = 0,                // 低电平点亮
    emLedOnLevel_High,                      // 高电平点亮
}
emLedOnLevelTdf;

/// @brief          LED 状态枚举
///
/// @note 
typedef enum
{
    emLedStatus_Off     = 0,                // 状态 OFF
    emLedStatus_On,                         // 状态 ON
}
emLedStatusTdf;

/// @brief          LED 模式枚举
///
/// @note 
typedef enum
{
    emLedMode_Static        = 0,            // 静态模式
    emLedMode_Blink,                        // 闪烁模式
    emLedMode_Breath,                       // 呼吸灯模式
}
emLedModeTdf;

/// @brief          静态参数定义
///
/// @note           
typedef struct
{
    GPIO_TypeDef        *pstGpioBase;       // 使用的 GPIOx
    uint16_t            usGpioPin;          // 使用的 GPIO_PIN_x
    emLedOnLevelTdf     emOnLevel;          // LED 点亮时的电平
}
stLedStaticParamTdf;

/// @brief          运行参数定义
///
/// @note           
typedef struct
{
    emLedStatusTdf      emCurrentStatus;        // 当前状态
    emLedModeTdf        emMode;                 // 模式
    
    uint32_t            ulCurrentCount;         // 当前计数
    uint32_t            ulOnCountThreshold;     // ON  计数阈值
    uint32_t            ulOffCountThreshold;    // OFF 计数阈值
    
    uint32_t            ulBreathPeriod;         // 呼吸周期
}
stLedRunningParamTdf;

/// @brief          结构参数定义
///
/// @note           
typedef struct
{
    stLedStaticParamTdf     stStaticParam;  // 静态参数
    stLedRunningParamTdf    stRunningParam; // 运行参数
}
stLedDeviceParamTdf;

const stLedDeviceParamTdf *c_pstGetLedDeviceParam(emLedDevNumTdf emDevNum);
void vLedDeviceRunningParamInit(stLedRunningParamTdf *pstInit, emLedDevNumTdf emDevNum);
void vLedOn(emLedDevNumTdf emDevNum);
void vLedOff(emLedDevNumTdf emDevNum);
void vLedToggle(emLedDevNumTdf emDevNum);
void vLedDevicePeriodExecute(emLedDevNumTdf emDevNum);
void vLedDeviceInit(stLedStaticParamTdf *pstInit, emLedDevNumTdf emDevNum);

#endif
