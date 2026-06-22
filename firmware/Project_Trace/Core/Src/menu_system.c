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

#define FFT_SIZE 256
#define FFT_MAX_DB 20
#define FFT_MIN_DB -60

extern TIM_HandleTypeDef htim1;

extern SampleBuffer sample_buffer;

int startpos[10][2] = { 
    {0,0},  {63,0},
    {0,11}, {63,11},
    {0,22}, {63,22},
    {0,33}, {63,33},
    {0,44}, {63,44} 
};

system_settings settings = {
    .ACCoupleEnabled = false,
    .SelectedAtten = SignalPathAtten_0db,
    .VoltmeterEnabled = true,
    .SelectedMenu = page_main// volt_menu_0
};

//static int framecounter = 0;

// call on startup
void menu_init(void){
    HAL_TIM_Base_Start_IT(&htim1);
}


// void draw_average(int x, int y, fix9_23* buf, size_t buf_len) {
// 	fix9_23 avg = meas_average(buf, buf_len);
// 	graphics_draw_fix9_23(x, y, avg);
// }

// void drawframe(fix9_23 *buffer, size_t len){
// 	assert(len == FFT_SIZE);

// 	Complex9_23 to_fft[FFT_SIZE];
// 	Complex9_23 fft_out[FFT_SIZE] = { 0 };
// 	for (int i = 0; i < FFT_SIZE; i++) {
// 		to_fft[i] = complex9_23_new(fix9_23_mul(buffer[i], fix9_23_blackman_harris(i, FFT_SIZE)), FIX9_23_ZERO);
// 		//to_fft[i] = complex9_23_new(buffer[i], FIX9_23_ZERO);
// 	}
// 	fft_fft(to_fft, fft_out, FFT_SIZE);
//     plot_fft(fft_out, FFT_SIZE, NULL, FFT_MIN_DB, FFT_MAX_DB);
	//draw_average(2,2, buffer,len);

//	char dbrange_buf[32] = {0};
//	(void)snprintf(dbrange_buf, 32, "%d:%d", FFT_MIN_DB, FFT_MAX_DB);

//	graphics_draw_text(80, 2, dbrange_buf);


//	test_output_fix923_buffer(buffer, len, 0);
//}


void draw_frame(void){
    //graphics_draw_vertical_line(60, 3, 9); TODO add cursor
    
	switch (settings.SelectedMenu){
    case page_main:
        draw_main_menu();
        break;
    case volt_menu_0:
        draw_settings_menu();
        graphics_draw_vertical_line(60, 14, 20);
        break;
    case volt_menu_20:
        draw_settings_menu();
        graphics_draw_vertical_line(60, 22, 28);
        break;
    case volt_menu_40:
        draw_settings_menu();
        break;
    case volt_menu_AC:
        draw_settings_menu();
        break;
    case volt_menu_Back:
        draw_settings_menu();
        break;
    case volt_menu_mode:
        draw_settings_menu();
        break;
    case page_main_fft:
        draw_main_menu();
        break;
    case page_fft:
        // draw_fft();
        break;
    default:
        break;
    }

}

void draw_settings_menu(void){
    draw_back();
    draw_attens();
    draw_selected_coupling();
    draw_selected_mode();
}

void draw_main_menu(void){
    //int SAMPLE_BUFFER_LEN = 16; TODO fix?
    
    fix9_23 current_samples[SAMPLE_BUFFER_LEN];
    // sample_peek_current(sample_buffer, amp_samples, SAMPLE_BUFFER_LEN);

    fix9_23 mean_samples[SAMPLE_BUFFER_LEN];
    // sample_peek_mean(sample_buffer, mean_samples, SAMPLE_BUFFER_LEN);

    draw_mean(mean_samples, SAMPLE_BUFFER_LEN);
    draw_current(current_samples, SAMPLE_BUFFER_LEN);
    draw_selected_atten();
    draw_amp_volt_mode();
    draw_selected_coupling();
    draw_options();
    draw_fft_box();
    draw_selected_mode();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        draw_frame();
        //printf("Update Frame\n");
        hal_oled_update_screen();
    }
}



