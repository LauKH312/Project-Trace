/*
 * fixpoint.h
 *
 *  Created on: 27 Apr 2026
 *      Author: lauri
 */

#ifndef INC_FIXPOINT_H_
#define INC_FIXPOINT_H_

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define FIX16_16_INT_BITS 16
#define FIX16_16_DEC_BITS 16

#define FIX1_15_INT_BITS 1
#define FIX1_15_DEC_BITS 15

#define FIX9_23_INT_BITS 9LL
#define FIX9_23_DEC_BITS 23LL

//! All fix1616 stuff is derived from the final project submitted by Laurits K. Hillemann and Alexander N. Olsen for the earlier DTU course 'Programming Project' (30010)
/// Archimedes' constant - π
#define FIX16_PI fix1616_frac(355,113)
#define FIX_INT(x, decimal_bits) ((x) << (decimal_bits))
#define FIX_RATIONAL(a, b, decimal_bits) ((FIX_INT(a, decimal_bits) << (decimal_bits)) / (FIX_INT(b, decimal_bits)))

/// Fixed point scalar in the format 16.16
typedef int32_t fix1616;

/// @brief Creates a 16.16 fixed point representation of the integer `x`
fix1616 fix1616_int(int16_t x);

/// @brief Creates a 16.16 fixed point approximation of the fraction `a/b`
/// @return The fraction `a/b` as a `fix1616`
fix1616 fix1616_frac(int16_t a, int16_t b);


/// @brief Multiplies two fixed point numbers `a` and `b`
/// @return The product of `a` and `b`
fix1616 fix1616_mul(fix1616 a, fix1616 b);

/// @brief Divides two fixed point numbers `a` and `b`
/// @return The quotient of `a` and `b`
fix1616 fix1616_div(fix1616 a, fix1616 b);

/// @brief Computes the square of `x`
fix1616 fix1616_sqr(fix1616 x);

/// @brief Computes the square root of `x`. Assumes `x` is no greater then 360.
fix1616 fix1616_sqrt(fix1616 x);

/// @brief Computes the inverse square root of `x`
fix1616 fix1616_isqrt(fix1616 x);


/// @brief Prints the fixed point number `x` to stdout.
void fix1616_print(fix1616 x);

/// @brief Rounds the fixed point number `x` to the nearest integer.
int16_t fix1616_round(fix1616 x);

/// @brief Truncates the decimal part of `x`
/// @return The integer part of `x`
int16_t fix1616_trunc(fix1616 x);

/// @brief Interpolates linearly between `a` and `b`
/// @param a Start value
/// @param b End value
/// @param t Time variable in the range [0-1] (inclusive)
fix1616 fix1616_lerp(fix1616 a, fix1616 b, fix1616 t);


struct fix1_15
{
    int16_t raw;
};
typedef struct fix1_15 fix1_15;

enum FixPointResult {
    FIX_Ok = 0,
    FIX_FormatBufferTooSmall,
};

fix1_15 fix1_15_int(int16_t n);
fix1_15 fix1_15_from_raw(int16_t raw);
fix1_15 fix1_15_frac(int16_t num, int16_t denom);

fix1_15 fix1_15_add(fix1_15 a, fix1_15 b);
fix1_15 fix1_15_sub(fix1_15 a, fix1_15 b);
fix1_15 fix1_15_mul(fix1_15 a, fix1_15 b);
fix1_15 fix1_15_div(fix1_15 a, fix1_15 b);

fix1_15 fix1_15_powi(fix1_15 base, int16_t exponent);
fix1_15 fix1_15_sqrt(fix1_15 x);

enum FixPointResult fix1_15_format(fix1_15 i, char* buffer, size_t len);

struct fix9_23
{
    int32_t raw;
};
typedef struct fix9_23 fix9_23;


/**
 * @brief Creates fix9.23 number from the integer `n`.
 * 
 * @param n 
 * @return fix9_23 
 */
fix9_23 fix9_23_int(int32_t n);

/**
 * @brief Creates fix9.23 number from its raw internal representation.
 * 
 * @param raw 
 * @return fix9_23 
 */
fix9_23 fix9_23_from_raw(int32_t raw);

/**
 * @brief Creates a fix9.23 number from a fraction.
 * 
 * @param num 
 * @param denom 
 * @return fix9_23 
 */
