/*
 * test_suite.c
 *
 *  Created on: Apr 27, 2026
 *      Author: Alexander & Marius Tyson & Laurits
 */

#include <math.h>
#include <time.h>

#include <test_suite.h>
#include <stdio.h>
#include <fixpoint.h>
#include <attounit.h>
#include <serialization.h>
#include <time.h>
#include <graphics.h>
#include <hal_oled.h>

#include <bitfont.h>
#include "complex.h"
#include "fft.h"
#include <plotting.h>

#include "core_cm7.h"

extern uint8_t framebuffer[OLED_PAGES][OLED_WIDTH];

#define BUFSZ 64

TEST_MAIN()
void test_suite(void) {
    int code = attomain();
    printf("Attotest exited with code %d\n", code);



    // TEST DRAW
    //graphics_draw_rect(2, 22, 23, 44);
    //graphics_draw_line(2, 22, 23, 44);
    //graphics_clear();
    //graphics_draw_glyph(4,4, bitfont_ascii_to_glyph('A'));
    //graphics_draw_text(2, 2, "Hello There!\n1234567890");

    // TODO: DELETEME
    #define FFT_N 256

    Complex9_23 test_signal[FFT_N];
    for (int i = 0; i < FFT_N; i++) {
    	float T = (float) FFT_N;
    	float x = 10.0f * 2.0f * 3.14159265359f * (float) i / T;
    	test_signal[i] = complex9_23_new(fix9_23_from_f32(cosf(x)), fix9_23_int(0));
    	test_signal[i] = complex9_23_mul_re(test_signal[i], fix9_23_blackman_harris(i, FFT_N));
    }

   // test_output_complex923_buffer(test_signal, FFT_N);

    Complex9_23 test_fft_impulse_out[FFT_N] = {0};
    fft_fft(test_signal, test_fft_impulse_out, FFT_N);

    test_output_complex923_buffer(test_fft_impulse_out, FFT_N);

    plot_fft(test_fft_impulse_out, FFT_N/2, NULL, -80, 80);
    test_output_framebuffer();
    test_output_FFT();

    // TEST complex9_23_format
    //Complex9_23 somez = complex9_23_i();
    //enum SerDataType column_types[1] = {Ser_Complex9_23};
    //void* column_data[1] = { &somez };
    //enum SerResult res = ser_file_write_csv_data (stdout, column_data, 1, column_types, 1);
    //if (res != Ser_Ok) {
    //	printf("Serialization Failed!\n");
    //}

    //test_output_fixpoint_table();
    //test_output_FFT();
    //test_fixpoint_performance();
}


void test_output_framebuffer(void) {
    char* column_names[8] = { "0","1","2","3","4","5","6","7" };
    enum SerDataType column_types[8] = { Ser_Uint8,Ser_Uint8,Ser_Uint8,Ser_Uint8,Ser_Uint8,Ser_Uint8,Ser_Uint8,Ser_Uint8 };
    void* column_data[8] = { 0 };
    for (int i = 0; i < 8; i++) {
        column_data[i] = &framebuffer[i][0];
    }
    (void)ser_file_write_csv_header(stdout, column_names, 8);
    (void)ser_file_write_csv_data(stdout, column_data, OLED_WIDTH, column_types, 8);
}

void test_output_fixpoint_table(void) {

    /* TEST MATH FUNCTIONS */
    char* column_names[5] = { "x", "sqrt(x)", "cos(x)", "log2(x)", "exp(x)" };
    (void)ser_file_write_csv_header(stdout, column_names, 5);

    enum SerDataType column_types[5] = { Ser_Fix9_23, Ser_Fix9_23, Ser_Fix9_23, Ser_Fix9_23, Ser_Fix9_23 };

    const int32_t N = 10000;
    for (int32_t i = 0; i < N; i++) {
        fix9_23 x = FIX9_23_RAW(i * 214747);

        fix9_23 cos = fix9_23_cos(x);
        fix9_23 exp = fix9_23_exp(x);
        fix9_23 log = fix9_23_log2(x);
        fix9_23 sqrt = fix9_23_sqrt(x);

        void* column_data[5] = { &x, &sqrt, &cos, &log, &exp };
        (void) ser_file_write_csv_data (stdout, column_data, 1, column_types, 5);
    }

}

#define TEST_IMPULSE_LEN 512

void test_output_FFT(void) {
    char* column_names[4] = { "x", "fft(x)", "ifft(x)","ifft(fft(x))" };
    (void)ser_file_write_csv_header(stdout, column_names, 4);

    enum SerDataType column_types[4] = {
        Ser_Complex9_23,
        Ser_Complex9_23, 
        Ser_Complex9_23,
        Ser_Complex9_23, 
    };

    Complex9_23 test_impulse[TEST_IMPULSE_LEN];
    for (int i = 0; i < TEST_IMPULSE_LEN; i++) {
        test_impulse[i] = complex9_23_new(fix9_23_int(0), fix9_23_int(0));
    }
    test_impulse[0] = complex9_23_new(fix9_23_int(1), fix9_23_int(0));


    Complex9_23 test_fft_impulse_out[TEST_IMPULSE_LEN] = {0};
    Complex9_23 test_fft_ifft_impulse_out[TEST_IMPULSE_LEN] = {0};
    Complex9_23 test_ifft_impulse_out[TEST_IMPULSE_LEN] = {0};


    fft_fft(test_impulse, test_fft_impulse_out, TEST_IMPULSE_LEN);
    fft_ifft(test_fft_impulse_out,test_fft_ifft_impulse_out, TEST_IMPULSE_LEN);
    fft_ifft(test_impulse, test_ifft_impulse_out, TEST_IMPULSE_LEN);


    void* column_data[4] = { test_impulse, test_fft_impulse_out, test_ifft_impulse_out, test_fft_ifft_impulse_out};
    (void) ser_file_write_csv_data (stdout, column_data, TEST_IMPULSE_LEN, column_types, 4);

     // if (fft_fft(test_impulse, test_fft_impulse_out, 512) != 1)
   /// return // todo;

    // if (fft_ifft(test_fft_impulse_out, test_ifft_impulse_out, 512) != 1)
   // return // todo;
}

