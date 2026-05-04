/*
 * complex
 *
 *  Created on: Apr 30, 2026
 *      Author: Laurits
 */

#ifndef INC_COMPLEX_H_
#define INC_COMPLEX_H_

#include <fixpoint.h>

typedef struct
{
    fix9_23 re;
    fix9_23 im;
} Complex9_23;

/**
 * @brief Creates a new complex9.23 from the real part `re` and the imaginary part `im`.
 * 
 * @param re 
 * @param im 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_new(fix9_23 re, fix9_23 im);

/**
 * @brief Returns the complex unit, `i = sqrt(-1)`.
 * 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_i();

/**
 * @brief Returns a complex9.23 from its polar coordinates.
 * 
 * @param mag The distance from the origin in the Gaussian plane.
 * @param angle The angle in radians.
 * @return Complex9_23 
 */
Complex9_23 complex9_23_polar(fix9_23 mag, fix9_23 angle);

/**
 * @brief Computes `z1 + z2`
 * 
 * @param z1 
 * @param z2 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_add(Complex9_23 z1, Complex9_23 z2);


/**
 * @brief Computes `z1 - z2`
 * 
 * @param z1 
 * @param z2 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_sub(Complex9_23 z1, Complex9_23 z2);


/**
 * @brief Computes `z1 * z2`
 * 
 * @param z1 
 * @param z2 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_mul(Complex9_23 z1, Complex9_23 z2);


/**
 * @brief Computes `z1 * x`, where `x` is a real fix9.23 value.
 * 
 * @param z1 
 * @param z2 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_mul_re(Complex9_23 z, fix9_23 x);

/**
 * @brief Computes `z1 * x`, where `x` is an integer.
 * 
 * @param z 
 * @param x 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_mul_int(Complex9_23 z, int32_t x);


/**
 * @brief Computes `z1 / z2`
 * 
 * @param z1 
 * @param z2 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_div(Complex9_23 z1, Complex9_23 z2);

/**
 * @brief Computes `z1 / x`, where `x` is a real fix9.23 value.
 * 
 * @param z1 
 * @param z2 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_div_re(Complex9_23 z, fix9_23 x);

/**
 * @brief Computes `z1 / x`, where `x` is an integer.
 * 
 * @param z 
 * @param x 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_div_int(Complex9_23 z, int32_t x);

/**
 * @brief Computes the complex conjugate of `z`
 * 
 * @param z 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_conj(Complex9_23 z);

/**
 * @brief Computes the square of the modulus of `z`
 * 
 * @param z 
 * @return Complex9_23 
 */
fix9_23 complex9_23_abs_sqr(Complex9_23 z);

/**
 * @brief Computes the modulus of `z`
 * 
 * @param z 
 * @return Complex9_23 
 */
fix9_23 complex9_23_abs(Complex9_23 z);


/**
 * @brief The euler function exp(i*x) = cos(x) + i * sin(x)
 * 
 * @param x 
 * @return Complex9_23 
 */
Complex9_23 complex9_23_euler(fix9_23 x);

#endif /* INC_COMPLEX_H_ */
