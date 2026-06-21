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

#include <math.h>

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

/*
fix1_15 fix1_15_from_raw(int16_t raw) {
    return (fix1_15){.raw = raw};
}
*/

fix1_15 fix1_15_from_f9_23(fix9_23 x) {
    return FIX1_15_RAW((int16_t)(x.raw >> (FIX9_23_DEC_BITS - FIX1_15_DEC_BITS)));
}

fix1_15 fix1_15_frac(int16_t num, int16_t denom) {
    return FIX1_15_RAW(((int32_t)num << FIX1_15_DEC_BITS) / denom);
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

    const fix1_15 initial_guess = FIX1_15_RAW(x.raw >> 1);

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

int fix1_15_format(fix1_15 i, char* buffer, size_t len) {
    if (i.raw < 0) {
        const int16_t sign_bit_mask = 0x8000;
        i.raw = ~i.raw + 1;
        if (i.raw & sign_bit_mask)
        return snprintf(buffer, len, "-1.%5ld", ((int32_t)(i.raw & 0x7fff) * 100000) >> 15);
        else
        return snprintf(buffer, len, "-0.%5ld", ((int32_t)(i.raw & 0x7fff) * 100000) >> 15);
    }
    else {
        return snprintf(buffer, len, "0.%5ld", ((int32_t)(i.raw & 0x7fff) * 100000) >> 15);
    }
}

// fix9.23
fix9_23 fix9_23_int(int32_t n)  {
    return (fix9_23){.raw = n << FIX9_23_DEC_BITS};
}

/*
fix9_23 fix9_23_from_raw(int32_t raw) {
    //return (fix9_23){.raw = raw};
	return FIX9_23_RAW(raw);
}
*/

fix9_23 fix9_23_frac(int32_t num, int32_t denom) {
    return FIX9_23_RAW(((int64_t)num << FIX9_23_DEC_BITS) / denom);
}


fix9_23 fix9_23_from_f32(float f) {
//	const float MULTIPLIER = 8388608.0f;
	const float MULTIPLIER = 1 << FIX9_23_DEC_BITS;
	return FIX9_23_RAW((int32_t)(f*MULTIPLIER));
}

float fix9_23_to_f32(fix9_23 f) {
	return (float) f.raw / (float) (1 << 23);
}

fix9_23 fix9_23_from_f1_15(fix1_15 x) {
    return FIX9_23_RAW((int32_t)(x.raw) << (FIX9_23_DEC_BITS - FIX1_15_DEC_BITS));
}

fix9_23 fix9_23_abs(fix9_23 x) {
    if(x.raw < 0) 
        return fix9_23_neg(x);
    else
        return x;
}

fix9_23 fix9_23_add(fix9_23 a, fix9_23 b) {
    return FIX9_23_RAW(a.raw + b.raw);
}

fix9_23 fix9_23_sum(int argc, ...) {
 fix9_23 acc = FIX9_23_ZERO;

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
	return FIX9_23_RAW(a.raw - b.raw);
}

fix9_23 fix9_23_neg(fix9_23 a) {
    return FIX9_23_RAW(-a.raw);
}

fix9_23 fix9_23_mul(fix9_23 a, fix9_23 b) {
    return (fix9_23){.raw = (int32_t)(((int64_t)a.raw * (int64_t)b.raw) >> FIX9_23_DEC_BITS)};
}


fix9_23 fix9_23_mul_int(fix9_23 a, int32_t b) {
	return (fix9_23){.raw = a.raw * b};
}

fix9_23 fix9_23_div(fix9_23 a, fix9_23 b) {
    return (fix9_23){.raw = (int32_t)((((int64_t)a.raw) << FIX9_23_DEC_BITS) / b.raw)};
}


fix9_23 fix9_23_div_int(fix9_23 a, int32_t b) {
	return (fix9_23){.raw = a.raw / b};
}

#define POWI_STACK_SIZE 32
#define POWI_MAX_ITER 128

size_t max_lesser_idx(int16_t* exponent_buf, size_t exponent_buf_len, int16_t exponent) {
    int16_t max_exponent = 0;
    for (size_t i = 0; i < exponent_buf_len; i++) {
        if (exponent_buf[i] < exponent) max_exponent = exponent_buf[i]; 
    }
    return max_exponent;
}

fix9_23 fix9_23_powi(fix9_23 base, int16_t exponent) {
    if (exponent < 0) return fix9_23_div(fix9_23_int(1), fix9_23_powi(base,-exponent));
    if (exponent == 0) return fix9_23_int(1);
    if (exponent == 1) return base;

    fix9_23 acc = fix9_23_int(1);
    //exponent--;

    for(int32_t i = 0; i < POWI_MAX_ITER && exponent > 0; i++) {
    	if (exponent == 1) return fix9_23_mul(acc,base);

    	if (exponent % 2 == 0) {
    		//acc = fix9_23_mul(acc,base);
    		base = fix9_23_sqr(base);
    		exponent /= 2;
    	} else {
    		acc = fix9_23_mul(acc, base);
    		base = fix9_23_sqr(base);
    		exponent = (exponent-1)/2;
    		//acc = fix9_23_mul(acc, base);
    		//exponent--;
    	}
    }

    return acc;
}

fix9_23 fix9_23_powf(fix9_23 base, fix9_23 exponent) {
	return fix9_23_exp(fix9_23_mul(exponent, fix9_23_ln(base)));
}

/*
fix9_23 fix9_23_powi(fix9_23 base, int16_t exponent) {
    if (exponent < 0) return fix9_23_div(fix9_23_int(1), fix9_23_powi(base,-exponent));
    if (exponent == 0) return fix9_23_int(1);
    if (exponent == 1) return base;

    // TODO: use exponentiation by squaring?

    fix9_23 powstack[POWI_STACK_SIZE] = {0};
    int16_t exponent_stack[POWI_STACK_SIZE] = {0};
    size_t topidx = 0;

    powstack[topidx] = fix9_23_int(1);
    exponent_stack[topidx] = 0;
    topidx++;

    powstack[topidx] = base;
    exponent_stack[topidx] = 1;
    topidx++;
    
    fix9_23 acc = base;
    int16_t pow2 = 1;

    for (int i = 0; i < POWI_MAX_ITER; i++) {
        if (pow2*2 >= exponent) break;

        acc = fix9_23_mul(acc,acc);
        pow2 *= 2;
        powstack[topidx] = acc;
        exponent_stack[topidx] = pow2;
        topidx++;
    }
    exponent -= pow2;

    for (int i = 0; i < POWI_MAX_ITER; i++) {
        if (exponent <= 1) break;
        int16_t powidx = max_lesser_idx(exponent_stack, topidx, exponent);
        acc = fix9_23_mul(acc, powstack[powidx]);
        exponent -= exponent_stack[powidx];
    }
    if (exponent == 1) acc = fix9_23_mul(acc, base);
    return acc;
}
*/

TEST_CASE(fix9_23_powi_test) {
    fix9_23 x = fix9_23_powi(fix9_23_int(2), 3);
    ASSERT_TRUE(x.raw == fix9_23_int(8).raw);

    x = fix9_23_powi(fix9_23_frac(1,4), 3);
    ASSERT_TRUE(x.raw == fix9_23_frac(1,64).raw);

    x = fix9_23_powi(fix9_23_int(2), -3);
    ASSERT_TRUE(x.raw == fix9_23_frac(1,8).raw);
}


// fix9_23 fix9_23_powi(fix9_23 base, int16_t exponent) {
//     if (exponent < 0) return fix9_23_div(fix9_23_int(1), fix9_23_powi(base,-exponent));
//     if (exponent == 0) return fix9_23_int(1);
//     if (exponent == 1) return base;

//     if ((exponent & 0b1) == 0) {
//         const fix9_23 tmp = fix9_23_powi(base, exponent / 2);
//         return fix9_23_mul(tmp, tmp);
//     } else {
//         const fix9_23 tmp = fix9_23_powi(base, (exponent - 1) / 2);
//         return fix9_23_mul(fix9_23_mul(base, tmp), tmp);
//     }
// }

TEST_CASE(Unity) {
	ASSERT_EQUAL(1,1);
}

/*
fix9_23 fix9_23_sqrt(fix9_23 x) {
    const int NUMBER_GUESSES = 8;

    const fix9_23 initial_guess = FIX9_23_RAW(x.raw >> 1);
    const fix9_23 TWENTY_FIVE = fix9_23_int(25);


    if (x.raw > TWENTY_FIVE.raw) {
    	const fix9_23 DIV_FACTOR = fix9_23_div(x, TWENTY_FIVE);

    	return fix9_23_mul(fix9_23_sqrt(DIV_FACTOR), fix9_23_sqrt(TWENTY_FIVE));
    }
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
*/

fix9_23 fix9_23_sqrt(fix9_23 x) {
	float xsqrt = sqrt(x.raw);
	float decimal_mult_sqrt = 2896.3093757400986599458585071815f; // sqrt(2^23).
	x.raw = (int32_t)(decimal_mult_sqrt * xsqrt);
	return x;
}

fix9_23 fix9_23_sqr(fix9_23 x) {
	return fix9_23_mul(x,x);
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

	const fix9_23 HALF_PI = FIX9_23_RAW(FIX9_23_PI.raw / 2);
	const fix9_23 PI = FIX9_23_PI;
	const fix9_23 THREE_HALF_PI = FIX9_23_RAW((FIX9_23_PI.raw * 3) / 2);
	const fix9_23 TWO_PI = FIX9_23_TAU;

	// Normalization step
	// TODO: Imprecise bounds.
	x.raw -= fix9_23_mul(FIX9_23_TAU, fix9_23_int(x.raw / FIX9_23_TAU.raw)).raw;

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
	const fix9_23 x8 = fix9_23_mul(x4,x4);
	const fix9_23 x10 = fix9_23_mul(x8,x2);

	const fix9_23 coeff1 = fix9_23_int(1);
	const fix9_23 coeff2 = fix9_23_frac(-1,2);
	const fix9_23 coeff4 = fix9_23_frac(1,24);
	const fix9_23 coeff6 = fix9_23_frac(-1,720);
	const fix9_23 coeff8 = fix9_23_frac(1,720*7*8);
	const fix9_23 coeff10 = fix9_23_frac(-1,720*7*8*9*10);


	// fix9_23 sum = fix9_23_sum(5,
	// 	coeff1,
	// 	fix9_23_mul(x2, coeff2),
	// 	fix9_23_mul(x4, coeff4),
	// 	fix9_23_mul(x6, coeff6),
	// 	fix9_23_mul(x8, coeff8)
	// );

	const fix9_23 sum = fix9_23_sum(6,
		coeff1,
		fix9_23_mul(x2, coeff2),
		fix9_23_mul(x4, coeff4),
		fix9_23_mul(x6, coeff6),
		fix9_23_mul(x8, coeff8),
		fix9_23_mul(x10, coeff10)
	);

	switch (xquad) {
	case CosQuad1:
	case CosQuad4:
		return (sum);
		break;
	case CosQuad2:
	case CosQuad3:
		return FIX9_23_RAW(-sum.raw);
		break;
	default:
		__builtin_unreachable();
		break;
	}
}

fix9_23 fix9_23_sin(fix9_23 x) {
	const fix9_23 HALF_PI = fix9_23_div_int(FIX9_23_PI,  2);
	return fix9_23_cos(fix9_23_sub(x, HALF_PI));
}

_Bool fix9_23_approx_eq(fix9_23 a, fix9_23 b, fix9_23 tol) {
    if (tol.raw == 0) tol.raw = 1;
    const fix9_23 diff = fix9_23_sub(a,b);
    return fix9_23_abs(diff).raw < tol.raw;
}

int32_t fix9_23_trunc(fix9_23 x) {
    return x.raw >> FIX9_23_DEC_BITS;
}

/**
 * @brief Returns the integer part of x.
 */
int32_t fix9_23_round(fix9_23 x) {
	const fix9_23 HALF = fix9_23_frac(1,2);

	if (x.raw < 0) {
		return fix9_23_trunc(fix9_23_sub(x, HALF));
	} else {
		return fix9_23_trunc(fix9_23_add(x, HALF));
	}
}



const int32_t table_zero_idx = 16;
const fix9_23 EXP_LUT[32] = {
FIX9_23_RAW(1),FIX9_23_RAW(3),FIX9_23_RAW(7),FIX9_23_RAW(19),FIX9_23_RAW(52),FIX9_23_RAW(140),FIX9_23_RAW(381),FIX9_23_RAW(1035),FIX9_23_RAW(2814),FIX9_23_RAW(7649),FIX9_23_RAW(20793),FIX9_23_RAW(56522),FIX9_23_RAW(153643),FIX9_23_RAW(417644),FIX9_23_RAW(1135275),FIX9_23_RAW(3085996),FIX9_23_RAW(8388608),FIX9_23_RAW(22802601),FIX9_23_RAW(61983895),FIX9_23_RAW(168489696),FIX9_23_RAW(458002478),FIX9_23_RAW(1244979814), // FIX9_23_RAW(3384206005), // FIX9_23_RAW(9199225686),FIX9_23_RAW(25006088018),FIX9_23_RAW(67973594660),FIX9_23_RAW(184771387178),FIX9_23_RAW(502260704185),FIX9_23_RAW(1365286145336),FIX9_23_RAW(3711232519513),FIX9_23_RAW(10088175918979),FIX9_23_RAW(27422505282859),
};

// Retrieved from https://github.com/nadavrot/fast_log.
// Rewritten to use fix9_23 fixed-precision decimals.
fix9_23 fix9_23_exp(fix9_23 x) {
    const int32_t integer = fix9_23_trunc(x);
    x = fix9_23_sub(x, fix9_23_int(integer));

    //const fix9_23 coeffs[4] = {FIX9_23_RAW(2351638), FIX9_23_RAW(3567692), FIX9_23_RAW(8495449), FIX9_23_RAW(8390365)};
    //fix9_23 acc = fix9_23_add(coeffs[1], fix9_23_mul(x,coeffs[0]));
    //acc = fix9_23_add(coeffs[2], fix9_23_mul(x, acc));
    //acc = fix9_23_add(coeffs[3], fix9_23_mul(x, acc));
    //return fix9_23_mul(acc, EXP_LUT[integer + table_zero_idx]);


    const fix9_23 coeffs[5] = {FIX9_23_RAW(585781), FIX9_23_RAW(1180076), FIX9_23_RAW(4265307), FIX9_23_RAW(8383615), FIX9_23_RAW(8388241)};
    fix9_23 acc = fix9_23_add(coeffs[1], fix9_23_mul(x,coeffs[0]));
    acc = fix9_23_add(coeffs[2], fix9_23_mul(x, acc));
    acc = fix9_23_add(coeffs[3], fix9_23_mul(x, acc));
    acc = fix9_23_add(coeffs[4], fix9_23_mul(x, acc));
    return fix9_23_mul(acc, EXP_LUT[integer + table_zero_idx]);

}

/*
// TODO: improve accuracy
fix9_23 fix9_23_exp(fix9_23 x) {

    const int32_t DEGREE = 7;
    const int32_t FACT[8] = { 1, 1, 2, 6, 24, 5*24, 6*5*24, 7*6*5*24 };

    const fix9_23 x0 = fix9_23_frac(-8,1);
    const fix9_23 x1 = fix9_23_frac(-31,10);
    const fix9_23 x2 = FIX9_23_ZERO;
    const fix9_23 x3 = fix9_23_frac(21,10);
    
    const fix9_23 bound0 = fix9_23_frac(-49,10);
    const fix9_23 bound1 = fix9_23_frac(-129,100);
    const fix9_23 bound2 = fix9_23_frac(117,100);

    const fix9_23 expx0 = fix9_23_frac(335, 1000000);
    const fix9_23 expx1 = fix9_23_frac(450, 10000);
    const fix9_23 expx2 = fix9_23_int(1);
    const fix9_23 expx3 = fix9_23_frac(817, 100);

    if (x.raw < bound0.raw) {
        // Taylor expansion @x0
        const fix9_23 diff = fix9_23_sub(x, x0);
        fix9_23 accum = expx0;
        fix9_23 diff_accum = diff;
        for (int i = 1; i <= DEGREE; i++) {
            const fix9_23 term = fix9_23_div_int(fix9_23_mul(expx0, diff_accum), FACT[i]);
            accum.raw += term.raw;
            diff_accum = fix9_23_mul(diff_accum, diff_accum);
        }
        return accum;
    } else if (x.raw < bound1.raw) {
        // Taylor expansion @x1
        const fix9_23 diff = fix9_23_sub(x, x1);
        fix9_23 accum = expx1;
        fix9_23 diff_accum = diff;
        for (int i = 1; i <= DEGREE; i++) {
            const fix9_23 term = fix9_23_div_int(fix9_23_mul(expx1, diff_accum), FACT[i]);
            accum.raw += term.raw;
            diff_accum = fix9_23_mul(diff_accum, diff_accum);
        }
        return accum;
    } else if (x.raw < bound2.raw) {
        // Taylor expansion @x2
        const fix9_23 diff = fix9_23_sub(x, x2);
        fix9_23 accum = expx2;
        fix9_23 diff_accum = diff;
        for (int i = 1; i <= DEGREE; i++) {
            const fix9_23 term = fix9_23_div_int(fix9_23_mul(expx2, diff_accum), FACT[i]);
            accum.raw += term.raw;
            diff_accum = fix9_23_mul(diff_accum, diff_accum);
        }
        return accum;
    } else {
        // Taylor expansion @x3
        const fix9_23 diff = fix9_23_sub(x, x3);
        fix9_23 accum = expx3;
        fix9_23 diff_accum = diff;
        for (int i = 1; i <= DEGREE; i++) {
            const fix9_23 term = fix9_23_div_int(fix9_23_mul(expx3, diff_accum), FACT[i]);
            accum.raw += term.raw;
            diff_accum = fix9_23_mul(diff_accum, diff_accum);
        }
        return accum;
    }
}
*/

const int32_t x0lut[32] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648};
const fix9_23 ln2 = FIX9_23_RAW(5814540);
// const fix9_23 ln2_recip = FIX9_23_RAW(12102203);
// const fix9_23 log2_10 = FIX9_23_RAW(27866353);
// const fix9_23 log2_e = FIX9_23_RAW(12102203);

