/*
 * plotting.c
 *
 *  Created on: Jun 10, 2026
 *      Author: Laurits
 */


#include <plotting.h>
#include <graphics.h>
#include <hal_oled.h>
#include <meas.h>
#include <assert.h>
#include <stdint.h>

void plot_fft(Complex9_23* buckets, size_t nbuckets, fix9_23* bucket_frequencies, int32_t min_db, int32_t max_db) {
	(void)bucket_frequencies;

	const int32_t MAXIMUM_Y = 20;
	const int32_t MINIMUM_Y = OLED_HEIGHT;

	for (size_t i = 0; i < nbuckets; i++) {
		fix9_23 amp = complex9_23_abs(buckets[i]);

		if (amp.raw < 1) {
			amp.raw = 1;
		}

		fix9_23 amp_db = fix9_23_gain_to_db(amp);

		fix9_23 y = fix9_23_map(amp_db, fix9_23_int(min_db), fix9_23_int(max_db), fix9_23_int(MINIMUM_Y), fix9_23_int(MAXIMUM_Y));
		int32_t yint = fix9_23_round(y);

		graphics_draw_vertical_line(i, MINIMUM_Y, yint);

		//hal_oled_drawpixel(i, yint, HalOledDrawOn);
	}

	for (int i = 0; i < OLED_WIDTH; i++) {
		fix9_23 y = fix9_23_map(fix9_23_int(0), fix9_23_int(min_db), fix9_23_int(max_db), fix9_23_int(MINIMUM_Y), fix9_23_int(MAXIMUM_Y));
		int32_t yint = fix9_23_round(y);

		hal_oled_drawpixel(i, yint, (i%2 == 0) ? HalOledDrawOn: HalOledDrawOff);
	}
}

#define NUMBER_HORIZONTAL_DIVS 4
#define NUMBER_VERTICAL_DIVS 4

void plot_trace(fix9_23* data, size_t data_len, fix9_23 voltage_div, float horizontal_div, float deltatime) {
	assert(data != NULL);
	assert(data_len != 0);
	assert(voltage_div.raw != 0);
	assert(horizontal_div != 0.f);
	assert(deltatime != 0.f);

	const int32_t HORIZONTAL_DIV_PIX = OLED_WIDTH / NUMBER_HORIZONTAL_DIVS;
	const int32_t VERTICAL_DIV_PIX = OLED_HEIGHT / NUMBER_VERTICAL_DIVS;

	const fix9_23 TRIGGER_THRESHOLD = {0};
	const int32_t TRIGGER_POINT_X = OLED_WIDTH / 8;
	const int32_t CENTER_Y = OLED_HEIGHT / 2;


	const float T = (float) NUMBER_HORIZONTAL_DIVS * horizontal_div;
	const int32_t number_samples = (int32_t) (T / deltatime);

	const int32_t trigger_point_idx = TRIGGER_POINT_X * number_samples / (NUMBER_HORIZONTAL_DIVS * HORIZONTAL_DIV_PIX);


	size_t rising_edge_idx;
	int32_t offset = 0;
	if(!meas_rising_edge(data, data_len, TRIGGER_THRESHOLD, &rising_edge_idx)) {
		offset = 0;
	} else {
		offset = trigger_point_idx - (int32_t)rising_edge_idx;
	}

	graphics_draw_horizontal_line(0, OLED_WIDTH, CENTER_Y);

	for(int32_t i = 0; i < NUMBER_HORIZONTAL_DIVS + 1; i++) {
		graphics_draw_vertical_line(i * HORIZONTAL_DIV_PIX, 0, OLED_HEIGHT);
	}

	for (size_t i = 0; i < data_len; i++) {
		fix9_23 y_div = fix9_23_div(data[i], voltage_div); // y normalized to the fractional 'div-number' it sits on.


		int32_t y = CENTER_Y - fix9_23_trunc(fix9_23_mul(y_div, fix9_23_int(VERTICAL_DIV_PIX)));
		int32_t x = offset + (int32_t)(i * NUMBER_HORIZONTAL_DIVS * HORIZONTAL_DIV_PIX / number_samples);

		// TODO: output may have bad quantization errors
		hal_oled_drawpixel(x, y, HalOledDrawOn);
	}
}
