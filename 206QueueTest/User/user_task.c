#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "stdio.h"
#include "user_task.h"
#include "freertos.h"
#include "task.h"
#include "cmsis_os2.h"
#include "queue.h"

extern UART_HandleTypeDef huart1;
extern osMessageQueueId_t ButtonQueueHandle;
extern osMessageQueueId_t TimerQueueHandle;
extern osMessageQueueId_t PrintQueueHandle;

static const uint8_t sc_aucButtonString_Up[]        = "按键【上】被按下\n\r";
static const uint8_t sc_aucButtonString_Down[]      = "按键【下】被按下\n\r";
static const uint8_t sc_aucButtonString_Enter[]     = "按键【确定】被按下\n\r";
static const uint8_t sc_aucButtonString_Cancel[]    = "按键【退出】被按下\n\r";

/// @brief      按键扫描
///
/// @note       每 20ms 获取一次【键值】，如果【键值】有变化，则将【键值】写入【按键队列】
void vButtonScan()
{
    uint8_t ucButtonValueCurrent;               // 当前键值
    static uint8_t s_ucButtonValueLast;         // 上次键值
    const uint8_t *c_pucSendString;
    
    // 1. 每次执行任务前，先将当前键值清零
    ucButtonValueCurrent = 0;
    
    // 2. 获取当前键值
    // 2.1. 按键 1 被按下，则将键值 bit0 设置为【1】
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 0)
    {
        ucButtonValueCurrent |= 0X01;
		
		// 上次按键没有按下，当前按键已经按下，则字符串入队
        if((s_ucButtonValueLast & 0X01) == 0)
        {
            c_pucSendString = sc_aucButtonString_Up;
            xQueueSend(PrintQueueHandle, &c_pucSendString, 0);
        }
        
    }
    // 2.2. 按键 2 被按下，则将键值 bit1 设置为【1】
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
    {
        ucButtonValueCurrent |= 0X02;
		
		// 上次按键没有按下，当前按键已经按下，则字符串入队
        if((s_ucButtonValueLast & 0X02) == 0)
        {
            c_pucSendString = sc_aucButtonString_Up;
            xQueueSend(PrintQueueHandle, &c_pucSendString, 0);
        }
        
    }
    // 2.3. 按键 3 被按下，则将键值 bit2 设置为【1】
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0)
    {
        ucButtonValueCurrent |= 0X04;
		
		// 上次按键没有按下，当前按键已经按下，则字符串入队
        if((s_ucButtonValueLast & 0X04) == 0)
        {
            c_pucSendString = sc_aucButtonString_Up;
            xQueueSend(PrintQueueHandle, &c_pucSendString, 0);
        }
        
    }
    // 2.4. 按键 4 被按下，则将键值 bit3 设置为【1】
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0)
    {
        ucButtonValueCurrent |= 0X08;
		
		// 上次按键没有按下，当前按键已经按下，则字符串入队
        if((s_ucButtonValueLast & 0X08) == 0)
        {
            c_pucSendString = sc_aucButtonString_Up;
            xQueueSend(PrintQueueHandle, &c_pucSendString, 0);
        }

    }
    
    // 3. 如果当前键值与上次键值不同，则将【当前键值】发送到【按键队列】中
    if(ucButtonValueCurrent != s_ucButtonValueLast)
    {
        xQueueSend(ButtonQueueHandle, &ucButtonValueCurrent, 0);
    }
    
    // 4. 更新上次键值
    s_ucButtonValueLast = ucButtonValueCurrent;
    
    // 5. 切换任务并延时 20 ms
    vTaskDelay(20);
}


/// @brief      LED 执行
///
/// @note       根据键值，点亮不同 LED 灯
void vLedExecute()
{
    uint8_t ucButtonValue;
	uint16_t ucTimerValue;
    
    // 1. 接收键值，不阻塞
    if(xQueueReceive(ButtonQueueHandle, &ucButtonValue, 0))
	{
        // 1.1. 根据键值，点亮和熄灭 LED
        // 1.1.1. LED 1 操作
        if((ucButtonValue & 0x01) != 0)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
        }
        // 1.1.2. LED 2 操作
        if((ucButtonValue & 0x02) != 0)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        }
        // 1.1.3. LED 3 操作
        if((ucButtonValue & 0x04) != 0)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
        }
        // 1.1.4. LED 4 操作
        if((ucButtonValue & 0x08) != 0)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
        }
	}
	 // 2. 接收定时器值，不阻塞
    if(xQueueReceive(TimerQueueHandle, &ucTimerValue, 0))
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }
    
    // 3. 切换任务并延时 10ms
    vTaskDelay(10);
    
}

/// @brief      打印按键字符串
///
/// @note       
void vPrintButtonString()
{
    uint8_t *p;
    
    // 1. 接收按键字符串，收不到就一直阻塞
    xQueueReceive(PrintQueueHandle, &p, portMAX_DELAY);
    
    // 2. 打印字符串
    printf("%s", p);
    
    // 3. 延时
    vTaskDelay(1000);
}


/// @brief      重定向 fputc，printf 打印的内容会从 UART1 发出
///
/// @note       
int fputc(int ch, FILE *f)
{
//    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 10);
    
    while((huart1.Instance->SR & USART_SR_TXE) == 0)
    {
        ;
    }
    
    huart1.Instance->DR = *(uint8_t*)&ch;

    return ch;
}
