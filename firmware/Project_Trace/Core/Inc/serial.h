/*
 * serial.h
 *
 *  Created on: Apr 27, 2026
 *      Author: Laurits
 *      **Based on the provided '30010_io' library from the DTU Course 30010**
 */

#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

#include <stdint.h>

#define SERIAL_BUFFER_LEN 256


void serial_init(uint32_t baud);
void serial_put_char(uint8_t c);
uint8_t serial_get_char();
uint8_t serial_get_count();
void serial_clear();

#endif /* INC_SERIAL_H_ */
