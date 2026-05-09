/*
 * ser.c
 *
 *  Created on: May 8, 2026
 *      Author: Laurits
 */

#include <string.h>

#include <buffered_writer.h>
#include <ser.h>


#define WRITER_BUF_LEN 1024
#define FMT_BUF_LEN 32

enum SerResult ser_write_buf1_15(FILE* stream, fix1_15* buf, size_t buf_len) {
	char writer_buf[WRITER_BUF_LEN];
	char fmt_buf[FMT_BUF_LEN] = {0};
	BufferedWriter bw = bw_new(stream, writer_buf, WRITER_BUF_LEN);

	for (size_t i = 0; i < buf_len; i++) {
		int fmtlen = fix1_15_format(buf[i], fmt_buf, FMT_BUF_LEN);
		if (bw_write(&bw, fmt_buf, fmtlen) != BW_Ok) {
				return Ser_IOError;
		}

		if (i != buf_len - 1) bw_write(&bw, " ", 1);
	}

	if(bw_flush(&bw) != BW_Ok) return Ser_IOError;
	return Ser_Ok;
}

enum SerResult ser_write_buf9_23(FILE* stream, fix9_23* buf, size_t buf_len) {
	char writer_buf[WRITER_BUF_LEN];
	char fmt_buf[FMT_BUF_LEN] = {0};
	BufferedWriter bw = bw_new(stream, writer_buf, WRITER_BUF_LEN);

	for (size_t i = 0; i < buf_len; i++) {

		int fmtlen = fix9_23_format(buf[i], fmt_buf, FMT_BUF_LEN);
		if (bw_write(&bw, fmt_buf, fmtlen) != BW_Ok) {
				return Ser_IOError;
		}

		if (i != buf_len - 1) bw_write(&bw, " ", 1);
	}

	if(bw_flush(&bw) != BW_Ok) return Ser_IOError;
	return Ser_Ok;
}

enum SerResult ser_write_csv_header(FILE* stream, char** column_names, int ncols) {
	char writer_buf[WRITER_BUF_LEN];

	BufferedWriter bw = bw_new(stream, writer_buf, WRITER_BUF_LEN);

	for (int col = 0; col < ncols; col++) {
		const char* name = column_names[col];
		int name_len = strlen(name);
		if (bw_write(&bw, name, name_len) != BW_Ok) {
			return Ser_IOError;
		}

		if (col + 1 != ncols) {
			if (bw_write(&bw, ",", 1) != BW_Ok) return Ser_IOError;
		}
	}
	if(bw_flush(&bw) != BW_Ok) return Ser_IOError;
	return Ser_Ok;
}

enum SerResult ser_write_csv_data(FILE* stream, void** column_data, size_t column_length, enum SerDataType* types, int ncols) {
	char writer_buf[WRITER_BUF_LEN];
	char fmt_buf[FMT_BUF_LEN] = {0};

	BufferedWriter bw = bw_new(stream, writer_buf, WRITER_BUF_LEN);
	int fmtlen;

	for (size_t i = 0; i < column_length; i++)
	for (int col = 0; col < ncols; col++) {
		switch (types[col]) {
		case Ser_Fix9_23: {

		const fix9_23* column = column_data[col];
		const fix9_23 data = column[i];
		fmtlen = fix9_23_format(data, fmt_buf, FMT_BUF_LEN);
		if (fmtlen == 0) return Ser_FmtError;
		break;
		}
		case Ser_Float: {
			const float* column = column_data[col];
			const float data = column[i];
			fmtlen = snprintf(fmt_buf, FMT_BUF_LEN,"%f\n", data);
			break;
		}
		case Ser_Int32: {
			const int32_t* column = column_data[col];
			const int32_t data = column[i];
			fmtlen = snprintf(fmt_buf, FMT_BUF_LEN,"%ld\n", data);
			break;
		}

		default:
			__builtin_unreachable();
		}
		if (bw_write(&bw, fmt_buf, fmtlen) != BW_Ok) return Ser_IOError;

		if (col + 1 != ncols) {
			if (bw_write(&bw, ",", 1) != BW_Ok) return Ser_IOError;
		}
	}
	if(bw_flush(&bw) != BW_Ok) return Ser_IOError;
	return Ser_Ok;
}
