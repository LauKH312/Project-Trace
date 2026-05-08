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

fix9_23 meas_scale_sample(fix1_15 x, fix9_23 unit);
fix9_23 meas_average(fix1_15* buf, size_t buffer_length, fix9_23 unit);
// TODO
// fix9_23 meas_rms(fix1_15* buf, size_t buffer_length, fix9_23 unit);
fix9_23 meas_max(fix1_15* buf, size_t buffer_length, fix9_23 unit);
fix9_23 meas_min(fix1_15* buf, size_t buffer_length, fix9_23 unit);

_Bool rising_edge(fix1_15* buf, size_t buffer_length, fix1_15 threshold, size_t* rising_edge_idx);

#endif /* INC_MEAS_H_ */
