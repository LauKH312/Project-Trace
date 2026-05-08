/*
 * buffered_writer.c
 *
 *  Created on: May 8, 2026
 *      Author: Laurits
 */

#include <buffered_writer.h>
#include <string.h>

BufferedWriter bw_new(FILE* stream, char* buffer, size_t buffer_len) {
	BufferedWriter bw;
	bw.stream = stream;
    bw.buffer = buffer;
    bw.occupied = 0;
    bw.capacity = buffer_len;
    return bw;
}
enum BWResult bw_write(BufferedWriter* bw, char* string, size_t string_len) {
    if (bw->occupied + string_len > bw->capacity) {
        bw_flush(bw);
        size_t written = fwrite(string, 1, string_len, bw->stream);
        if (written != string_len) return BW_IOError;

        return BW_Ok;
    }

    memcpy(&bw->buffer[bw->occupied], string, string_len);
    bw->occupied += string_len;
    return BW_Ok;
}
/*
enum BufferedWriterResult bw_writef(BufferedWriter* bw, char* format_buffer, char* format, ...) {
    va_list args;
    va_start(args, format);
    size_t string_len = vsnprintf(NULL, 0, format, args);
    va_end(args);

    va_start(args, format);
    vsnprintf(string, string_len + 1, format, args);
    va_end(args);

    bw_fwrite(bw, string, string_len);
    free(string);
}*/

enum BWResult bw_flush(BufferedWriter* bw) {
    size_t written = fwrite(bw->buffer, 1, bw->occupied, bw->stream);
    size_t occupied = bw->occupied;
    bw->occupied = 0;
    if (written == occupied) return BW_Ok;
    else return BW_IOError;
}

void bw_clear(BufferedWriter* bw) {
    bw->occupied = 0;
}
