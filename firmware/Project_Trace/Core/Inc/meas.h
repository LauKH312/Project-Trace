/*
 * meas.h
 *
 *  Created on: May 4, 2026
 *      Author: Laurits
 */

#ifndef INC_MEAS_H_
#define INC_MEAS_H_

#include <fixpoint.h>
#include <stddef.h>

fix9_23 meas_scale_sample(fix9_23 x);
fix9_23 meas_average(fix9_23* buf, size_t buffer_length);

fix9_23 meas_rms(fix9_23* buf, size_t buffer_length);
fix9_23 meas_max(fix9_23* buf, size_t buffer_length);
fix9_23 meas_min(fix9_23* buf, size_t buffer_length);

_Bool rising_edge(fix9_23* buf, size_t buffer_length, fix9_23 threshold, size_t* rising_edge_idx);

#endif /* INC_MEAS_H_ */
