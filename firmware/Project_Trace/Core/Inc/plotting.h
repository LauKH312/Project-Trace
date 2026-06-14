/*
 * plotting.h
 *
 *  Created on: Jun 10, 2026
 *      Author: Laurits
 */

#ifndef INC_PLOTTING_H_
#define INC_PLOTTING_H_

#include <fixpoint.h>
#include <complex.h>
#include <stddef.h>

/**
 * Plots an FFT to the screen.
 * @param buckets The resulting buckets from the FFT
 * @param nbuckets The number of buckets
 * @param bucket_frequencies [OPTIONAL] The real frequecny of each bucket. Pass NULL if not used.
 */
void plot_fft(Complex9_23* buckets, size_t nbuckets, fix9_23* bucket_frequencies, int32_t min_db, int32_t max_db);
void plot_trace(fix9_23* data, size_t data_len, fix9_23 voltage_div, float horizontal_div, float deltatime);

#endif /* INC_PLOTTING_H_ */
