/**
  * @file       project_config.h
  * @author     拉咯比哩
  * @version    V1.0.1
  * @date       20260103
  * @brief      工程配置文件
  *
  * <h2><center>&copy;此文件版权归【拉咯比哩】所有.</center></h2>
  */ 
  
#ifndef _PROJECT_CONFIG_H_
#define _PROJECT_CONFIG_H_

#define TRUE                                            1
#define FALSE                                           0

#define STM32_IC_HAL                                    "stm32f1xx_hal.h"

#define LED_DEV_NUM                                     1                               // LED 设备数
#define LED_STATUS                                      emLedDevNum0                    


#define BTN_DEV_NUM                                     4                               // 按键设备数
#define BUTTON_UP                                       emBtnDevNum0   
#define BUTTON_DOWN                                     emBtnDevNum1   
#define BUTTON_ENTER                                    emBtnDevNum2   
#define BUTTON_CANCEL                                   emBtnDevNum3   

#define OLED_DEV_NUM                                    1                               // OLED 设备数
#define OLED                                            emOledDevNum0
#define OLED_POINT_WIDTH                                128
#define OLED_POINT_HEIGHT                               64
#define OLED_BUFFER_WIDTH                               OLED_POINT_WIDTH                // OLED 一帧数据的行宽度，等于屏幕一行的像素数量
#define OLED_BUFFER_HEIGHT                              OLED_POINT_HEIGHT / 8           // OLED 一帧数据的列高度，等于屏幕一列的像素数量的 1/8

#endif

