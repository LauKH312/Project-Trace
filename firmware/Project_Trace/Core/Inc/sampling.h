/*
 * sampling.h
 *
 *  Created on: Jun 5, 2026
 *      Author: Laurits
 */

#ifndef INC_SAMPLING_H_
#define INC_SAMPLING_H_

#include <fixpoint.h>
#include <stddef.h>

// TODO: choose proper value.
#define SAMPLE_BUFFER_LEN 65536


typedef struct {
	fix9_23 buf[SAMPLE_BUFFER_LEN];
	size_t len;
	_Bool ready_to_read;
	int32_t samplerate;
	// TODO - In the future, the buffer should be a union type, such we can store raw ADC samples if averaging is not used.

	// On ADC buffer full:
	// Step 1: Average samples into temporary buffer if applicable
	// Step 2: Write the samples into the samplebuffer, discarding old ones if full.
	// Step 3: Serialize and write to USB if enabled, using a DMA.
	// On frame update: Render with samplebuffer (skip if not ready)
} SampleBuffer;

SampleBuffer sample_buffer;

#endif /* INC_SAMPLING_H_ */
