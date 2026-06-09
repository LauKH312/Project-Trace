/*
 * sampling.c
 *
 *  Created on: Jun 5, 2026
 *      Author: Laurits
 */

#include <assert.h>
#include <string.h>
#include <mathdefs.h>
#include <sample_buffer.h>

void sample_buffer_init(SampleBuffer* sb, int32_t sample_rate) {
	memset(sb->buf, 0, sizeof(sb->buf));
	sb->len = 0;
	sb->write_idx = 0;
	//sb->ready_to_read = 0;
	sb->samplerate = sample_rate;
}

// TODO: ALMOST CERTAINLY NON-FUNCTIONAL. TEST ASAP
void sample_buffer_write_samples(SampleBuffer* sb, const fix9_23* samples, size_t samples_len) {
	assert(samples_len <= SAMPLE_BUFFER_LEN); // If violated, new samples would be overwritten.

	if(sb->len + samples_len <= SAMPLE_BUFFER_LEN) {
		assert(sb->write_idx + samples_len <= SAMPLE_BUFFER_LEN);

		memcpy(&sb->buf[sb->write_idx], samples, samples_len * sizeof(fix9_23));
		sb->len += samples_len;
		sb->write_idx += samples_len;

		return;
	}

	/*
	size_t fwd_samples_len = SAMPLE_BUFFER_LEN - sb->write_idx - 1;
	size_t rem = samples_len - fwd_samples_len;
	assert(rem > 0);

	memcpy(&sb->buf[sb->write_idx], samples, fwd_samples_len * sizeof(fix9_23));
	sb->write_idx = 0;
	memcpy(&sb->buf[sb->write_idx], &samples[fwd_samples_len], rem * sizeof(fix9_23));
	sb->len = SAMPLE_BUFFER_LEN;
	sb->write_idx += rem;
	 */

	for (size_t i = 0; i < samples_len; i++) {
		sb->buf[sb->write_idx] = samples[i];
		sb->write_idx = (sb->write_idx + 1) % SAMPLE_BUFFER_LEN;
		sb->len = MIN(sb->len, SAMPLE_BUFFER_LEN);
	}
}

void sample_buffer_read_samples(SampleBuffer* sb, fix9_23* target, size_t target_len) {
	for (size_t i = 0; i < target_len; i++) {
		target[i] = sb->buf[(sb->write_idx + i) % SAMPLE_BUFFER_LEN];
	}
}
