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

#include <serialization.h>

#include <test_suite.h>

#include <plotting.h>
#include <fft.h>

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

#define FFT_SIZE 1024
#define FFT_MAX_DB 20
#define FFT_MIN_DB -60

void draw_average(int x, int y, const fix9_23* buf, size_t buf_len) {
	fix9_23 avg = meas_average(buf, buf_len);
	graphics_draw_fix9_23(x, y, avg);
}

void drawframe(const fix9_23 *buffer, size_t len){
	// assert(len == FFT_SIZE);

	Complex9_23 to_fft[FFT_SIZE];
	Complex9_23 fft_out[FFT_SIZE] = { 0 };
	for (int i = 0; i < FFT_SIZE; i++) {
		to_fft[i] = complex9_23_new(fix9_23_mul(buffer[i], fix9_23_blackman_harris(i, FFT_SIZE)), FIX9_23_ZERO);
		//to_fft[i] = complex9_23_new(buffer[i], FIX9_23_ZERO);
	}
	fft_fft(to_fft, fft_out, FFT_SIZE);
	draw_average(2,2, buffer,len);

	char dbrange_buf[32] = {0};
	(void)snprintf(dbrange_buf, 32, "%d:%d", FFT_MIN_DB, FFT_MAX_DB);

	graphics_draw_text(80, 2, dbrange_buf);

	plot_fft(fft_out, FFT_SIZE, NULL, FFT_MIN_DB, FFT_MAX_DB);

	test_output_fix923_buffer(buffer, len, 0);

}

void menu_system_frame_update(void) {
	graphics_clear();
    //int start_idx = (FFT_SIZE * framecounter) % SAMPLE_BUFFER_LEN;
    //if (start_idx + FFT_SIZE >= SAMPLE_BUFFER_LEN) start_idx = 0;
    int start_idx = 0;


	fix9_23 current_samplebuf[SAMPLE_BUFFER_LEN];
	sample_buffer_peek_samples(&sample_buffer, current_samplebuf, SAMPLE_BUFFER_LEN);
    drawframe(&current_samplebuf[start_idx], 256);

    graphics_draw_int(16, 16, framecounter);
    framecounter++;

    hal_oled_update_screen();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	printf("HAL_TIM_PERIOD_ELAPSED\n");
    if (htim->Instance == TIM1)
    {
    	printf("TIM_1\n");
    	__disable_irq();
    	graphics_clear();
        //int start_idx = (FFT_SIZE * framecounter) % SAMPLE_BUFFER_LEN;
        //if (start_idx + FFT_SIZE >= SAMPLE_BUFFER_LEN) start_idx = 0;
        int start_idx = 0;


    	fix9_23 current_samplebuf[SAMPLE_BUFFER_LEN];
    	sample_buffer_peek_samples(&sample_buffer, current_samplebuf, SAMPLE_BUFFER_LEN);
        drawframe(&current_samplebuf[start_idx], FFT_SIZE);

        graphics_draw_int(16, 16, framecounter);
        framecounter++;

        hal_oled_update_screen();
        __enable_irq();
        printf("TIM_1 finish\n");
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
        hal_signal_path_attenuator_set(SignalPathAtten_0db); // TODO: reintroduce plus 20dB
        break;
    case volt_menu_0:
        hal_signal_path_attenuator_set(SignalPathAtten_0db);
        break;
    case volt_menu_m20:
        hal_signal_path_attenuator_set(SignalPathAtten_20db);
        break;
    case volt_menu_m40:
        hal_signal_path_attenuator_set(SignalPathAtten_40db);
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
