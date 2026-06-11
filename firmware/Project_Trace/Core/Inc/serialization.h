/*
 * ser.h
 *
 *  Created on: May 8, 2026
 *      Author: Laurits
 */

#ifndef INC_SERIALIZATION_H_
#define INC_SERIALIZATION_H_

#include <stdint.h>
#include <fixpoint.h>
#include <stdio.h>

enum SerResult {
	Ser_Ok = 0,
	Ser_IOError,
	Ser_FmtError,
};


enum SerResult ser_write_buf1_15(FILE* stream, fix1_15* buf, size_t buf_len);
enum SerResult ser_write_buf9_23(FILE* stream, fix9_23* buf, size_t buf_len);

enum SerDataType {
	Ser_Fix9_23,
	Ser_Int32,
	Ser_Uint8,
	Ser_Float,
	Ser_String,
	Ser_Bool,
};

enum SerResult ser_file_write_csv_header(FILE* stream, char** column_names, int ncols);
enum SerResult ser_file_write_csv_data(FILE* stream, void** column_data, size_t column_length, enum SerDataType* types, int ncols);

// enum SerResult ser_write_csv_header(char* destination, size_t destination_len, char** column_names, int ncols);
// enum SerResult ser_write_csv_data(char* destination, size_t destination_len, void** column_data, size_t column_length, enum SerDataType* types, int ncols);


#endif /* INC_SERIALIZATION_H_ */
