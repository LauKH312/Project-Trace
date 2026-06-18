// #include <stdlib.h>
#include <fixpoint.h>
//#include <stm32h7xx_it.h>
#include <menu_system.h>
#include <sample_buffer.h>
#include <stm32h7xx_hal.h>
#include <menu_system.h>
#include <graphics.h>
#include <meas.h>
#include <hal_signal_path.h>

#include <stdio.h>

#include <hal_oled.h>

extern TIM_HandleTypeDef htim1;

extern SampleBuffer sample_buffer;

static MenuPage menu = page_main;

static int framecounter = 0;

// call on startup
void menu_init(void){
    HAL_TIM_Base_Start_IT(&htim1);
}

void drawframe(fix9_23 *avg_buffer, size_t avg_len){

	draw_avg(avg_buffer, avg_len);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        fix9_23 test_buf = fix9_23_int(123);
        drawframe(&test_buf, 5);
        printf("Update Frame\n");

        char fmtbuf[64] = {0};
        int ln = snprintf(fmtbuf, 64, "%d", framecounter);
        fmtbuf[ln] = '\0';
        framecounter++;

        graphics_draw_text(16, 16, fmtbuf);

        hal_oled_update_screen();
    }
}


// void drawframe(void){
//     if (BAREBONES_MODE == true)
//     {
//     switch (MenuPage){
//     case Main:
//         code
//         break;
//     case volt_menu_p20:
//         code
//         break;
//     case volt_menu_0:
//         code
//         break;
//     case volt_menu_m20:
//         code
//         break;
//     case volt_menu_m40:
//         code
//         break;
//     case volt_menu_AC:
//         code
//         break;
//     case volt_menu_Back:
//         code
//         break;
//     };
//     }
//     else {
//     return
//     }
// }

//void draw_atten

// void draw_amplitude(int amplitude){
//     void graphics_draw_rect_rel(startpos[2][0], startpos[2][1], BOX_WIDTH, BOX_HEIGHT);
//     void graphics_draw_text(startpos[2][0]+1, startpos[2][1]+1, amp);
// }

// void draw_freq(int freq){
//     void graphics_draw_rect_rel(startpos[4][0], startpos[4][1], BOX_WIDTH, BOX_HEIGHT);
//     void graphics_draw_text(startpos[4][0]+1, startpos[4][1]+1, freq);
// }

void draw_avg(fix9_23 *avg, size_t len){
    if (!avg || len == 0) return;

    char fmtbuf[16] = {0};

    // Safety
    if (len > sizeof(fmtbuf)) {
        len = sizeof(fmtbuf);
    }

    int fmtlen = fix9_23_format(*avg, fmtbuf, len);
    if (fmtlen <= 0){
        return;
    }
    int startpos[10][2] = { 
        {0,0},  {64,0},
        {0,12}, {64,12},
        {0,24}, {64,24},
        {0,36}, {64,36},
        {0,48}, {64,48} 
    };

    graphics_draw_rect_rel(startpos[6][0], startpos[6][1], BOX_WIDTH, BOX_HEIGHT);
    graphics_draw_text(startpos[6][0] + 1, startpos[6][1] + 1, fmtbuf);
}

// void draw_dc_avg(int dc_avg){
//     void graphics_draw_rect_rel(startpos[0][0], startpos[0][1], BOX_WIDTH, BOX_HEIGHT);
//     void graphics_draw_text(startpos[0][0]+1, startpos[0][1]+1, dc_avg);
// }

// void draw_amp_volt_mode(void){
//     void graphics_draw_rect_rel(startpos[1][0], startpos[1][1], BOX_WIDTH, BOX_HEIGHT)
//     switch (mode){
//         case 0:
//             void graphics_draw_text(startpos[1][0]+1, startpos[1][1]+1, "Selected Amp")
//             break;
//         case 1:
//             void graphics_draw_text(startpos[1][0]+1, startpos[1][1]+1, "Selected Volt")
//             break;
//     };
// }

// void draw_selected_atten(void){
//     void graphics_draw_rect_rel(startpos[3][0], startpos[3][1], BOX_WIDTH, BOX_HEIGHT)
//     switch (selected_atten){
//         case SignalPathAtten_m20db:
//             void graphics_draw_text(startpos[3][0]+1, startpos[3][1]+1, "Selected +20dB")
//             break;
//         case SignalPathAtten_0db:
//             void graphics_draw_text(startpos[3][0]+1, startpos[3][1]+1, "Selected 0dB")
//             break;
//         case SignalPathAtten_p20db:
//             void graphics_draw_text(startpos[3][0]+1, startpos[3][1]+1, "Selected -20dB")
//             break;
//         case SignalPathAtten_p40db:
//             void graphics_draw_text(startpos[3][0]+1, startpos[3][1]+1, "Selected -40dB")
//             break;
//     };
// }

// void draw_current(void){
//     void graphics_draw_rect_rel(startpos[9][0], startpos[9][1], BOX_WIDTH, BOX_HEIGHT)
//     void graphics_draw_text(startpos[9][0]+1, startpos[9][1]+1, char* text)
// }




//_________________________________________________________________________________________________________________________________ done




//to call when left button is pushed
void left_push_button(void){
    switch (menu){
    case page_main:
        // mode = mode * -1;
        break;
    case volt_menu_p20:
        menu = volt_menu_0;
        break;
    case volt_menu_0:
        menu = volt_menu_m20;
        break;
    case volt_menu_m20:
        menu = volt_menu_m40;
        break;
    case volt_menu_m40:
        menu = volt_menu_AC;
        break;
    case volt_menu_AC:
        menu = volt_menu_Back;
        break;
    case volt_menu_Back:
        menu = volt_menu_p20;
        break;
    default: break;
    }
}

//to call when right button is pushed
void right_push_button(void){
    switch (menu){
    case page_main:
        menu = volt_menu_Back;
        break;
    case volt_menu_p20:
        hal_signal_path_attenuator_set(SignalPathAtten_m20db);
        break;
    case volt_menu_0:
        hal_signal_path_attenuator_set(SignalPathAtten_0db);
        break;
    case volt_menu_m20:
        hal_signal_path_attenuator_set(SignalPathAtten_p20db);
        break;
    case volt_menu_m40:
        hal_signal_path_attenuator_set(SignalPathAtten_p40db);
        break;
    case volt_menu_AC:
        break;
    case volt_menu_Back:
        menu = page_main;
        break;
    default:
        break;
    }
}
