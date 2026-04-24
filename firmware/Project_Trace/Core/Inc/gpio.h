/*
 * gpio_set.h
 *
 *  Created on: 20 Apr 2026
 *      Author: Marius H. Tyson
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_
#include <main.h>

// TODO: Review this is almost certainly wrong in some way first lines for the project
// TODO: There is a lot of repeated code perhaps replace with functions
void set_GPIOx_MODER(char x, int bit, char mode);
void set_GPIOx_OTYPER(char x, int bit, char mode);
void set_GPIOx_OSPEEDR(char x, int bit, char mode);
void set_GPIOx_PUPDR(char x, int bit, char mode);


#endif /* INC_GPIO_H_ */
