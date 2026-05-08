/*
 * buffered_writer.h
 *
 *  Created on: May 8, 2026
 *      Author: Laurits
 */

#ifndef INC_BUFFERED_WRITER_H_
#define INC_BUFFERED_WRITER_H_

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>

enum BWResult {
	BW_Ok = 0,
	BW_IOError,
};

typedef struct {
	FILE* stream;
    char* buffer;
    size_t occupied;
    size_t capacity;
} BufferedWriter;

BufferedWriter bw_new(FILE* stream, char* buffer, size_t buffer_len);
enum BWResult bw_write(BufferedWriter* bw, char* string, size_t string_len);
//enum BufferedWriterResult bw_writef(BufferedWriter* bw, char* format_buffer, char* format, ...);
enum BWResult bw_flush(BufferedWriter* bw);
void bw_clear(BufferedWriter* bw);


#endif /* INC_BUFFERED_WRITER_H_ */
