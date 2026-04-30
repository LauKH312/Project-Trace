/*
 * complex.c
 *
 *  Created on: Apr 30, 2026
 *      Author: Laurits
 */


#include <complex.h>
#include <attounit.h>


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

Complex9_23 complex9_23_euler(fix9_23 x) {
    return (Complex9_23) {
        .re = fix9_23_cos(x),
        .im = fix9_23_sin(x)
    };
}