void draw_mean(fix9_23 *avg, size_t len){
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


    graphics_draw_rect_rel(startpos[6][0], startpos[6][1], BOX_WIDTH, BOX_HEIGHT);
    graphics_draw_text(startpos[6][0] + 1, startpos[6][1] + 1, fmtbuf);
}


void draw_amp_volt_mode(void){
    graphics_draw_rect_rel(startpos[0][0], startpos[0][1], BOX_WIDTH, BOX_HEIGHT);
    if (settings.VoltmeterEnabled)
    {
        graphics_draw_text(startpos[0][0]+1, startpos[0][1]+1, "VoltAct");
    } else 
    {
        graphics_draw_text(startpos[0][0]+1, startpos[0][1]+1, "AmpAct");
    };
}


void draw_selected_atten(void){
    graphics_draw_rect_rel(startpos[3][0], startpos[3][1], BOX_WIDTH, BOX_HEIGHT);
    switch (settings.SelectedAtten){
        case SignalPathAtten_0db:
            graphics_draw_text(startpos[3][0]+1, startpos[3][1]+1, "Sel0dB");
            break;
        case SignalPathAtten_20db:
            graphics_draw_text(startpos[3][0]+1, startpos[3][1]+1, "Sel-20dB");
            break;
        case SignalPathAtten_40db:
            graphics_draw_text(startpos[3][0]+1, startpos[3][1]+1, "Sel-40dB");
            break;
        default:
            break;
    };
}

void draw_current(fix9_23 *mean, size_t len){
    if (!mean || len == 0) return;

    char fmtbuf[16] = {0};

    // Safety
    if (len > sizeof(fmtbuf)) {
        len = sizeof(fmtbuf);
    }

    int fmtlen = fix9_23_format(*mean, fmtbuf, len);
    if (fmtlen <= 0){
        return;
    }


    graphics_draw_rect_rel(startpos[8][0], startpos[8][1], BOX_WIDTH, BOX_HEIGHT);
    graphics_draw_text(startpos[8][0] + 1, startpos[8][1] + 1, fmtbuf);
}


void draw_back(void){
    graphics_draw_rect_rel(startpos[9][0], startpos[9][1], BOX_WIDTH, BOX_HEIGHT);
    graphics_draw_text(startpos[9][0] + 1, startpos[9][1] + 1, "Back");
}

void draw_attens(void){
    graphics_draw_rect_rel(startpos[2][0], startpos[2][1], BOX_WIDTH, BOX_HEIGHT);
    graphics_draw_text(startpos[2][0] + 1, startpos[2][1] + 1, "0dBAt");
    graphics_draw_rect_rel(startpos[4][0], startpos[4][1], BOX_WIDTH, BOX_HEIGHT);
    graphics_draw_text(startpos[4][0] + 1, startpos[4][1] + 1, "20dBAt");
    graphics_draw_rect_rel(startpos[6][0], startpos[6][1], BOX_WIDTH, BOX_HEIGHT);
    graphics_draw_text(startpos[6][0] + 1, startpos[6][1] + 1, "40dBAt");
}

void draw_selected_coupling(void){
    if (settings.ACCoupleEnabled)
    {
        graphics_draw_rect_rel(startpos[1][0], startpos[1][1], BOX_WIDTH, BOX_HEIGHT);
        graphics_draw_text(startpos[1][0] + 1, startpos[1][1] + 1, "Cou En");
    } else
    {
        graphics_draw_rect_rel(startpos[1][0], startpos[1][1], BOX_WIDTH, BOX_HEIGHT);
        graphics_draw_text(startpos[1][0] + 1, startpos[1][1] + 1, "Cou Dis");
    }
}

void draw_options(void){
    graphics_draw_rect_rel(startpos[9][0], startpos[9][1], BOX_WIDTH, BOX_HEIGHT);
    graphics_draw_text(startpos[9][0] + 1, startpos[9][1] + 1, "Sett");
}