int32_t fastlog2i(int32_t x) {
    return 31 - __builtin_clzl(x);
}

// Hand derived
/*fix9_23 fix9_23_log2(fix9_23 x) {
    const fix9_23 l2x = FIX9_23_RAW(fastlog2i(x.raw));
    const fix9_23 x0 = FIX9_23_RAW(x0lut[l2x.raw]);
    
    // const fix9_23 x0_recip = fix9_23_div(fix9_23_int(1), x0);
    // const fix9_23 dx0 = 1/ln(2) * 1/x0;
    const fix9_23 dx0 = fix9_23_div(fix9_23_int(1),fix9_23_mul(ln2, x0));

    // Newton Raphson Method on 2^l = x
    const fix9_23 deltax = fix9_23_sub(x, x0);

    const fix9_23 guess = fix9_23_add(l2x, fix9_23_mul(dx0, deltax));

    const fix9_23 newtexp = fix9_23_exp(fix9_23_mul(guess, ln2));
    const fix9_23 it1 = fix9_23_add(guess, fix9_23_div(fix9_23_sub(newtexp,x), fix9_23_mul(newtexp, ln2)));

    return fix9_23_sub(it1, fix9_23_int(FIX9_23_DEC_BITS));
}*/

#define LOG2_NCOEFFS 5

