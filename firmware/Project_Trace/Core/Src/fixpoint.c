/*
 * fixpoint.c
 *
 *  Created on: 27 Apr 2026
 *      Author: lauri
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fixpoint.h>
#include <attounit.h>

TEST_SUITE(fixpoint)
BEFORE_EACH() {}
AFTER_EACH() {}

fix1616 fix1616_int(int16_t x) {
    return FIX_INT(x, FIX16_16_DEC_BITS);
}

fix1616 fix1616_frac(int16_t a, int16_t b) {
    return FIX_RATIONAL(a, b, FIX16_16_DEC_BITS);
}


// The following function is an adaption of the macro `FIX14_MULT` from the 3rd exercise.
fix1616 fix1616_mul(fix1616 a, fix1616 b) {
    return ((int64_t)a * (int64_t)b) >> FIX16_16_DEC_BITS;
}

// The following function is an adaption of the macro `FIX14_DIV` from the 3rd exercise.
fix1616 fix1616_div(fix1616 a, fix1616 b) {
    // return FIX_DIV(a, b, FIX1616_SHIFT);
    return ((int64_t)a << FIX16_16_DEC_BITS) / b;
}

fix1616 fix1616_sqr(fix1616 x) {
    return fix1616_mul(x, x);
}

// Computes the square root of `x` using the Newton-Raphson method
fix1616 fix1616_sqrt(fix1616 x) {
    // Derivation:
    // g = sqrt(x)
    // g^2=x
    // x-g^2 = 0
    // We let f(g) = x - (g^2)
    // Then f'(g) = - 2g

    const int NEWTON_STEPS = 8;

    // fix1616 guess = fix1616_int(1);
    fix1616 guess = fix1616_int(x >> 1);

    for (int i = 0; i < NEWTON_STEPS; i++) {
        guess = guess - fix1616_div(x - fix1616_sqr(guess), -(guess + guess));
    }

    return guess;
}

// Computes the inverse square root of `x` using the Newton-Raphson method
fix1616 fix1616_isqrt(fix1616 x) {
    // g = 1/sqrt(x)
    // therefore
    // g * sqrt(x) = 1
    // x*g^2 = 1
    // 0 = x*g^2-1
    // f(g) = x*g^2-1
    // f'(g) = 2xg

    const int NEWTON_STEPS = 8;
    const fix1616 ONE = fix1616_int(1);
    fix1616 guess = fix1616_int(1);

    for (int i = 0; i < NEWTON_STEPS; i++) {
        guess = guess - fix1616_div(fix1616_mul(x, fix1616_sqr(guess)) - ONE, fix1616_mul(x, (guess + guess)));
    }

    return guess;
}


// NOTE: The implementation of this function is taken from the guide for Exercise 3.
void fix1616_print(fix1616 x) {
    if ((x & 0x80000000) != 0) { // Handle negative numbers
        printf("-");
        x = ~x + 1;
    }
    printf("%ld.%04ld", ((int32_t)x) >> 16, 10000 * (uint32_t)(x & 0xFFFF) >> 16);// Print a maximum of 4 decimal digits to avoid overflow
}

int16_t fix1616_round(fix1616 x) {
    const fix1616 HALF = fix1616_frac(1, 2);
    if (x >= 0)
        return fix1616_trunc(x + HALF);
    else
        return fix1616_trunc(x - HALF);
}

int16_t fix1616_trunc(fix1616 x) {
    return x >> FIX16_16_DEC_BITS;
}

fix1616 fix1616_lerp(fix1616 a, fix1616 b, fix1616 t) {
    const fix1616 diff = b - a;
    return a + fix1616_mul(diff, t);
}

fix1_15 fix1_15_int(int16_t n)  {
    assert(n == 0 || n == 1);

    return (fix1_15){.raw = n << FIX1_15_DEC_BITS};
}

fix1_15 fix1_15_from_raw(int16_t raw) {
    return (fix1_15){.raw = raw};
}

fix1_15 fix1_15_frac(int16_t num, int16_t denom) {
    return fix1_15_from_raw(((int32_t)num << FIX1_15_DEC_BITS) / denom);
}

fix1_15 fix1_15_add(fix1_15 a, fix1_15 b) {
    return (fix1_15){.raw = a.raw + b.raw};
}

fix1_15 fix1_15_sub(fix1_15 a, fix1_15 b) {
    return (fix1_15){.raw = a.raw - b.raw};
}

fix1_15 fix1_15_mul(fix1_15 a, fix1_15 b) {
    return (fix1_15){.raw = (int16_t)(((int32_t)a.raw * (int32_t)b.raw) >> FIX1_15_DEC_BITS)};
}

fix1_15 fix1_15_div(fix1_15 a, fix1_15 b) {
    return (fix1_15){.raw = (int16_t)((((int32_t)a.raw) << FIX1_15_DEC_BITS) / b.raw)};
}

fix1_15 fix1_15_powi(fix1_15 base, int16_t exponent) {
    if (exponent < 0) return fix1_15_div(fix1_15_int(1), fix1_15_powi(base,-exponent));
    if (exponent == 0) return fix1_15_int(1);
    if (exponent == 1) return base;

    if ((exponent & 1) == 0) {
        const fix1_15 tmp = fix1_15_powi(base, exponent / 2);
        return fix1_15_mul(tmp, tmp);
    } else {
        const fix1_15 tmp = fix1_15_powi(base, (exponent-1) / 2);
        return fix1_15_mul(fix1_15_mul(base, tmp), tmp);
    }

}

fix1_15 fix1_15_sqrt(fix1_15 x) {
    const int NUMBER_GUESSES = 8;

    const fix1_15 initial_guess = fix1_15_from_raw(x.raw >> 1);

    // x = root^2
    // 0 = x - root^2
    fix1_15 root = initial_guess;
    for(int i = 0; i < NUMBER_GUESSES; i++) {
        // TODO SOLVE INT OUT OF RANGE
        fix1_15 delta = fix1_15_div(fix1_15_mul(root,root), fix1_15_mul(fix1_15_int(-2), root));
        root = fix1_15_sub(x, delta);
    }

    return root;
}

enum FixPointResult fix1_15_format(fix1_15 i, char* buffer, size_t len) {
    // TODO error handling
    //if (len < )
    if (i.raw < 0) {
        //printf("-");
        const int16_t sign_bit_mask = 0x8000;
        i.raw = ~i.raw + 1;
        if (i.raw & sign_bit_mask)
        (void)snprintf(buffer, len, "-1.%5ld", ((int32_t)(i.raw & 0x7fff) * 100000) >> 15);
        else
        (void)snprintf(buffer, len, "-0.%5ld", ((int32_t)(i.raw & 0x7fff) * 100000) >> 15);
    }
    else {
        (void)snprintf(buffer, len, "0.%5ld", ((int32_t)(i.raw & 0x7fff) * 100000) >> 15);
    }
    //(void)snprintf(buffer, len, "%1d.%5d", i.raw >> 15, ((int32_t)(i.raw & 0x7fff) * 100000) >> 15);
    return FIX_Ok;
}

// fix9.23
fix9_23 fix9_23_int(int32_t n)  {
    return (fix9_23){.raw = n << FIX9_23_DEC_BITS};
}

fix9_23 fix9_23_from_raw(int32_t raw) {
    return (fix9_23){.raw = raw};
}

fix9_23 fix9_23_frac(int32_t num, int32_t denom) {
    return fix9_23_from_raw(((int64_t)num << FIX9_23_DEC_BITS) / denom);
}


fix9_23 fix9_23_from_f32(float f) {
	const float MULTIPLIER = 8388608.0f;
	return fix9_23_from_raw((int32_t) (f*MULTIPLIER));
}


fix9_23 fix9_23_tau() {
	return fix9_23_from_f32(6.2831853072f);
}


fix9_23 fix9_23_pi() {
	return fix9_23_from_f32(3.141592654f);
}


fix9_23 fix9_23_add(fix9_23 a, fix9_23 b) {
    return (fix9_23){.raw = a.raw + b.raw};
}

fix9_23 fix9_23_sum(int argc, ...) {
 fix9_23 acc = fix9_23_int(0);

 va_list argptr;
 va_start(argptr, argc);

 for (int i = 0; i < argc; i++) {
	 fix9_23 arg = va_arg(argptr,fix9_23);
	 acc = fix9_23_add(acc, arg);
 }

 va_end(argptr);

 return acc;
}

fix9_23 fix9_23_sub(fix9_23 a, fix9_23 b) {
    return (fix9_23){.raw = a.raw - b.raw};
}

fix9_23 fix9_23_mul(fix9_23 a, fix9_23 b) {
    return (fix9_23){.raw = (int32_t)(((int64_t)a.raw * (int64_t)b.raw) >> FIX9_23_DEC_BITS)};
}

fix9_23 fix9_23_div(fix9_23 a, fix9_23 b) {
    return (fix9_23){.raw = (int32_t)((((int64_t)a.raw) << FIX9_23_DEC_BITS) / b.raw)};
}

fix9_23 fix9_23_powi(fix9_23 base, int16_t exponent) {
    if (exponent < 0) return fix9_23_div(fix9_23_int(1), fix9_23_powi(base,-exponent));
    if (exponent == 0) return fix9_23_int(1);
    if (exponent == 1) return base;

    if ((exponent & 0b1) == 0) {
        const fix9_23 tmp = fix9_23_powi(base, exponent / 2);
        return fix9_23_mul(tmp, tmp);
    } else {
        const fix9_23 tmp = fix9_23_powi(base, (exponent - 1) / 2);
        return fix9_23_mul(fix9_23_mul(base, tmp), tmp);
    }
}

TEST_CASE(Unity) {
	ASSERT_EQUAL(1,1);
}

fix9_23 fix9_23_sqrt(fix9_23 x) {
    const int NUMBER_GUESSES = 8;

    const fix9_23 initial_guess = fix9_23_from_raw(x.raw >> 1);

    // x = root^2
    // f: x -> k - x^2
    // f': x -> - 2x
    fix9_23 root = initial_guess;
    for(int i = 0; i < NUMBER_GUESSES; i++) {
        fix9_23 delta = fix9_23_div(fix9_23_sub(x,fix9_23_mul(root,root)), fix9_23_mul(fix9_23_int(-2), root));
        root = fix9_23_sub(root, delta);
    }

    return root;
}

enum CosineQuadrant {
	CosQuad1 = 0,
	CosQuad2,
	CosQuad3,
	CosQuad4,
};

fix9_23 fix9_23_cos(fix9_23 x) {
	if (x.raw < 0) {
		x.raw = -x.raw;
	}

	const fix9_23 HALF_PI = fix9_23_from_raw(fix9_23_pi().raw / 2);
	const fix9_23 PI = fix9_23_pi();
	const fix9_23 THREE_HALF_PI = fix9_23_from_raw((fix9_23_pi().raw * 3) / 2);
	const fix9_23 TWO_PI = fix9_23_tau();

	// Normalization step
	// TODO: Imprecise bounds.
	x.raw -= fix9_23_mul(fix9_23_tau(), fix9_23_int(x.raw / fix9_23_tau().raw)).raw;

	enum CosineQuadrant xquad;

	if (x.raw < HALF_PI.raw) {
		xquad = CosQuad1;
	} else if (x.raw < PI.raw) {
		xquad = CosQuad2;
	} else if (x.raw < THREE_HALF_PI.raw) {
		xquad = CosQuad3;
	} else {
		xquad = CosQuad4;
	}

	switch(xquad) {
	case CosQuad1:
		break;
	case CosQuad2:
		x.raw -= PI.raw;
		break;
	case CosQuad3:
		x.raw -= PI.raw;
		break;
	case CosQuad4:
		x.raw -= TWO_PI.raw;
		break;
	default:
		__builtin_unreachable();
		break;
	}

	const fix9_23 x2 = fix9_23_mul(x,x);
	const fix9_23 x4 = fix9_23_mul(x2,x2);
	const fix9_23 x6 = fix9_23_mul(x2,x4);

	const fix9_23 coeff1 = fix9_23_int(1);
	const fix9_23 coeff2 = fix9_23_frac(-1,2);
	const fix9_23 coeff4 = fix9_23_frac(1,24);
	const fix9_23 coeff6 = fix9_23_frac(-1,720);

	fix9_23 sum = fix9_23_sum(4,
		coeff1,
		fix9_23_mul(x2, coeff2),
		fix9_23_mul(x4, coeff4),
		fix9_23_mul(x6, coeff6)
	);

	switch (xquad) {
	case CosQuad1:
	case CosQuad4:
		return sum;
		break;
	case CosQuad2:
	case CosQuad3:
		return fix9_23_from_raw(-sum.raw);
		break;
	default:
		__builtin_unreachable();
		break;
	}
}

fix9_23 fix9_23_sin(fix9_23 x) {
	const fix9_23 HALF_PI = fix9_23_from_raw(fix9_23_pi().raw / 2);
	return fix9_23_cos(fix9_23_sub(x, HALF_PI));
}

enum FixPointResult fix9_23_format(fix9_23 i, char* buffer, size_t len) {
    // TODO error handling
    //if (len < )
    const int32_t DECIMAL_MASK = 8388607L;
    //const int32_t sign_bit_mask = 8388608L;

    if (i.raw < 0) {
        i.raw = ~i.raw + 1;
        (void)snprintf(buffer, len, "-%ld.%07lld", i.raw >> FIX9_23_DEC_BITS,  ((int64_t)(i.raw & DECIMAL_MASK) * 10000000LL) >> FIX9_23_DEC_BITS);
    }
    else {
        (void)snprintf(buffer, len, "%ld.%07lld",i.raw >> FIX9_23_DEC_BITS, ((int64_t)(i.raw & DECIMAL_MASK) * 10000000LL) >> FIX9_23_DEC_BITS);
    }
    //(void)snprintf(buffer, len, "%1d.%5d", i.raw >> FIX9_23_DEC_BITS, ((int32_t)(i.raw & DECIMAL_MASK) * 100000) >> FIX9_23_DEC_BITS);
    return FIX_Ok;
}
