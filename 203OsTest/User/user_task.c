#include "stm32f1xx_hal.h"                 // Device header
#include "stdint.h"
#include "stdio.h"
#include "user_task.h"
#include "freertos.h"
#include "task.h"

extern UART_HandleTypeDef huart1;


void vButtonTask()
{
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
    {
        HAL_Delay(20);
        if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
        {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        }
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    }
}


void vUartTask()
{
	uint32_t ulBeginTime;
	
	ulBeginTime = HAL_GetTick();
	printf("uart time:%d \n\r", HAL_GetTick());
	vTaskDelayUntil(&ulBeginTime, 1000);

}

void vAdcTask()
{
	uint32_t ulBeginTime;
	
	ulBeginTime = HAL_GetTick();
	printf("adc begin time:%d \n\r", HAL_GetTick());
	HAL_Delay(200);
    printf("adc end time:%d \n\r", HAL_GetTick());
	vTaskDelayUntil(&ulBeginTime, 500);

}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit (&huart1, (uint8_t*)&ch, 1, 10);
	
	return ch;
}