void test_fixpoint_performance(void) {
	const size_t REPETITION_COUNT = 360000;

	// INIT DWT
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    uint32_t start, end;

	// SQRT
	start = DWT->CYCCNT;
	for (size_t i = 0; i < REPETITION_COUNT; i++) {
		float x = 255.0f / REPETITION_COUNT * i;
		float sqrtx = sqrtf(x);
	    asm volatile("" : : "r,m"(sqrtx) : "memory");
	}
	end = DWT->CYCCNT;
	uint32_t sqrt_dur_float = (end-start)/REPETITION_COUNT;

	start = DWT->CYCCNT;
	for (size_t i = 0; i < REPETITION_COUNT; i++) {
		fix9_23 x = fix9_23_frac(255 * i,  REPETITION_COUNT);
		fix9_23 sqrtx = fix9_23_sqrt(x);
	    asm volatile("" : : "r,m"(sqrtx) : "memory");
	}
	end = DWT->CYCCNT;
	uint32_t sqrt_dur_fix = (end-start)/REPETITION_COUNT;

    printf("SQRT DONE!\n");

	// LOG2
	start = DWT->CYCCNT;
	for (size_t i = 0; i < REPETITION_COUNT; i++) {
		float x = 255.0f / REPETITION_COUNT * i;
		float log2x = log2f(x);
	    asm volatile("" : : "r,m"(log2x) : "memory");
	}
	end = DWT->CYCCNT;
	uint32_t log2_dur_float = (end-start)/REPETITION_COUNT;

	start = DWT->CYCCNT;
	for (size_t i = 0; i < REPETITION_COUNT; i++) {
		fix9_23 x = fix9_23_frac(255 * i,  REPETITION_COUNT);
		fix9_23 log2x = fix9_23_log2(x);
	    asm volatile("" : : "r,m"(log2x) : "memory");
	}
	end = DWT->CYCCNT;
	uint32_t log2_dur_fix = (end-start)/REPETITION_COUNT;

    printf("LOG2 DONE!\n");

	// EXP
	start = DWT->CYCCNT;
	for (size_t i = 0; i < REPETITION_COUNT; i++) {
		float x = 255.0f / REPETITION_COUNT * i;
		float expx = expf(x);
	    asm volatile("" : : "r,m"(expx) : "memory");
	}
	end = DWT->CYCCNT;
	uint32_t exp_dur_float = (end-start)/REPETITION_COUNT;

	start = DWT->CYCCNT;
	for (size_t i = 0; i < REPETITION_COUNT; i++) {
		fix9_23 x = fix9_23_frac(255 * i,  REPETITION_COUNT);
		fix9_23 expx = fix9_23_exp(x);
	    asm volatile("" : : "r,m"(expx) : "memory");
	}
	end = DWT->CYCCNT;
	uint32_t exp_dur_fix = (end-start)/REPETITION_COUNT;

    printf("EXP DONE!\n");

    // COS
	start = DWT->CYCCNT;
	for (size_t i = 0; i < REPETITION_COUNT; i++) {
		float x = 255.0f / REPETITION_COUNT * i;
		float cosx = cosf(x);
	    asm volatile("" : : "r,m"(cosx) : "memory");
	}
	end = DWT->CYCCNT;
	uint32_t cos_dur_float = (end-start)/REPETITION_COUNT;

	start = DWT->CYCCNT;
	for (size_t i = 0; i < REPETITION_COUNT; i++) {
		fix9_23 x = fix9_23_frac(255 * i,  REPETITION_COUNT);
		fix9_23 cosx = fix9_23_cos(x);
	    asm volatile("" : : "r,m"(cosx) : "memory");
	}
	end = DWT->CYCCNT;
	uint32_t cos_dur_fix = (end-start)/REPETITION_COUNT;

    printf("COS DONE!\n");

	printf("sqrt: fix: %lu cycles, float: %lu cycles\n", sqrt_dur_fix, sqrt_dur_float);
	printf("log2: fix: %lu cycles, float: %lu cycles\n", log2_dur_fix, log2_dur_float);
	printf("cos: fix: %lu cycles, float: %lu cycles\n", cos_dur_fix, cos_dur_float);
	printf("exp: fix: %lu cycles, float: %lu cycles\n", exp_dur_fix, exp_dur_float);
}

void test_output_fix923_buffer(fix9_23* buf, size_t len) {
    char* column_names[1] = { "x" };
    (void)ser_file_write_csv_header(stdout, column_names, 1);

    enum SerDataType column_types[1] = { Ser_Fix9_23 };

	void* column_data[1] = { buf };
	(void) ser_file_write_csv_data (stdout, column_data, len, column_types, 1);
}

void test_output_complex923_buffer(Complex9_23* buf, size_t len){
    char* column_names[1] = { "z" };
    (void)ser_file_write_csv_header(stdout, column_names, 1);

    enum SerDataType column_types[1] = { Ser_Complex9_23 };

	void* column_data[1] = { buf };
	(void) ser_file_write_csv_data (stdout, column_data, len, column_types, 1);
}