fix9_23 fix9_23_log2(fix9_23 x) {
//	const int32_t int_part = fastlog2i(fix9_23_truncate(x));
	const int32_t int_part = fastlog2i(x.raw) - FIX9_23_DEC_BITS;
	const fix9_23 x0 = FIX9_23_RAW(x0lut[int_part + FIX9_23_DEC_BITS]);
	const fix9_23 dx = fix9_23_sub(x, x0);

	const int32_t DX_SCALE_FACTOR = 8; // Done to increase numerical accuracy.

	const fix9_23 dx_scaled = fix9_23_mul_int(dx, DX_SCALE_FACTOR);

	if (dx.raw < fix9_23_frac(255,DX_SCALE_FACTOR).raw)
		x = fix9_23_add(fix9_23_div_int(fix9_23_div(dx_scaled, x0), DX_SCALE_FACTOR), fix9_23_int(1)); // Remove Integer part of log.
	else
		x = fix9_23_add(fix9_23_div(dx, x0), fix9_23_int(1));
	assert(x.raw > 0);


	//const fix9_23 COEFFS[LOG2_NCOEFFS] = {  FIX9_23_RAW(1333044), FIX9_23_RAW(-8826379), FIX9_23_RAW(25538082), FIX9_23_RAW(-18036197) };

	// WORKS
	//const fix9_23 COEFFS[LOG2_NCOEFFS] = { fix9_23_from_f32(0.15891118), fix9_23_from_f32(-1.0521864), fix9_23_from_f32(3.04437661), fix9_23_from_f32(-2.15008228)};
	const fix9_23 COEFFS[LOG2_NCOEFFS] = { FIX9_23_RAW(-688806), FIX9_23_RAW(5425023), FIX9_23_RAW(-17792409), FIX9_23_RAW(34119948), FIX9_23_RAW(-21062541)};

	//const int32_t COEFFS[LOG2_NCOEFFS] = { -688806 * M, 5425023 * M, -17792409 * M, 34119948 * M, -21062541 * M};

	//char fmtbuf[64] = {0};
	//fix9_23_format(x0, fmtbuf, 64);
	//printf("[%s]\n", fmtbuf);

	//int32_t acc = COEFFS[0];
	fix9_23 acc = COEFFS[0];
	for (int32_t i = 1; i < LOG2_NCOEFFS; i++) {
		acc = fix9_23_add(COEFFS[i], fix9_23_mul(acc, x));
		//acc = COEFFS[i] + FIX_MUL32(acc, _x, DEC_BITS);
	}
	//return fix9_23_add(fix9_23_int(int_part), FIX9_23_RAW(acc >> EXTRA_BITS));
	return fix9_23_add(fix9_23_int(int_part), acc);
}

