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
#include "string.h"
#include "led_device.h"
#include "button_device.h"


/// @brief      按钮回调函数声明
///
/// @note
void vButtonCallback_On(void);
void vButtonCallback_Off(void);
void vButtonCallback_Toggle(void);


/// @brief      【单行文本】数据结构
///
/// @note
typedef struct{
	
	uint32_t            x;          // 起始 X 坐标
    uint32_t            y;          // 起始 Y 坐标
    uint8_t             *ucData;    // 要显示的字符串
    emOledFontSizeTdf   emFontSize; // 字号大小

}stSingleLineTextTdf;



static uint8_t s_aucLed0[]     = "Led0";



/// @brief      【Led0】单行文本
///
/// @note
static stSingleLineTextTdf s_stSingleLineTest_Led0 = 
{
    .x          = 64 - (sizeof(s_aucLed0) / sizeof(s_aucLed0[0]) - 1) / 2 * emOledFontSize_8x16 / 2,
    .y          = 0,
    .ucData     = s_aucLed0,
    .emFontSize = emOledFontSize_8x16,
};


/// @brief      单行文本指针数组
///
/// @note       将三个结构体存储下来
static stSingleLineTextTdf *s_apstSingleLineText[] = 
{
    &s_stSingleLineTest_Led0,
};



/// @brief      【按钮回调函数】
///
/// @note
typedef void vfButtonCallbackTdf(void);


/// @brief      【按钮选中状态】
///
/// @note
typedef enum{
    emButtonSelectStatus_IsNotSelected      = 0,    // 未选中
    emButtonSelectStatus_IsSelected         = 1,    // 已选中
}emButtonSelectStatusTdf;

/// @brief      【按钮按下状态】
///
/// @note
typedef enum{
    emButtonPressStatus_IsNotPressed        = 0,    // 未按下
    emButtonPressStatus_IsPressed           = 1,    // 已按下
}emButtonPressStatusTdf;

/// @brief      【按钮触发状态】
///
/// @note
typedef enum
{
    emButtonTriggerStatus_IsNotTriggered    = 0,    // 未触发
    emButtonTriggerStatus_IsTriggered       = 1,    // 已触发
}emButtonTriggerStatusTdf;



/// @brief      【按钮】数据结构
///
/// @note
typedef struct{
    uint32_t                    x;                          // 起始 X 坐标
    uint32_t                    y;                          // 起始 Y 坐标
    uint8_t                     *ucData;                    // 要显示的字符串
    emOledFontSizeTdf           emFontSize;                 // 字号大小
    emButtonSelectStatusTdf     emSelectedStatus;           // 选中状态
    emButtonPressStatusTdf      emPressStatusCurrent;       // 当前按下状态
    emButtonPressStatusTdf      emPressStatusLast;          // 上次按下状态
    uint32_t                    ulPressedCount;             // 按下计数
    uint32_t                    ulPressedCountThreshold;    // 按下计数阈值
    vfButtonCallbackTdf         *vpfCallback;               // 回调函数指针
}stButtonTdf;



/// @brief      【On】按钮
///
/// @note
static uint8_t s_aucOn[]        = "On";

static stButtonTdf s_stButton_On = 
{
    .x                          = 20,
    .y                          = 1.2 * emOledFontSize_8x16,
    .ucData                     = s_aucOn,
    .emFontSize                 = emOledFontSize_8x16,
    .emSelectedStatus           = emButtonSelectStatus_IsSelected,
    .emPressStatusCurrent       = emButtonPressStatus_IsNotPressed,
    .emPressStatusLast          = emButtonPressStatus_IsNotPressed,
    .ulPressedCount             = 0,
    .ulPressedCountThreshold    = 5,
    .vpfCallback                = vButtonCallback_On,
};


/// @brief      【Off】按钮
///
/// @note
static uint8_t s_aucOff[]       = "Off";

