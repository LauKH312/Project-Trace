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

#include "stm32h7xx_hal.h"

void hal_signal_path_attenuator_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = ATTEN_0DB.pin;
    HAL_GPIO_Init(ATTEN_0DB.bank, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ATTEN_20DB.pin;
    HAL_GPIO_Init(ATTEN_20DB.bank, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ATTEN_40DB.pin;
    HAL_GPIO_Init(ATTEN_40DB.bank, &GPIO_InitStruct);

    HAL_GPIO_WritePin(ATTEN_0DB.bank,  ATTEN_0DB.pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ATTEN_20DB.bank, ATTEN_20DB.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ATTEN_40DB.bank, ATTEN_40DB.pin, GPIO_PIN_RESET);
}

void hal_signal_path_attenuator_set(enum SignalPathAtten range)
{
    HAL_GPIO_WritePin(ATTEN_0DB.bank,  ATTEN_0DB.pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ATTEN_20DB.bank, ATTEN_20DB.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ATTEN_40DB.bank, ATTEN_40DB.pin, GPIO_PIN_RESET);

    switch (range){
        case SignalPathAtten_0db:
            HAL_GPIO_WritePin(ATTEN_0DB.bank,  ATTEN_0DB.pin,  GPIO_PIN_SET);
            break;
        case SignalPathAtten_20db:
            HAL_GPIO_WritePin(ATTEN_20DB.bank, ATTEN_20DB.pin, GPIO_PIN_SET);
            break;
        case SignalPathAtten_40db:
            HAL_GPIO_WritePin(ATTEN_40DB.bank, ATTEN_40DB.pin, GPIO_PIN_SET);
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
    GPIO_InitStruct.Pin = GAIN_20DB.pin;
    HAL_GPIO_Init(GAIN_20DB.bank, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GAIN_20DB.bank,  GAIN_20DB.pin,  GPIO_PIN_RESET);
}

void hal_signal_path_gain_set(enum SignalPathGain range){
	HAL_GPIO_WritePin(GAIN_20DB.bank,  GAIN_20DB.pin,  GPIO_PIN_RESET);
	switch(range){
	case SignalPathGain_0db: HAL_GPIO_WritePin(GAIN_20DB.bank,  GAIN_20DB.pin,  GPIO_PIN_RESET);  break;
	case SignalPathGain_20db:
		HAL_GPIO_WritePin(GAIN_20DB.bank,  GAIN_20DB.pin,  GPIO_PIN_SET); break;
	default:
		break;
	}
}
