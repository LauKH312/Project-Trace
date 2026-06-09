/*
 * hal_oled1.h
 *
 *  Created on: 5 May 2026
 *      Author: Marius Tyson
 */

#ifndef INC_HAL_OLED_H_
#define INC_HAL_OLED_H_
#include <main.h>
#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include <stdint.h>
#include <string.h>
#include <hal_gpio.h>



#define OLED_WIDTH     128
#define OLED_HEIGHT    64
#define OLED_PAGES     8


//const GPIO_Pin_t hal_oled_CS  = { .bank = GPIOE, .pin = GPIO_PIN_5 };
//const GPIO_Pin_t hal_oled_DC  = { .bank = GPIOE, .pin = GPIO_PIN_6 };
//const GPIO_Pin_t hal_oled_RES  = { .bank = GPIOE, .pin = GPIO_PIN_3 };

enum HalOledDraw {
	HalOledDrawOff =  0,
	HalOledDrawOn,
};

void hal_oled_clear(void);
void hal_oled_update_screen(void);
void hal_oled_init();
void hal_oled_send_data(uint8_t *data, uint16_t len);
void hal_oled_send_command(uint8_t cmd);
void hal_oled_drawpixel(int x, int y, enum HalOledDraw OnOff);

#define HAL_OLED_CS_LOW() HAL_GPIO_WritePin(hal_oled_CS.bank,  hal_oled_CS.pin,  GPIO_PIN_RESET);
#define HAL_OLED_CS_HIGH() HAL_GPIO_WritePin(hal_oled_CS.bank,  hal_oled_CS.pin,  GPIO_PIN_SET);

#define HAL_OLED_DC_HIGH() HAL_GPIO_WritePin(hal_oled_DC.bank,  hal_oled_DC.pin,  GPIO_PIN_SET);
#define HAL_OLED_DC_LOW() HAL_GPIO_WritePin(hal_oled_DC.bank,  hal_oled_DC.pin,  GPIO_PIN_RESET);

#define HAL_OLED_RES_LOW() HAL_GPIO_WritePin(hal_oled_RES.bank,  hal_oled_RES.pin,  GPIO_PIN_RESET);
#define HAL_OLED_RES_HIGH() HAL_GPIO_WritePin(hal_oled_RES.bank,  hal_oled_RES.pin,  GPIO_PIN_SET);

void hal_oled_update_screen(void);

#endif