fix9_23 fix9_23_ln(fix9_23 x) {
	return fix9_23_div(fix9_23_log2(x), FIX9_23_LOG2E);
}

fix9_23 fix9_23_lerp(fix9_23 t, fix9_23 a, fix9_23 b) {
	fix9_23 delta = b;
	delta.raw -= a.raw;
	return fix9_23_add(a, fix9_23_mul(delta,t));
}

fix9_23 fix9_23_ilerp(fix9_23 v, fix9_23 a, fix9_23 b) {
	const fix9_23 num = fix9_23_sub(v, a);
	const fix9_23 denom = fix9_23_sub(b,a);

	return fix9_23_div(num,denom);
}

fix9_23 fix9_23_map(fix9_23 v, fix9_23 from_a, fix9_23 from_b, fix9_23 to_a, fix9_23 to_b) {
	return fix9_23_lerp(fix9_23_ilerp(v,from_a,from_b), to_a, to_b);
}

fix9_23 fix9_23_gain_to_db(fix9_23 g) {
	const fix9_23 log2_of_10 = fix9_23_log2(fix9_23_int(10));
	const fix9_23 frac = fix9_23_div(fix9_23_log2(g), log2_of_10);

	return fix9_23_mul(fix9_23_int(20), frac);
}


fix9_23 fix9_23_blackman_harris(int32_t x, int32_t T) {
	// Retrieved from https://ccrma.stanford.edu/~jos/sasp/Blackman_Harris_Window_Family.html
	const fix9_23 COEFFS[3] = {FIX9_23_RAW(3559958), FIX9_23_RAW(4171995),  FIX9_23_RAW(656654)};

	const fix9_23 OmegaM = fix9_23_div_int(FIX9_23_TAU, T);
	const fix9_23 t2 = fix9_23_cos(fix9_23_mul_int(OmegaM, x));
	const fix9_23 t3 = fix9_23_cos(fix9_23_mul_int(OmegaM, x*2));

	// bharris = a0 + a1 * np.cos(OmegaM * x) + a2 * np.cos(2*OmegaM*x)
	return fix9_23_sum(4,
			COEFFS[0],
			fix9_23_mul(COEFFS[1], t2),
			fix9_23_mul(COEFFS[2], t3),
			fix9_23_int(-1)
	);
}

int fix9_23_format(fix9_23 x, char* buffer, size_t len) {
    const int64_t DECIMAL_MASK = 8388607L;
    const int64_t DECIMAL_MULTIPLIER = 10000000LL;

    if (x.raw < 0) {
        x.raw = ~x.raw + 1;
        return snprintf(buffer, len, "-%ld.%07ld", x.raw >> FIX9_23_DEC_BITS,  (int32_t)((((int64_t)x.raw & DECIMAL_MASK) * DECIMAL_MULTIPLIER) >> (int64_t)FIX9_23_DEC_BITS));
    }
    else {
        return snprintf(buffer, len, "%ld.%07ld", x.raw >> FIX9_23_DEC_BITS, (int32_t)((((int64_t)x.raw & DECIMAL_MASK) * DECIMAL_MULTIPLIER) >> (int64_t)FIX9_23_DEC_BITS));
    }
}
