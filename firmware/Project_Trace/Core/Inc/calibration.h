/*
 * calibration.h
 *
 *  Created on: May 4, 2026
 *      Author: Laurits
 * 
 */

#ifndef INC_CALIBRATION_H_
#define INC_CALIBRATION_H_

#include <fixpoint.h>
#include <stddef.h>

/*
Model Interface
fix9_23 calibration_model(fix1_15);
*/

// TODO: undefined whether this is fractional or not. (is converted directly to fix1.15 in `cal_calibrate_buffer_1_15`)
fix9_23 cal_calibrate_input(int16_t x);
void cal_calibrate_buffer(fix9_23* out, const int16_t* input, size_t buffer_length);
void cal_calibrate_buffer_1_15(fix1_15* out, const int16_t* input, size_t buffer_length);

#endif /* INC_CALIBRATION_H_ */