static stButtonTdf s_stButton_Off = 
{
    .x                          = 80,
    .y                          = 1.2 * emOledFontSize_8x16,
    .ucData                     = s_aucOff,
    .emFontSize                 = emOledFontSize_8x16,
    .emSelectedStatus           = emButtonSelectStatus_IsNotSelected,
    .emPressStatusCurrent       = emButtonPressStatus_IsNotPressed,
    .emPressStatusLast          = emButtonPressStatus_IsNotPressed,
    .ulPressedCount             = 0,
    .ulPressedCountThreshold    = 5,
    .vpfCallback                = vButtonCallback_Off,
};


/// @brief      【Toggle】按钮
///
/// @note
static uint8_t s_aucToggle[]    = "Toggle";

static stButtonTdf s_stButton_Toggle = 
{
    .x                          = 20,
    .y                          = 2.5 * emOledFontSize_8x16,
    .ucData                     = s_aucToggle,
    .emFontSize                 = emOledFontSize_8x16,
    .emSelectedStatus           = emButtonSelectStatus_IsNotSelected,
    .emPressStatusCurrent       = emButtonPressStatus_IsNotPressed,
    .emPressStatusLast          = emButtonPressStatus_IsNotPressed,
    .ulPressedCount             = 0,
    .ulPressedCountThreshold    = 5,
    .vpfCallback                = vButtonCallback_Toggle,
};



/// @brief      按钮指针数组
///
/// @note
static stButtonTdf *s_apstButton[] = 
{
    &s_stButton_On,
    &s_stButton_Off,
    &s_stButton_Toggle,
};



/// @brief      显示所有单行文本
///
/// @note
void vShowSingleLineTextAll()
{
    uint32_t i;
    
    for(i = 0; i < sizeof(s_apstSingleLineText) / sizeof(s_apstSingleLineText[0]); i++)
    {
        vOledWriteStringToBuffer(s_apstSingleLineText[i]->x, 
                               s_apstSingleLineText[i]->y, 
                               s_apstSingleLineText[i]->ucData, 
                               s_apstSingleLineText[i]->emFontSize, 
                               emOledPixelShowMode_Positive, 
                               OLED);
    }
	
}



/// @brief      显示所有按钮
///
/// @note
void vShowButtonAll()
{
    uint32_t i;
    
    for(i = 0; i < sizeof(s_apstButton) / sizeof(s_apstButton[0]); i++)
    {
		// 1. 【未按下】的按钮正色显示，【已按下】的按钮反色显示
		if(s_apstButton[i]->emPressStatusCurrent == emButtonPressStatus_IsNotPressed)
		{
			vOledWriteStringToBuffer(s_apstButton[i]->x, 
                               s_apstButton[i]->y, 
                               s_apstButton[i]->ucData, 
                               s_apstButton[i]->emFontSize, 
                               emOledPixelShowMode_Positive, 
                               OLED);
		}
		else
		{
			vOledWriteStringToBuffer(s_apstButton[i]->x, 
                               s_apstButton[i]->y, 
                               s_apstButton[i]->ucData, 
                               s_apstButton[i]->emFontSize, 
                               emOledPixelShowMode_Negative, 
                               OLED);
		}
		
		// 2. 【已选中】的按钮添加顶端虚线
		if(s_apstButton[i]->emSelectedStatus == emButtonSelectStatus_IsSelected)
        {
            vOledDrawDotLine(s_apstButton[i]->x, 
                             s_apstButton[i]->y, 
                             strlen((char *)s_apstButton[i]->ucData) * s_apstButton[i]->emFontSize / 2, 
                             OLED);
        }
		
		// 3. 按钮添加底部虚线
        vOledDrawDotLine(s_apstButton[i]->x, 
                         s_apstButton[i]->y + s_apstButton[i]->emFontSize, 
                         strlen((char *)s_apstButton[i]->ucData) * s_apstButton[i]->emFontSize / 2, 
                         OLED);
	
	}
        
}



