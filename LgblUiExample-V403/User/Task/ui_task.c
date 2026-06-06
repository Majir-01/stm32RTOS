/**
  * @file       ui_task.h
  * @author     拉咯比哩
  * @version    V1.0.1
  * @date       20260403
  * @brief      UI 任务
  *
  * <h2><center>&copy;此文件版权归【拉咯比哩】所有.</center></h2>
  */
#include "freertos.h"
#include "task.h"
#include "ui_task.h"
#include "oled_device.h"


/// @brief      【单行文本】数据结构
///
/// @note
typedef struct{
	
	uint32_t            x;          // 起始 X 坐标
    uint32_t            y;          // 起始 Y 坐标
    uint8_t             *ucData;    // 要显示的字符串
    emOledFontSizeTdf   emFontSize; // 字号大小

}stSingleLineTextTdf;



static uint8_t s_aucHello[]     = "Hello World!";
static uint8_t s_aucLetter[]    = "ABCD4683414641";
static uint8_t s_aucNumber[]    = "1234";



/// @brief      【Hello】单行文本
///
/// @note
static stSingleLineTextTdf s_stSingleLineTest_Hello = 
{
    .x          = 0,
    .y          = 0,
    .ucData     = s_aucHello,
    .emFontSize = emOledFontSize_8x16,
};


/// @brief      【字母】单行文本
///
/// @note
static stSingleLineTextTdf s_stSingleLineTest_Letter = 
{
    .x          = 0,
    .y          = 20,
    .ucData     = s_aucLetter,
    .emFontSize = emOledFontSize_6x12,
};


/// @brief      【数字】单行文本
///
/// @note
static stSingleLineTextTdf s_stSingleLineTest_Number = 
{
    .x          = 20,
    .y          = 40,
    .ucData     = s_aucNumber,
    .emFontSize = emOledFontSize_12x24,
};


/// @brief      单行文本指针数组
///
/// @note       将三个结构体存储下来
static stSingleLineTextTdf *s_apstSingleLineText[] = 
{
    &s_stSingleLineTest_Hello,
    &s_stSingleLineTest_Letter,
    &s_stSingleLineTest_Number,
};



void vTest()
{
    uint32_t i;
    
    for(i = 0; i < sizeof(s_apstSingleLineText) / sizeof(s_apstSingleLineText[0]); i++)
    {
        vUiWriteStringToBuffer(s_apstSingleLineText[i]->x, 
                               s_apstSingleLineText[i]->y, 
                               s_apstSingleLineText[i]->ucData, 
                               s_apstSingleLineText[i]->emFontSize, 
                               emOledPixelShowMode_Positive, 
                               OLED);
    }
    
    vOledRefreshFromBuffer(OLED);
}



/// @brief      OLED 初始化
///
/// @note
void vOledInit()
{
	stOledStaticParamTdf stInit;
    
    stInit.pstSclGpioBase   = GPIOB;
    stInit.usSclGpioPin     = GPIO_PIN_7;
    stInit.pstSdaGpioBase   = GPIOB;
    stInit.usSdaGpioPin     = GPIO_PIN_6;
	
	vOledDeviceInit(&stInit,OLED);
}


/// @brief      UI 任务
///
/// @note
void vUiTask()
{
	vOledInit();
	while(1)
	{
		
//		vUiWriteStringToBuffer(0,0,(const uint8_t *)"123",emOledFontSize_12x24,emOledPixelShowMode_Positive,OLED);
//		vOledRefreshFromBuffer(OLED);
		
		vTest();
		vTaskDelay(1);
	}


}
