/*
 * fft.c
 *
 *  Created on: 6 May 2026
 *      Author: Marius Tyson
 */

#include "fft.h"

#define FFT_MAX_N 1024

void bit_reverse_old(Complex9_23 *a, int n) {
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j) {
            Complex9_23 tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }
}

void bit_reverse(Complex9_23 *a, int n) {
    // Reverse bits bytewise
    for (int i = 0; i < n; i++) {
        void* vre_ptr = (void*) &a->re;
        void* vim_ptr = (void*) &a->im;
        uint32_t* re_ptr = (uint32_t*)vre_ptr;
        uint32_t* im_ptr = (uint32_t*)vim_ptr;

        *re_ptr = __builtin_bswap32(*re_ptr);
        *im_ptr = __builtin_bswap32(*im_ptr);
    }
}

void fft_iterative(Complex9_23 *buf, int n, int invert) {
    bit_reverse(buf, n);
    for (int len = 2; len <= n; len <<= 1) {
        fix9_23 angle = fix9_23_div_int(FIX9_23_TAU, len);
        if (!invert)
            angle = fix9_23_neg(angle);
        
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < len / 2; j++) {
                Complex9_23 w = complex9_23_euler(fix9_23_mul_int(angle, j));
                Complex9_23 u = buf[i + j];
                Complex9_23 v = complex9_23_mul(w, buf[i + j + len / 2]);
                Complex9_23 a = complex9_23_add(u, v);
                Complex9_23 b = complex9_23_sub(u, v);

                //buf[i + j]           = complex9_23_add(u, v);
                //buf[i + j + len / 2] = complex9_23_sub(u, v);
                buf[i + j]           = complex9_23_div_int(a, 2);
                buf[i + j + len / 2] = complex9_23_div_int(b, 2);
            }
        }
    }
    //if (invert) {
    //    for (int i = 0; i < n; i++)
    //        buf[i] = complex9_23_div_int(buf[i], n);
    //}

    if (!invert) {
    for (int i = 0; i < n; i++)
    	buf[i] = complex9_23_mul_int(buf[i],n);
    }
}

int is_power_of_two(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

int next_power_of_two(int n) {
    int p = 1;
    while (p < n) p <<= 1;
    return p;
}

int prepare_buf(Complex9_23 *buf, const Complex9_23 *P, int n, int *out_padded_n) {
    int padded = is_power_of_two(n) ? n : next_power_of_two(n);
    if (padded > FFT_MAX_N)
        return -1;
    memcpy(buf, P, n * sizeof(Complex9_23));
    memset(&buf[n], 0, (padded - n) * sizeof(Complex9_23));
    *out_padded_n = padded;
    return 1;
}

int fft_fft(const Complex9_23 *P, Complex9_23 *y, int n) {
    Complex9_23 buf[FFT_MAX_N];
    int padded_n;
    if (prepare_buf(buf, P, n, &padded_n) != 1)
        return -1;
    fft_iterative(buf, padded_n, 0);
    memcpy(y, buf, n * sizeof(Complex9_23));
    return 1;
}

int fft_ifft(const Complex9_23 *P, Complex9_23 *y, int n) {
    Complex9_23 buf[FFT_MAX_N];
    int padded_n;
    if (prepare_buf(buf, P, n, &padded_n) != 1)
        return -1;
    fft_iterative(buf, padded_n, 1);
    memcpy(y, buf, n * sizeof(Complex9_23));
    return 1;
}