fix9_23 fix9_23_frac(int32_t num, int32_t denom);

/**
 * @brief Creates a fix9.23 number from a 32-bit floating point number.
 * 
 * @param f 
 * @return fix9_23 
 */
fix9_23 fix9_23_from_f32(float f);

/**
 * @brief Returns tau = 2*pi
 * 
 * @return fix9_23 
 */
fix9_23 fix9_23_tau();

/**
 * @brief Returns pi
 * 
 * @return fix9_23 
 */
fix9_23 fix9_23_pi();

/**
 * @brief Returns the absolute value of `x`
 * 
 * @param x 
 * @return fix9_23 
 */
fix9_23 fix9_23_abs(fix9_23 x);

/**
 * @brief Computes `a + b`
 * 
 * @param a 
 * @param b 
 * @return fix9_23 
 */
fix9_23 fix9_23_add(fix9_23 a, fix9_23 b);

/**
 * @brief Returns the sum of an arbitrary amount of fix9.23 arguments
 * 
 * @param argc Number of arguments
 * @param ... 
 * @return fix9_23 
 */
fix9_23 fix9_23_sum(int argc, ...);

/**
 * @brief Computes `a - b`
 * 
 * @param a 
 * @param b 
 * @return fix9_23 
 */
fix9_23 fix9_23_sub(fix9_23 a, fix9_23 b);

/**
 * @brief Computes the negation of `a`, i.e. `-a`
 * 
 * @param a 
 * @return fix9_23 
 */
fix9_23 fix9_23_neg(fix9_23 a);

/**
 * @brief Computes `a * b`
 * 
 * @param a 
 * @param b 
 * @return fix9_23 
 */
fix9_23 fix9_23_mul(fix9_23 a, fix9_23 b);

/**
 * @brief Computes `a * b`, where `b` is an integer.
 * 
 * @param a 
 * @param b 
 * @return fix9_23 
 */
fix9_23 fix9_23_mul_int(fix9_23 a, int32_t b);

/**
 * @brief Computes `a / b`
 * 
 * @param a 
 * @param b 
 * @return fix9_23 
 */
fix9_23 fix9_23_div(fix9_23 a, fix9_23 b);

/**
 * @brief Computes `a / b`, where `b` is an integer
 * 
 * @param a 
 * @param b 
 * @return fix9_23 
 */
fix9_23 fix9_23_div_int(fix9_23 a, int32_t b);

/**
 * @brief The integer power function. Computes `base^exponent`, where `exponent` is an integer.
 * 
 * @param base 
 * @param exponent 
 * @return fix9_23 
 */
fix9_23 fix9_23_powi(fix9_23 base, int16_t exponent);

/**
 * @brief Computes the square root of `x`
 * 
 * @param x 
 * @return fix9_23 
 */
fix9_23 fix9_23_sqrt(fix9_23 x);

/**
 * @brief Computes the square of `x`, i.e. `x * x`
 * 
 * @param x 
 * @return fix9_23 
 */
fix9_23 fix9_23_sqr(fix9_23 x);

/**
 * @brief Computes `cos(x)`, where `x` is given in radians.
 * 
 * @param x The phase in radians
 * @return fix9_23 
 */
fix9_23 fix9_23_cos(fix9_23 x);

/**
 * @brief Computes `sin(x)`, where `x` is given in radians.
 * 
 * @param x 
 * @return fix9_23 
 */
fix9_23 fix9_23_sin(fix9_23 x);

/**
 * @brief Computes the real exponential function of x, `exp(x)=e^x`
 * 
 * @param x 
 * @return fix9_23 
 */
fix9_23 fix9_23_exp(fix9_23 x);

/**
 * @brief Checks if `a` and `b` are equal within the tolerance `tol`
 * 
 * @param a 
 * @param b 
 * @param tol
 * @return fix9_23 
 */
_Bool fix9_23_approx_eq(fix9_23 a, fix9_23 b, fix9_23 tol);

/**
 * @brief Formats the fix9.23 number to `buffer`. Returns the length of the formatted string.
 * 
 * @param x 
 * @param buffer 
 * @param len 
 * @return `int` The number of bytes written to buffer
 */
int fix9_23_format(fix9_23 x, char* buffer, size_t len);


#endif /* INC_FIXPOINT_H_ */
