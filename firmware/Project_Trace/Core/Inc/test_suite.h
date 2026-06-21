/*
 * test_suite.h
 *
 *  Created on: Apr 27, 2026
 *      Author: Alexander
 */

#ifndef INC_TEST_SUITE_H_
#define INC_TEST_SUITE_H_

#include <fixpoint.h>
#include <complex.h>

void test_suite(void);

void test_output_framebuffer(void);
void test_output_fixpoint_table(void);
void test_output_FFT(void);
void test_fixpoint_performance(void);

void test_output_fix923_buffer(const fix9_23* buf, size_t len, _Bool write_header);
void test_output_complex923_buffer(Complex9_23* buf, size_t len);

#endif /* INC_TEST_SUITE_H_ */
