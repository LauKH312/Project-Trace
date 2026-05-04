/*
 * meas.c
 *
 *  Created on: May 4, 2026
 *      Author: Laurits
 */


#include <meas.h>

fix9_23 meas_scale_sample(fix1_15 x, fix9_23 unit) {
    return fix9_23_mul(fix9_23_from_f1_15(x), unit);
}

fix9_23 meas_average(fix1_15* buf, size_t buffer_length, fix9_23 unit) {
    int64_t acc = 0;
    for (size_t i = 0; i < buffer_length; i++) {
        const fix9_23 x = meas_scale_sample(buf[i], unit);
        acc += x.raw;
    } 
    acc /= (int64_t) buffer_length;
    return fix9_23_from_raw((int32_t) acc); 
}

// fix9_23 meas_rms(fix1_15* buf, size_t buffer_length, fix9_23 unit) {

// }

fix9_23 meas_max(fix1_15* buf, size_t buffer_length, fix9_23 unit) {
    fix9_23 max = fix9_23_from_raw(INT32_MIN);

    for (size_t i = 0; i < buffer_length; i++) {
        const fix9_23 x = meas_scale_sample(buf[i], unit);
        if (x.raw > max.raw) max = x;
    }

    return max;

}

fix9_23 meas_min(fix1_15* buf, size_t buffer_length, fix9_23 unit) {
    fix9_23 min = fix9_23_from_raw(INT32_MAX);

    for (size_t i = 0; i < buffer_length; i++) {
        const fix9_23 x = meas_scale_sample(buf[i], unit);
        if (x.raw > min.raw) min = x;
    }

    return min;
}
