/*
 * ser.h
 *
 *  Created on: May 8, 2026
 *      Author: Laurits
 */

#ifndef INC_SER_H_
#define INC_SER_H_

#include <stdint.h>
#include <fixpoint.h>
#include <stdio.h>

enum SerResult {
	Ser_Ok = 0,
	Ser_IOError,
};

enum SerResult ser_write_buf1_15(FILE* stream, fix1_15* buf, size_t buf_len);
enum SerResult ser_write_buf9_23(FILE* stream, fix9_23* buf, size_t buf_len);

#endif /* INC_SER_H_ */