void draw_fft_box(void){
    graphics_draw_rect_rel(startpos[5][0], startpos[5][1], BOX_WIDTH, BOX_HEIGHT);
    graphics_draw_text(startpos[5][0] + 1, startpos[5][1] + 1, "Sett");
}

void draw_fft(fix9_23 *buffer, size_t len){
	assert(len == FFT_SIZE);

	Complex9_23 to_fft[FFT_SIZE];
	Complex9_23 fft_out[FFT_SIZE] = { 0 };
	for (int i = 0; i < FFT_SIZE; i++) {
		to_fft[i] = complex9_23_new(fix9_23_mul(buffer[i], fix9_23_blackman_harris(i, FFT_SIZE)), FIX9_23_ZERO);
		//to_fft[i] = complex9_23_new(buffer[i], FIX9_23_ZERO);
	}
	fft_fft(to_fft, fft_out, FFT_SIZE);
    plot_fft(fft_out, FFT_SIZE, NULL, FFT_MIN_DB, FFT_MAX_DB);
}

void draw_selected_mode(void){
    if (settings.VoltmeterEnabled)
    {
        graphics_draw_rect_rel(startpos[7][0], startpos[7][1], BOX_WIDTH, BOX_HEIGHT);
        graphics_draw_text(startpos[7][0] + 1, startpos[7][1] + 1, "V En");
    } else
    {
        graphics_draw_rect_rel(startpos[7][0], startpos[7][1], BOX_WIDTH, BOX_HEIGHT);
        graphics_draw_text(startpos[7][0] + 1, startpos[7][1] + 1, "A En");
    }
}

//_________________________________________________________________________________________________________________________________ done




//to call when left button is pushed
void left_push_button(void){
    switch (settings.SelectedMenu){
    case page_main:
        settings.SelectedMenu = volt_menu_0;
        break;
    case volt_menu_0:
        settings.SelectedMenu = volt_menu_20;
        break;
    case volt_menu_20:
        settings.SelectedMenu = volt_menu_40;
        break;
    case volt_menu_40:
        settings.SelectedMenu = volt_menu_AC;
        break;
    case volt_menu_AC:
        settings.SelectedMenu = volt_menu_Back;
        break;
    case volt_menu_Back:
        settings.SelectedMenu = page_main_fft;
        break;
    case page_main_fft:
        settings.SelectedMenu = page_main;
        break;  
    case page_fft:
        settings.SelectedMenu = page_main;
        break;
    default: 
        break;
    }
}

//to call when right button is pushed
void right_push_button(void){
    switch (settings.SelectedMenu){
    case page_main:
        settings.SelectedMenu = volt_menu_Back;
        break;
    case volt_menu_0:
        hal_signal_path_attenuator_set(SignalPathAtten_0db);
        settings.SelectedAtten = SignalPathAtten_0db;
        break;
    case volt_menu_20:
        hal_signal_path_attenuator_set(SignalPathAtten_20db);
        settings.SelectedAtten = SignalPathAtten_20db;
        break;
    case volt_menu_40:
        hal_signal_path_attenuator_set(SignalPathAtten_40db);
        settings.SelectedAtten = SignalPathAtten_40db;
        break;
    case volt_menu_AC:
        if (settings.ACCoupleEnabled)
        {
            settings.ACCoupleEnabled = false;
        } else 
        {
            settings.ACCoupleEnabled = true;
        }
        break;
    case volt_menu_Back:
        settings.SelectedMenu = page_main;
        break;
    case volt_menu_mode:
        if (settings.VoltmeterEnabled)
        {
            settings.VoltmeterEnabled = false;
        }else 
        {
            settings.VoltmeterEnabled = true;
        }
        break;
    case page_main_fft:
        settings.SelectedMenu = page_fft;
        break;
    case page_fft:
        settings.SelectedMenu = page_main;
        break;
    default:
        break;
    }
}