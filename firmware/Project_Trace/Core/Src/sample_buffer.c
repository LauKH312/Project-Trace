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
    size_t rhs_contiguous = SAMPLE_BUFFER_LEN - sb->write_idx;

    if (samples_len <= rhs_contiguous &&
        sb->len + samples_len <= SAMPLE_BUFFER_LEN) {
        memcpy(&sb->buf[sb->write_idx], samples, samples_len * sizeof(fix9_23));
        sb->len += samples_len;
        sb->write_idx = (sb->write_idx + samples_len) % SAMPLE_BUFFER_LEN;;
        return;
    }

    for (size_t i = 0; i < samples_len; i++) {
        sb->buf[sb->write_idx] = samples[i];
        sb->write_idx = (sb->write_idx + 1) % SAMPLE_BUFFER_LEN;
        sb->len = MIN(sb->len + 1, SAMPLE_BUFFER_LEN);
    }
}

size_t sample_buffer_peek_samples(const SampleBuffer* sb, fix9_23* target, size_t target_len) {
    size_t values_read = MIN(sb->len, target_len);
    
    size_t oldest_sample =
        (sb->write_idx + SAMPLE_BUFFER_LEN - sb->len)
        % SAMPLE_BUFFER_LEN;

    for (size_t i = 0; i < values_read; i++) {
        target[i] = sb->buf[(oldest_sample + i) % SAMPLE_BUFFER_LEN];
    }

    return values_read;
}
