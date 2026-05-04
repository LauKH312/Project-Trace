/*
 * calibration.c
 *
 *  Created on: May 4, 2026
 *      Author: Laurits
 */


#include <calibration.h>


fix9_23 cal_calibrate_input(int16_t x) {
    const fix9_23 a = fix9_23_int(1);
    const fix9_23 b = fix9_23_int(0);

    const fix9_23 x9_23 = fix9_23_from_raw(x);
    return fix9_23_add(fix9_23_mul(a, x9_23), b);
}


void cal_calibrate_buffer(fix9_23* out, const int16_t* input, size_t buffer_length) {
	for (size_t i = 0 ; i < buffer_length; i++) {
		out[i] = cal_calibrate_input(input[i]);
	}
}


void cal_calibrate_buffer_1_15(fix1_15* out, const int16_t* input, size_t buffer_length) {
	for (size_t i = 0 ; i < buffer_length; i++) {
		out[i] = fix1_15_from_f9_23(cal_calibrate_input(input[i]));
	}
}