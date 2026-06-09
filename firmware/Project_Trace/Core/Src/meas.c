/*
 * meas.c
 *
 *  Created on: May 4, 2026
 *      Author: Laurits
 */


#include <meas.h>

fix9_23 meas_average(fix9_23* buf, size_t buffer_length) {
    int64_t acc = 0;
    for (size_t i = 0; i < buffer_length; i++) {
        const fix9_23 x = buf[i];
        acc += x.raw;
    } 
    acc /= (int64_t) buffer_length;
    return FIX9_23_RAW((int32_t) acc);
}

fix9_23 meas_rms(fix9_23* buf, size_t buffer_length) {
    int64_t acc = 0;
    for (size_t i = 0; i < buffer_length; i++) {
        const fix9_23 x = buf[i];
        acc += fix9_23_sqr(x).raw;
    }

    acc /= buffer_length;
    return fix9_23_sqrt(FIX9_23_RAW((int32_t) acc));
}

fix9_23 meas_max(fix9_23* buf, size_t buffer_length) {
    fix9_23 max = FIX9_23_RAW(INT32_MIN);

    for (size_t i = 0; i < buffer_length; i++) {
        const fix9_23 x = buf[i];
        if (x.raw > max.raw) max = x;
    }

    return max;

}

fix9_23 meas_min(fix9_23* buf, size_t buffer_length) {
    fix9_23 min = FIX9_23_RAW(INT32_MAX);

    for (size_t i = 0; i < buffer_length; i++) {
        const fix9_23 x = buf[i];
        if (x.raw > min.raw) min = x;
    }

    return min;
}

_Bool rising_edge(fix9_23* buf, size_t buffer_length, fix9_23 threshold, size_t* rising_edge_idx) {
	_Bool previous_below_thr = 0;
	for (size_t i = 0; i < buffer_length; i++) {

		if (buf[i].raw > threshold.raw && previous_below_thr) {
			*rising_edge_idx = i;
			return 1;
		}

		previous_below_thr = buf[i].raw < threshold.raw;
	}
	return 0;
}
