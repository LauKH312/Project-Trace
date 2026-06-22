#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stm32h7xx_hal.h>
#include <hal_signal_path.h>
#include "fixpoint.h"


#define BAREBONES_MODE 1 // currently does nothing

#define BOX_HEIGHT 11
#define BOX_WIDTH 63

extern int startpos[10][2];

typedef enum{
    page_main = 0,
    volt_menu_0,
    volt_menu_20,
    volt_menu_40,
    volt_menu_AC,
    volt_menu_Back,
    volt_menu_mode,
    page_main_fft,
    page_fft
} SelectedMenu;

typedef struct {
    bool ACCoupleEnabled;
    enum SignalPathAtten SelectedAtten;
    bool VoltmeterEnabled;
    SelectedMenu SelectedMenu;
} system_settings;


//initializes the menu setup. To call on system start
void menu_init(void);

//automatically calls when timer interrupts. Draws a new frame.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


//draws the frame depending on the settings.SelectedMenu variable.
void draw_frame(void);

//bundling all draw functions for main menu
void draw_main_menu(void);

//bundling all draw functions for settings menu
void draw_settings_menu(void);

//returns the newest buffer with the voltmeter values and draws it.
void draw_mean(fix9_23 *buffer, size_t len);

//draws a box displaying the currently selected ammeter or voltmeter mode. 
void draw_amp_volt_mode(void);

//Draws a box that displays if coupling is enabled. 
void draw_selected_atten(void);

//returns the newest buffer with the ammeter values and draws it. 
void draw_current(fix9_23 *mean, size_t len);

//draws the back button for returning from the options menu
void draw_back(void);

//draws the options for selecting 0dB, 20dB and 40dB attenuation
void draw_attens(void);

//draws the currently selected coupling enable/disable (true/false) 
void draw_selected_coupling(void);

//draws the box for the user to enter the options menu
void draw_options(void);

//Draws the box for selecting the FFT view
void draw_fft_box(void);

//draws the FFT screen on the display
void draw_fft(fix9_23 *buffer, size_t len);

//draws a square with the selected ammeter/voltmeter mode
void draw_selected_mode(void);

//statemachine controller for menu system
void left_push_button(void);

//statemachine controller for menu system
void right_push_button(void);
