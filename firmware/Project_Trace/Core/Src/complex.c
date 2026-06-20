/*
 * complex.c
 *
 *  Created on: Apr 30, 2026
 *      Author: Laurits
 */


#include <attounit.h>
#include <math.h>

#include <complex.h>



Complex9_23 complex9_23_new(fix9_23 re, fix9_23 im) {
    return (Complex9_23){.re = re, .im = im};
}
Complex9_23 complex9_23_i() {
    return (Complex9_23){.re = fix9_23_int(0), .im = fix9_23_int(1)};
}
Complex9_23 complex9_23_polar(fix9_23 mag, fix9_23 angle) {
    return complex9_23_mul_re(complex9_23_euler(angle), mag);
}

Complex9_23 complex9_23_add(Complex9_23 z1, Complex9_23 z2) {
    return complex9_23_new(fix9_23_add(z1.re, z2.re), fix9_23_add(z1.im, z2.im));
}

Complex9_23 complex9_23_sub(Complex9_23 z1, Complex9_23 z2) {
    return complex9_23_new(fix9_23_sub(z1.re, z2.re), fix9_23_sub(z1.im, z2.im));
}

Complex9_23 complex9_23_mul(Complex9_23 z1, Complex9_23 z2) {
    const fix9_23 a = z1.re;
    const fix9_23 b = z1.im;
    const fix9_23 c = z2.re;
    const fix9_23 d = z2.im;

    const fix9_23 ac = fix9_23_mul(a, c);
    const fix9_23 bd = fix9_23_mul(b, d);
    const fix9_23 ad = fix9_23_mul(a, d);
    const fix9_23 bc = fix9_23_mul(b, c);

    return complex9_23_new(fix9_23_sub(ac,bd), fix9_23_add(ad,bc));
}

Complex9_23 complex9_23_mul_re(Complex9_23 z, fix9_23 x) {
    return complex9_23_new(fix9_23_mul(z.re, x), fix9_23_mul(z.im, x));
}

Complex9_23 complex9_23_mul_int(Complex9_23 z, int32_t x) {
    z.re.raw *= x;
    z.im.raw *= x;
    return z;
}

Complex9_23 complex9_23_div(Complex9_23 z1, Complex9_23 z2) {
    const fix9_23 a = z1.re;
    const fix9_23 b = z1.im;
    const fix9_23 c = z2.re;
    const fix9_23 d = z2.im;

    const fix9_23 ac = fix9_23_mul(a, c);
    const fix9_23 bd = fix9_23_mul(b, d);
    const fix9_23 ad = fix9_23_mul(a, d);
    const fix9_23 bc = fix9_23_mul(b, c);
    const fix9_23 c2 = fix9_23_sqr(c);
    const fix9_23 d2 = fix9_23_sqr(d);

    const fix9_23 denom = fix9_23_add(c2,d2);

    return complex9_23_new(
        fix9_23_div(fix9_23_add(ac,bd), denom),
        fix9_23_div(fix9_23_sub(bc,ad), denom)
    );
}

Complex9_23 complex9_23_div_re(Complex9_23 z, fix9_23 x) {
    return complex9_23_new(
        fix9_23_div(z.re, x),
        fix9_23_div(z.im, x)
    );
}

Complex9_23 complex9_23_div_int(Complex9_23 z, int32_t x) {
    z.re.raw /= x;
    z.im.raw /= x;
    return z;
}

Complex9_23 complex9_23_conj(Complex9_23 z) {
    return complex9_23_new(z.re, fix9_23_neg(z.im));
}

fix9_23 complex9_23_abs_sqr(Complex9_23 z) {
    return complex9_23_mul(z, complex9_23_conj(z)).re;
}

fix9_23 complex9_23_abs(Complex9_23 z) {

	//float zre = fix9_23_to_f32(z.re);
	//float zim = fix9_23_to_f32(z.im);

	int64_t zre2 = (int64_t)z.re.raw * (int64_t)z.re.raw / (1 << FIX9_23_DEC_BITS);
	int64_t zim2 = (int64_t)z.im.raw * (int64_t)z.im.raw / (1 << FIX9_23_DEC_BITS);

	float zre2f = (float)zre2 / (float)(1<<FIX9_23_DEC_BITS);
	float zim2f = (float)zim2 / (float)(1<<FIX9_23_DEC_BITS);

	return fix9_23_from_f32(sqrtf(zre2f+zim2f));

//    return fix9_23_sqrt(complex9_23_abs_sqr(z));


}

Complex9_23 complex9_23_euler(fix9_23 x) {
    return (Complex9_23) {
        .re = fix9_23_cos(x),
        .im = fix9_23_sin(x)
    };
}

#define COMPLEX_FORMAT_BUF_LEN 16

int complex9_23_format(Complex9_23 z, char* buffer, size_t len) {
    char real_buf[COMPLEX_FORMAT_BUF_LEN];
    char imag_buf[COMPLEX_FORMAT_BUF_LEN];
    
    int real_len = fix9_23_format(z.re, real_buf,COMPLEX_FORMAT_BUF_LEN);
    int imag_len = fix9_23_format(z.im, imag_buf,COMPLEX_FORMAT_BUF_LEN);

    assert(real_len >= 0);
    assert(imag_len >= 0);

    if(z.im.raw >= 0) {
        return snprintf(buffer, len, "%.*s+%.*sj", real_len, real_buf, imag_len, imag_buf);
    } else { 
        return snprintf(buffer, len, "%.*s%.*sj", real_len, real_buf, imag_len, imag_buf);
    }
}
