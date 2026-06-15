/*
 * hal_signal_path.c
 *
 *  Created on: 27. apr. 2026
 *      Author: Marius H. Tyson
 */

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include <hal_signal_path.h>
#include <hal_gpio.h>

const GPIO_Pin_t ATTEN_0DB_PIN  = { .bank = GPIOA, .pin = GPIO_PIN_7 };
const GPIO_Pin_t ATTEN_20DB_PIN = { .bank = GPIOB, .pin = GPIO_PIN_4 };
const GPIO_Pin_t ATTEN_40DB_PIN = { .bank = GPIOC, .pin = GPIO_PIN_7 };
const GPIO_Pin_t GAIN_20DB_PIN  = { .bank = GPIOC, .pin = GPIO_PIN_0 };

// #include "stm32h7xx_hal.h"

void hal_signal_path_attenuator_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    GPIO_InitStruct.Pin = ATTEN_0DB_PIN.pin;
    HAL_GPIO_Init(ATTEN_0DB_PIN.bank, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ATTEN_20DB_PIN.pin;
    HAL_GPIO_Init(ATTEN_20DB_PIN.bank, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ATTEN_40DB_PIN.pin;
    HAL_GPIO_Init(ATTEN_40DB_PIN.bank, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GAIN_20DB_PIN.pin;
    HAL_GPIO_Init(ATTEN_40DB_PIN.bank, &GPIO_InitStruct);

    HAL_GPIO_WritePin(ATTEN_0DB_PIN.bank,  ATTEN_0DB_PIN.pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ATTEN_20DB_PIN.bank, ATTEN_20DB_PIN.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ATTEN_40DB_PIN.bank, ATTEN_40DB_PIN.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GAIN_20DB_PIN.bank, ATTEN_40DB_PIN.pin, GPIO_PIN_RESET);
}

void hal_signal_path_attenuator_set(enum SignalPathAtten range)
{
    HAL_GPIO_WritePin(ATTEN_0DB_PIN.bank,  ATTEN_0DB_PIN.pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ATTEN_20DB_PIN.bank, ATTEN_20DB_PIN.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ATTEN_40DB_PIN.bank, ATTEN_40DB_PIN.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GAIN_20DB_PIN.bank, ATTEN_40DB_PIN.pin, GPIO_PIN_RESET);
    
    switch (range){
        case SignalPathATTEN_0DB_PIN:
            HAL_GPIO_WritePin(ATTEN_0DB_PIN.bank,  ATTEN_0DB_PIN.pin,  GPIO_PIN_SET);
            break;
        case SignalPathAtten_p20db:
            HAL_GPIO_WritePin(ATTEN_20DB_PIN.bank, ATTEN_20DB_PIN.pin, GPIO_PIN_SET);
            break;
        case SignalPathAtten_p40db:
            HAL_GPIO_WritePin(ATTEN_40DB_PIN.bank, ATTEN_40DB_PIN.pin, GPIO_PIN_SET);
            break;
        case SignalPathAtten_m20db:
            HAL_GPIO_WritePin(GAIN_20DB_PIN.bank, GAIN_20DB_PIN.pin, GPIO_PIN_SET);
            break;
        default:
        	__builtin_unreachable();
            break;
    }
}

void hal_signal_path_gain_init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = GAIN_20DB_PIN.pin;
    HAL_GPIO_Init(GAIN_20DB_PIN.bank, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GAIN_20DB_PIN.bank,  GAIN_20DB_PIN.pin,  GPIO_PIN_RESET);
}

// void hal_signal_path_gain_set(enum SignalPathAtten range){
// 	HAL_GPIO_WritePin(GAIN_20DB_PIN.bank,  GAIN_20DB_PIN.pin,  GPIO_PIN_RESET);
// 	switch(range){
// 	case SignalPathATTEN_0DB_PIN: HAL_GPIO_WritePin(GAIN_20DB_PIN.bank,  GAIN_20DB_PIN.pin,  GPIO_PIN_RESET);  break;
// 	case SignalPathAtten_m20db:
// 		HAL_GPIO_WritePin(GAIN_20DB_PIN.bank,  GAIN_20DB_PIN.pin,  GPIO_PIN_SET); break;
// 	default:
// 		break;
// 	}
// }
