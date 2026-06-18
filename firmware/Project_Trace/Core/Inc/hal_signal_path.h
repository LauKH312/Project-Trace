/*
 * hal_signal_path.h
 *
 *  Created on: 27. apr. 2026
 *      Author: Marius H. Tyson
 */

#ifndef INC_HAL_SIGNAL_PATH_H_
#define INC_HAL_SIGNAL_PATH_H_



//#define ATTEN_0DB_BANK GPIOC
//#define ATTEN_0DB_PIN  GPIO_PIN_7
//
//#define ATTEN_20DB_BANK GPIOB
//#define ATTEN_20DB_PIN  GPIO_PIN_13
//
//#define ATTEN_40DB_BANK GPIOB
//#define ATTEN_40DB_PIN  GPIO_PIN_12
//
//#define GAIN_20DB_BANK GPIOC
//#define GAIN_20DB_PIN GPIO_PIN_6
#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include <hal_gpio.h>

enum SignalPathAtten {
	SignalPathAtten_m20db = 0,
	SignalPathAtten_0db,
	SignalPathAtten_p20db,
	SignalPathAtten_p40db
};

void hal_signal_path_attenuator_init(void);

// enum SignalPathAtten {
// 	SignalPathAtten_0db =  0,
// 	SignalPathAtten_20db,
// 	SignalPathAtten_40db
// };

// enum SignalPathGain {
// 	SignalPathGain_0db = 0,
// 	SignalPathGain_20db
// };

void hal_signal_path_attenuator_set(enum SignalPathAtten range);
// void hal_signal_path_gain_set(enum SignalPathGain range);



#endif /* INC_HAL_SIGNAL_PATH_H_ */
