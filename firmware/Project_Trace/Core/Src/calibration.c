/*
 * calibration.c
 *
 *  Created on: May 4, 2026
 *      Author: Laurits
 */


#include <calibration.h>


fix9_23 cal_calibrate_input(int16_t x) {
	//const float COEFFS[2] = { 2.14125242e-05, -1.40869303e-03 };
	//return fix9_23_from_f32(COEFFS[0] * (float) x + COEFFS[1]);
	float xf = x;
	const float COEFFS[3] = { -3.49059013e-13,  2.14121482e-05, -9.43319572e-04 };
	return fix9_23_from_f32(COEFFS[2] + xf * (COEFFS[1] + xf * COEFFS[0]));
}


void cal_calibrate_buffer(fix9_23* out, const int16_t* input, size_t buffer_length) {
	for (size_t i = 0 ; i < buffer_length; i++) {
		out[i] = cal_calibrate_input(input[i]);
	}
}

