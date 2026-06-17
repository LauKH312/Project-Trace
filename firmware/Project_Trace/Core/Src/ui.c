#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#define DEBOUNCE_MS 10

#define MIN_VALUE 0
#define MAX_VALUE 100

volatile uint32_t last_press_btn1 = 0;
volatile uint32_t last_press_btn2 = 0;


// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
//     uint32_t now = HAL_GetTick();

//     if (GPIO_Pin == GPIO_PIN_13)
//     {
//         if ((now - last_press_btn1) > DEBOUNCE_MS)
//         {
//             last_press_btn1 = now;

//             left_push_button();
//         }
//     }

//     if (GPIO_Pin == GPIO_PIN_0)
//     {
//         if ((now - last_press_btn2) > DEBOUNCE_MS)
//         {
//             last_press_btn2 = now;

//             right_push_button();
//         }
//     }
// }

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