/// @brief      按钮触发状态扫描
///
/// @pstButton  按钮实例首地址
///
/// @retval     当前按钮的触发状态
///
/// @note
emButtonTriggerStatusTdf emButtonTriggerStatusScan(stButtonTdf *pstButton)
{
    emButtonTriggerStatusTdf emTriggerStatus = emButtonTriggerStatus_IsNotTriggered;
    
    // 1. 判断按钮是否被触发了
    if((pstButton->emPressStatusCurrent == emButtonPressStatus_IsPressed) && 
       (pstButton->emPressStatusLast == emButtonPressStatus_IsNotPressed))
    {
        pstButton->ulPressedCount = 0;
        
        emTriggerStatus = emButtonTriggerStatus_IsTriggered;
    }
    
    // 2. 更新按下保持计数，计数超过阈值则自动将按下状态复位
    pstButton->ulPressedCount++;
    if(pstButton->ulPressedCount > pstButton->ulPressedCountThreshold)
    {
        pstButton->ulPressedCount = 0;
        pstButton->emPressStatusCurrent = emButtonPressStatus_IsNotPressed;
    }
    
    // 3. 更新上一次的状态
    pstButton->emPressStatusLast = pstButton->emPressStatusCurrent;
    
    return emTriggerStatus;
}



/// @brief      【On】按钮回调
///
/// @note
void vButtonCallback_On()
{
    static stButtonTdf *s_pstButton = &s_stButton_On;
    
    if(emButtonTriggerStatusScan(s_pstButton) == emButtonTriggerStatus_IsTriggered)
    {
        vLedOn(LED_STATUS);
    }
}

/// @brief      【Off】按钮回调
///
/// @note
void vButtonCallback_Off()
{
    static stButtonTdf *s_pstButton = &s_stButton_Off;
    
    if(emButtonTriggerStatusScan(s_pstButton) == emButtonTriggerStatus_IsTriggered)
    {
        vLedOff(LED_STATUS);
    }
}

/// @brief      【Toggle】按钮回调
///
/// @note
void vButtonCallback_Toggle()
{
    static stButtonTdf *s_pstButton = &s_stButton_Toggle;
    
    if(emButtonTriggerStatusScan(s_pstButton) == emButtonTriggerStatus_IsTriggered)
    {
        vLedToggle(LED_STATUS);
    }
	
}



/// @brief      执行所有按钮的回调函数
///
/// @note
void vButtonCallbackExecuteAll()
{
    uint32_t i;
    
    for(i = 0; i < sizeof(s_apstButton) / sizeof(s_apstButton[0]); i++)
    {
        if(s_apstButton[i]->vpfCallback == 0)
        {
            continue;
        }
        
        s_apstButton[i]->vpfCallback();
    }
}



