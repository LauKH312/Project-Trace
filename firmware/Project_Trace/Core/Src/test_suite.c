/*
 * test_suite.c
 *
 *  Created on: Apr 27, 2026
 *      Author: Alexander & Marius Tyson
 */

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

extern uint8_t framebuffer[OLED_PAGES][OLED_WIDTH];

#define BUFSZ 64

TEST_MAIN()
void test_suite(void) {
    int code = attomain();
    printf("Attotest exited with code %d\n", code);



    //graphics_draw_rect(2, 22, 23, 44);
    //graphics_draw_line(2, 22, 23, 44);
    //graphics_clear();
    //graphics_draw_glyph(4,4, bitfont_ascii_to_glyph('A'));
    //graphics_draw_text(2, 2, "Hello There!\n1234567890");

    //test_output_framebuffer();

    Complex9_23 somez = complex9_23_i();
    enum SerDataType column_types[1] = {Ser_Complex9_23};
    void* column_data[1] = { &somez };
    enum SerResult res = ser_file_write_csv_data (stdout, column_data, 1, column_types, 1);
    if (res != Ser_Ok) {
    	printf("Serialization Failed!\n");
    }


    test_output_FFT();
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

void test_output_FFT(void) {
    char* column_names[3] = { "test", "fft", "ifft" };
    (void)ser_file_write_csv_header(stdout, column_names, 3);

    enum SerDataType column_types[3] = {
        Ser_Complex9_23,
        Ser_Complex9_23, 
        Ser_Complex9_23, 
    };

    Complex9_23 test_impulse[512];
    for (int i = 0; i < 512; i++) {
        test_impulse[i] = complex9_23_new(fix9_23_int(0), fix9_23_int(0));
    }
    test_impulse[0] = complex9_23_new(fix9_23_int(1), fix9_23_int(0));


    Complex9_23 test_fft_impulse_out[512] = {0};
    Complex9_23 test_ifft_impulse_out[512] = {0};


    fft_fft(test_impulse, test_fft_impulse_out, 512);
    fft_ifft(test_fft_impulse_out,test_ifft_impulse_out, 512);


    void* column_data[3] = { test_impulse, test_fft_impulse_out, test_ifft_impulse_out};
    (void) ser_file_write_csv_data (stdout, column_data, 512, column_types, 3);

     // if (fft_fft(test_impulse, test_fft_impulse_out, 512) != 1)
   /// return // todo;

    // if (fft_ifft(test_fft_impulse_out, test_ifft_impulse_out, 512) != 1)
   // return // todo;



}
