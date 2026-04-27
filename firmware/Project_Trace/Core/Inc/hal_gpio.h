/*
 * gpio_set.h
 *
 *  Created on: 20 Apr 2026
 *      Author: Marius H. Tyson
 */

#ifndef INC_HAL_GPIO_H_
#define INC_HAL_GPIO_H_
#include <main.h>

// TODO: Review this is almost certainly wrong in some way first lines for the project
// TODO: There is a lot of repeated code perhaps replace with functions

enum gpio_GPIObank{
	  GPIO_A = 0,
	  GPIO_B = 1,
	  GPIO_C = 2,
	  GPIO_D = 3,
	  GPIO_E = 4,
	  GPIO_F = 5,
	  GPIO_G = 6,
	  GPIO_H = 7,
};

enum gpio_MODER{
	input = 0,
	output = 1,
	alternative = 2,
	analog = 3
};

enum gpio_pushpullactive{
	pushpull = 0,
	open_drain = 1
};


enum gpio_speed{
	low = 0,
	med = 1,
	high = 2,
	very_high = 3
};

enum gpio_pullupdown{
	nothing = 0,
	pull_up = 1,
	pull_down = 2,
	pull_updown = 3,
};
enum gpio_clock{
	disable = 0,
	enable = 1,
};

void hal_gpio_enable_clock(enum gpio_GPIObank x, enum gpio_clock state); // MUST RUN FIRST, takes the letter of the bank you wish to use x and the state you wish refer above
void hal_gpio_set_moder(enum gpio_GPIObank x, int pin, enum gpio_MODER mode ); // takes the letter of the bank you wish to use, the pin you need,
void hal_gpio_set_pushpullactive(enum gpio_GPIObank x, int pin,  enum gpio_pushpullactive mode);
void hal_gpio_set_speed(enum gpio_GPIObank x, int pin,  enum gpio_speed mode);
void hal_gpio_set_pullupdown(enum gpio_GPIObank x, int pin, enum gpio_pullupdown mode);





#endif /* INC_HAL_GPIO_H_ */