/// @brief      LED 初始化
///
/// @note
void vLedInit()
{
    stLedStaticParamTdf stInit;
    
    stInit.pstGpioBase   = GPIOC;
    stInit.usGpioPin     = GPIO_PIN_13;
    stInit.emOnLevel     = emLedOnLevel_Low;
    
    vLedDeviceInit(&stInit, LED_STATUS);
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


/// @brief      物理按键初始化
///
/// @note
void vPhyButtonInit()
{
    stBtnStaticParamTdf stInit;
    
    stInit.pstGpioBase                  = GPIOA;
    stInit.usGpioPin                    = GPIO_PIN_1;
    stInit.emActiveLevel                = emBtnActiveLevel_Low;
    stInit.ulDebounceCountThreshold     = 2;
    stInit.ulDoubleClickInterval        = 0;
    stInit.ulLongPressCountThreshold    = 1000000;
    
    emBtnDeviceInit(&stInit, BUTTON_UP);
    
    stInit.pstGpioBase                  = GPIOA;
    stInit.usGpioPin                    = GPIO_PIN_0;
    stInit.emActiveLevel                = emBtnActiveLevel_Low;
    stInit.ulDebounceCountThreshold     = 2;
    stInit.ulDoubleClickInterval        = 0;
    stInit.ulLongPressCountThreshold    = 1000000;
    
    emBtnDeviceInit(&stInit, BUTTON_DOWN);
    
    stInit.pstGpioBase                  = GPIOC;
    stInit.usGpioPin                    = GPIO_PIN_15;
    stInit.emActiveLevel                = emBtnActiveLevel_Low;
    stInit.ulDebounceCountThreshold     = 2;
    stInit.ulDoubleClickInterval        = 0;
    stInit.ulLongPressCountThreshold    = 1000000;
    
    emBtnDeviceInit(&stInit, BUTTON_ENTER);
    
    stInit.pstGpioBase                  = GPIOC;
    stInit.usGpioPin                    = GPIO_PIN_14;
    stInit.emActiveLevel                = emBtnActiveLevel_Low;
    stInit.ulDebounceCountThreshold     = 2;
    stInit.ulDoubleClickInterval        = 0;
    stInit.ulLongPressCountThreshold    = 1000000;
    
    emBtnDeviceInit(&stInit, BUTTON_CANCEL);
}



/// @brief      物理按键扫描
///
/// @note
void vPhyButtonExecuteAll()
{
    uint8_t i;
    static uint8_t s_ucSelectIndex = 0;

    // 1. 执行一次物理按键扫描
    for(i = 0; i < BTN_DEV_NUM; i++)
    {
        vBtnExecute((emBtnDevNumTdf)i);
    }

    // 2. 获取各个按键事件，并执行对应操作
    // 2.1. 【UP】键向下切换当前选中的按键
    if(emBtnGetEvent(BUTTON_UP) == emBtnEvent_Click)
    {
        vBtnClearEvent(BUTTON_UP);

        s_apstButton[s_ucSelectIndex]->emSelectedStatus = emButtonSelectStatus_IsNotSelected;

        s_ucSelectIndex--;
        if(s_ucSelectIndex > (sizeof(s_apstButton) / sizeof(s_apstButton[0]) - 1))
        {
            s_ucSelectIndex = (sizeof(s_apstButton) / sizeof(s_apstButton[0]) -1);
        }
        s_apstButton[s_ucSelectIndex]->emSelectedStatus = emButtonSelectStatus_IsSelected;
    }

    // 2.2. 【DOWN】键向上切换当前选中的按键
    if(emBtnGetEvent(BUTTON_DOWN) == emBtnEvent_Click)
    {
        vBtnClearEvent(BUTTON_DOWN);

        s_apstButton[s_ucSelectIndex]->emSelectedStatus = emButtonSelectStatus_IsNotSelected;

        s_ucSelectIndex++;
        if(s_ucSelectIndex > (sizeof(s_apstButton) / sizeof(s_apstButton[0]) - 1))
        {
            s_ucSelectIndex = 0;
        }
        s_apstButton[s_ucSelectIndex]->emSelectedStatus = emButtonSelectStatus_IsSelected;
    }

    // 2.3. 【ENTER】键将当前选中的按键改为按下状态
    if(emBtnGetEvent(BUTTON_ENTER) == emBtnEvent_Click)
    {
        vBtnClearEvent(BUTTON_ENTER);

        s_apstButton[s_ucSelectIndex]->emPressStatusCurrent = emButtonPressStatus_IsPressed;
    }
}


/// @brief      测试函数
///
/// @note
void vTest()
{
	vShowSingleLineTextAll();		//显示顶部标题
	vShowButtonAll();				//显示按钮选项
	vButtonCallbackExecuteAll();	//循环扫描按钮指针数组中成员的触发状态，调用回调函数
	vPhyButtonExecuteAll();
	
    vOledRefreshFromBuffer(OLED);
}


/// @brief      UI 任务
///
/// @note
void vUiTask()
{
	vOledInit();
	vLedInit();
	vPhyButtonInit();
	while(1)
	{
		vTest();
		
		vTaskDelay(1);
		
	}


}
