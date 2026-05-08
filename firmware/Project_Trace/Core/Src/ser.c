/*
 * ser.c
 *
 *  Created on: May 8, 2026
 *      Author: Laurits
 */


#include <ser.h>
#include <buffered_writer.h>


#define WRITER_BUF_LEN 128
#define FMT_BUF_LEN 64

enum SerResult ser_write_buf1_15(FILE* stream, fix1_15* buf, size_t buf_len) {
	char writer_buf[WRITER_BUF_LEN];
	char fmt_buf[FMT_BUF_LEN] = {0};
	BufferedWriter bw = bw_new(stream, writer_buf, WRITER_BUF_LEN);

	for (size_t i = 0; i < buf_len; i++) {
		int fmtlen = fix1_15_format(buf[i], fmt_buf, FMT_BUF_LEN);
		enum BWResult res = bw_write(&bw, fmt_buf, fmtlen);
		switch(res){
		case BW_Ok:
			break;
		case BW_IOError:
			return Ser_IOError;
		default:
		}

		if (i != buf_len - 1) bw_write(&bw, " ", 1);
	}

	bw_flush(&bw);
	return Ser_Ok;
}

enum SerResult ser_write_buf9_23(FILE* stream, fix9_23* buf, size_t buf_len) {
	char writer_buf[WRITER_BUF_LEN];
	char fmt_buf[FMT_BUF_LEN] = {0};
	BufferedWriter bw = bw_new(stream, writer_buf, WRITER_BUF_LEN);

	for (size_t i = 0; i < buf_len; i++) {

		int fmtlen = fix9_23_format(buf[i], fmt_buf, FMT_BUF_LEN);
		enum BWResult res = bw_write(&bw, fmt_buf, fmtlen);
		switch(res){
			case BW_Ok:
				break;
			case BW_IOError:
				return Ser_IOError;
			default:
		}

		if (i != buf_len - 1) bw_write(&bw, " ", 1);
	}

	bw_flush(&bw);
	return Ser_Ok;
}


