/*
 * fft.h
 *
 *  Created on: May 12, 2026
 *      Author: Marius Tyson
 */

#ifndef INC_FFT_H
#define INC_FFT_H

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include "fixpoint.h"
#include "complex.h"
#include "string.h"

void bit_reverse(Complex9_23 *a, int n);
void fft_iterative(Complex9_23 *buf, int n, int invert);
int is_power_of_two(int n);
int next_power_of_two(int n);
int prepare_buf(Complex9_23 *buf, const Complex9_23 *P, int n, int *out_padded_n);
int fft_fft(const Complex9_23 *P, Complex9_23 *y, int n);
int fft_ifft(const Complex9_23 *P, Complex9_23 *y, int n);

#endif /* INC_FFT_H */
