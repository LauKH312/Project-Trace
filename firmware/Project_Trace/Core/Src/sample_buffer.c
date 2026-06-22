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

SampleBuffer sample_buffer;

void sample_buffer_init(SampleBuffer* sb, int32_t sample_rate) {
    memset(sb->buf, 0, sizeof(sb->buf));
    sb->len = 0;
    sb->write_idx = 0;
    sb->samplerate = sample_rate;
}

void sample_buffer_write_samples(SampleBuffer* sb, const fix9_23* samples, size_t samples_len) {
    assert(samples_len <= SAMPLE_BUFFER_LEN);
    assert(sb->write_idx < SAMPLE_BUFFER_LEN);
    assert(sb->len <= SAMPLE_BUFFER_LEN);

    for (size_t i = 0; i < samples_len; i++) {
        sb->buf[sb->write_idx] = samples[i];
        sb->write_idx = sample_buffer_writeidx_offset(sb, 1);
        sb->len = MIN(sb->len + 1, SAMPLE_BUFFER_LEN);
    }
}

size_t sample_buffer_peek_samples(const SampleBuffer* sb, fix9_23* target, size_t target_len) {
    size_t values_read = MIN(sb->len, target_len);

    for (size_t i = 0; i < values_read; i++) {
        target[i] = sample_buffer_get(sb, SAMPLE_BUFFER_LEN - sb->len + i);
    }

    return values_read;
}

fix9_23 sample_buffer_get(const SampleBuffer* sb, size_t idx) {
	return sb->buf[(sb->write_idx + idx) % SAMPLE_BUFFER_LEN];
}

size_t sample_buffer_writeidx_offset(const SampleBuffer* sb, size_t offset) {
	return (sb->write_idx + offset) % SAMPLE_BUFFER_LEN;
}
