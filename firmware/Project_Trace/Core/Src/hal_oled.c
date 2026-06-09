/*
 * hal_oled.c
 *
 *  Created on: 5 May 2026
 *      Author: Marius Tyson
 */
#include <hal_oled.h>


const GPIO_Pin_t hal_oled_CS  = { .bank = GPIOE, .pin = GPIO_PIN_5 };
const GPIO_Pin_t hal_oled_DC  = { .bank = GPIOE, .pin = GPIO_PIN_6 };
const GPIO_Pin_t hal_oled_RES  = { .bank = GPIOE, .pin = GPIO_PIN_3 };

uint8_t framebuffer[OLED_PAGES][OLED_WIDTH];

extern SPI_HandleTypeDef hspi1;

void hal_oled_drawpixel(int x, int y, enum HalOledDraw OnOff)
{
    switch(OnOff){
    case HalOledDrawOn : framebuffer[y / 8][x] |= (1 << (y % 8)); break;
    case HalOledDrawOff :  framebuffer[y / 8][x] &= ~(1 << (y % 8)); break;
    default:
    	__builtin_unreachable(); break;
    }
}

void hal_oled_send_command(uint8_t cmd){
	HAL_OLED_CS_LOW();   // Command mode
	HAL_OLED_DC_LOW();
	HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
	HAL_OLED_CS_HIGH();
}

void hal_oled_send_data(uint8_t *data, uint16_t len)
{
	HAL_OLED_CS_LOW();   // enable, start transmission
	HAL_OLED_DC_HIGH();  // data mode
	HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
	HAL_OLED_CS_HIGH();  // return to idle
}

void hal_oled_init()
{
    HAL_OLED_RES_LOW();
    HAL_Delay(10);  // arbitrary subject to change
    HAL_OLED_RES_HIGH();
    HAL_Delay(100);  // arbitrary subject to change


    hal_oled_send_command(0xAE); // Display OFF
    hal_oled_send_command(0xA1); // Segment re-map
    hal_oled_send_command(0xC8); // COM scan reversed
    hal_oled_send_command(0xA8); // Set multiplex ratio
    hal_oled_send_command(0x3F); // 1/64 duty
    hal_oled_send_command(0xD3); // Display offset
    hal_oled_send_command(0x00); // No offset
    hal_oled_send_command(0x40); // Display start line = 0
    hal_oled_send_command(0xAD); // DC-DC control
    hal_oled_send_command(0x8B); // DC-DC ON
    hal_oled_send_command(0x81); // Contrast
    hal_oled_send_command(0x7F); // Mid contrast
    hal_oled_send_command(0xA4); // Normal display
    hal_oled_send_command(0xA6); // Non-inverted
    hal_oled_send_command(0xD5); // Clock divide
    hal_oled_send_command(0x50); // Ratio
    hal_oled_send_command(0xD9); // Pre-charge
    hal_oled_send_command(0x22); // 2 DCLKs
    hal_oled_send_command(0xDB); // VCOM deselect
    hal_oled_send_command(0x35);
    hal_oled_send_command(0xAF); // Display ON

    HAL_Delay(100); // arbitrary subject to change

}

void hal_oled_clear(void)
{

    for (int page = 0; page < OLED_PAGES; page++)
    {
        for (int col = 0; col < OLED_WIDTH; col++)
        {
            framebuffer[page][col] = 0x00;
        }
    }

    hal_oled_update_screen();
}
void hal_oled_update_screen(void){

	    for (uint8_t page = 0; page < OLED_PAGES; page++)
	    {
	        // page address
	        hal_oled_send_command(0xB0 | page);

	        // Set column address
	        hal_oled_send_command(0x00);  // lower column nibble
	        hal_oled_send_command(0x10);  // upper column nibble

	        // Send page
	        for (uint8_t col = 0; col < OLED_WIDTH; col++)
	        {
	            uint8_t data = (uint8_t)framebuffer[page][col];
	            hal_oled_send_data(&data, 1);
	        }
	    }
}
