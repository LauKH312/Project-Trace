#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <ui.h>
#include <ui.h>

//#define DEBOUNCE_MS 10

//#define MIN_VALUE 0
//#define MAX_VALUE 100

//volatile uint32_t last_press_btn1 = 0;
//volatile uint32_t last_press_btn2 = 0;

void ui_init(void){
    // Enable GPIOD clock
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // PD3
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;   // Button to GND
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    // PD4
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    // EXTO setup
    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_3)
    {
        left_push_button();
    }

    if(GPIO_Pin == GPIO_PIN_4)
    {
        right_push_button();
    }
}

//--------------------------------------------------------------

// HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

// int16_t encoderPos;

// encoderPos = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
// // or
// encoderPos = (int16_t)TIM2->CNT;


// HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);

// void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
// {
//     if (htim->Instance == TIM2)
//     {
//         int32_t count = __HAL_TIM_GET_COUNTER(htim);
//         //code
//     }
// }

// int16_t pos_rot_1 = TIM2->CNT;
// int16_t pos_rot_2 = TIM3->CNT;