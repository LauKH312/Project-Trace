#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stm32h7xx_hal.h>

#include "fixpoint.h"


#define BAREBONES_MODE 1 // currently does nothing

#define BOX_HEIGHT 12
#define BOX_WIDTH 64

typedef enum
{
    page_main = 0,
    volt_menu_p20,
    volt_menu_0,
    volt_menu_m20,
    volt_menu_m40,
    volt_menu_AC,
    volt_menu_Back,
} MenuPage;



//initializes the menu setup. To call on system start
void menu_init(void);
void menu_system_frame_update(void);
//automatically calls when timer interrupts. Draws a new frame.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


//currently only draws the average value currently measured. WIP to include more stuff for the display
void drawframe(const fix9_23 *avg_buffer, size_t avg_len);

void draw_avg(fix9_23 *buffer, size_t len);

//statemachine controller for menu system
//void left_push_button(void);

//statemachine controller for menu system
//void right_push_button(void);
