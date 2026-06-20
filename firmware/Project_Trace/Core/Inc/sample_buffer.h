/*
 * sampling.h
 *
 *  Created on: Jun 5, 2026
 *      Author: Laurits
 */

#ifndef INC_SAMPLE_BUFFER_H_
#define INC_SAMPLE_BUFFER_H_

#include <stddef.h>

#include <fixpoint.h>

// TODO: choose proper value.
// #define SAMPLE_BUFFER_LEN 65536
#define SAMPLE_BUFFER_LEN 16384

typedef struct {
	fix9_23 buf[SAMPLE_BUFFER_LEN];
    // CircularBufferFix9_32 circbuf;
	size_t len;
	size_t write_idx;
	//_Bool ready_to_read;
	int32_t samplerate;
	// TODO - In the future, the buffer should be a union type, such we can store raw ADC samples if averaging is not used.

	// On ADC buffer full:
	// Step 1: Average samples into temporary buffer if applicable
	// Step 2: Write the samples into the samplebuffer, discarding old ones if full.
	// Step 3: Serialize and write to USB if enabled, using a DMA.
	// On frame update: Render with samplebuffer (skip if not ready)
} SampleBuffer;

// SampleBuffer sample_buffer;

void sample_buffer_init(SampleBuffer* sb, int32_t sample_rate);
void sample_buffer_write_samples(SampleBuffer* sb, const fix9_23* samples, size_t samples_len);
size_t sample_buffer_peek_samples(const SampleBuffer* sb, fix9_23* target, size_t target_len);

#endif /* INC_SAMPLE_BUFFER_H_ */
