#include "stm32f1xx_hal.h"                 // Device header
#include "stdint.h"
#include "stdio.h"
#include "user_task.h"
#include "freertos.h"
#include "task.h"

extern UART_HandleTypeDef huart1;


void vTask1()
{
	uint32_t ulTaskCount;
	
	ulTaskCount = HAL_GetTick();
	printf("abcdef");
	portYIELD();      // 告诉调度器切换任务

    vTaskDelayUntil(&ulTaskCount, 1);
}

void vTask2()
{
	printf("ABCDEFGHIJKLMNOPQ");
	vTaskDelay(1);
	
}

void vTask3()
{
	printf("+");
}


int fputc(int ch, FILE *f)
{
//	HAL_UART_Transmit (&huart1, (uint8_t*)&ch, 1, 10);
	
    
    while((huart1.Instance->SR & USART_SR_TXE) == 0)
    {
        ;
    }
    
    huart1.Instance->DR = *(uint8_t*)&ch;
	
	return ch;
}
