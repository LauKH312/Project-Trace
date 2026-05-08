#include <hal_gpio.h>
#include <stm32h7xx_it.h>


//GPIO_TypeDef *gpio_bank(enum gpio_GPIObank x) {
//    switch (x) {
//        case GPIO_A: return GPIOA;
//        case GPIO_B: return GPIOB;
//        case GPIO_C: return GPIOC;
//        case GPIO_D: return GPIOD;
//        case GPIO_E: return GPIOE;
//        case GPIO_F: return GPIOF;
//        case GPIO_G: return GPIOG;
//        case GPIO_H: return GPIOH;
//        default:
//        	return NULL; // TODO: improve error handling
//    }
//    //TODO: Reference manual says there are more banks, but they can't be found here. add more banks if needed
//}
//
//
//void hal_gpio_set_moder(enum gpio_GPIObank x, int pin, enum gpio_MODER mode ) {
//	 GPIO_TypeDef *GPIOx = gpio_bank(x);
//	 //  if (GPIOx == NULL) return; // TODO: improve error handling
//
//	switch (mode) {
//    case input:
//    	GPIOx ->MODER &= ~(0x3 << (pin * 2));
//    	break;
//    case output:
//    	GPIOx ->MODER &= ~(0x3 << (pin * 2));
//        GPIOx ->MODER |=  (0x1 << (pin * 2));
//        break;
//    case alternative :
//    	GPIOx ->MODER &= ~(0x3 << (pin * 2));
//    	GPIOx ->MODER |=  (0x2 << (pin * 2));
//    	break;
//    case analog:
//    	GPIOx ->MODER &= ~(0x3 << (pin * 2));
//    	GPIOx ->MODER |=  (0x3 << (pin * 2));
//    	break;
//
//    default:
//           //return 0; // TODO: improve error handling
//           break;
//    }
//}
//
//void hal_gpio_set_pushpullactive(enum gpio_GPIObank x, int pin,  enum gpio_pushpullactive mode) {
//	 GPIO_TypeDef *GPIOx = gpio_bank(x);
//	    switch (mode) {
//	        case pushpull :
//	        	GPIOx ->OTYPER &= ~(0x1 << (pin));
//	        	break;
//	        case open_drain:
//	        	GPIOx ->OTYPER &= ~(0x1 << (pin));
//	            GPIOx ->OTYPER |=  (0x1 << (pin));
//	            break;
//
//	        default:
//	               //return 0; // TODO: improve error handling
//	               break;
//	    }
//}
//void hal_gpio_set_speed(enum gpio_GPIObank x, int pin,  enum gpio_speed mode) {
//	GPIO_TypeDef *GPIOx = gpio_bank(x);
//		    switch (mode) {
//		   	        case low: // low speed
//		   	        	GPIOx->OSPEEDR &= ~(0x3 << (pin*2));
//		   	        	break;
//		   	        case med: // med speed
//		   	        	GPIOx->OSPEEDR &= ~(0x3 << (pin*2));
//		   	            GPIOx->OSPEEDR |=  (0x1 << (pin*2));
//		   	            break;
//		   	        case high: // high speed
//		   	        	GPIOx->OSPEEDR &= ~(0x3 << (pin*2));
//		   	            GPIOx->OSPEEDR |=  (0x2 << (pin*2));
//		   	            break;
//		   	        case very_high: // very high speed
//		   	        	GPIOx->OSPEEDR &= ~(0x3 << (pin*2));
//		   	            GPIOx->OSPEEDR |=  (0x3 << (pin*2));
//		   	            break;
//		   	        default:
//		   	               //return 0; // TODO: improve error handling
//		   	        	 break;
//		    }
//}
//
//void hal_gpio_set_pullupdown(enum gpio_GPIObank x, int pin, enum gpio_pullupdown mode) {
//	GPIO_TypeDef *GPIOx = gpio_bank(x);
//			    switch (mode) {
//			       case nothing: // no pull up
//			    	   GPIOx ->PUPDR &= ~(0x3 << (pin*2));
//			    		   	        	break;
//			       case pull_up: // only pull up
//			    	   GPIOx ->PUPDR &= ~(0x3 << (pin*2));
//			    	   GPIOx ->PUPDR |=  (0x1 << (pin*2));
//			    		   	            break;
//			       case pull_down: // only pull down
//			    	   GPIOx ->PUPDR &= ~(0x3 << (pin*2));
//			    	   GPIOx ->PUPDR |=  (0x2 << (pin*2));
//			    	   	   	   	   	    break;
//			       case pull_updown: // both pull up and pull down
//			    	   GPIOx ->PUPDR &= ~(0x3 << (pin*2));
//			    	   GPIOx ->PUPDR |=  (0x3 << (pin*2));
//			    	   	   	   	   	    break;
//			       default:
//			    	   //return 0; // TODO: improve error handling
//			    	   break;
//			    }
//}
//void hal_gpio_enable_clock(enum gpio_GPIObank x, enum gpio_clock state) { //TODO: Marius check pins are set correctly
//    switch (x) {
//        case GPIO_A:
//            switch (state) {
//                case enable:  RCC->AHB4ENR |=  RCC_AHB4ENR_GPIOAEN; break;
//                case disable: RCC->AHB4ENR &= ~RCC_AHB4ENR_GPIOAEN; break;
//                default: break;
//            }
//            break;
//        case GPIO_B:
//            switch (state) {
//                case enable:  RCC->AHB4ENR |=  RCC_AHB4ENR_GPIOBEN; break;
//                case disable: RCC->AHB4ENR &= ~RCC_AHB4ENR_GPIOBEN; break;
//                default: break;
//            }
//            break;
//        case GPIO_C:
//            switch (state) {
//                case enable:  RCC->AHB4ENR |=  RCC_AHB4ENR_GPIOCEN; break;
//                case disable: RCC->AHB4ENR &= ~RCC_AHB4ENR_GPIOCEN; break;
//                default: break;
//            }
//            break;
//        case GPIO_D:
//            switch (state) {
//                case enable:  RCC->AHB4ENR |=  RCC_AHB4ENR_GPIODEN; break;
//                case disable: RCC->AHB4ENR &= ~RCC_AHB4ENR_GPIODEN; break;
//                default: break;
//            }
//            break;
//        case GPIO_E:
//            switch (state) {
//                case enable:  RCC->AHB4ENR |=  RCC_AHB4ENR_GPIOEEN; break;
//                case disable: RCC->AHB4ENR &= ~RCC_AHB4ENR_GPIOEEN; break;
//                default: break;
//            }
//            break;
//        case GPIO_F:
//            switch (state) {
//                case enable:  RCC->AHB4ENR |=  RCC_AHB4ENR_GPIOFEN; break;
//                case disable: RCC->AHB4ENR &= ~RCC_AHB4ENR_GPIOFEN; break;
//                default: break;
//            }
//            break;
//        case GPIO_G:
//            switch (state) {
//                case enable:  RCC->AHB4ENR |=  RCC_AHB4ENR_GPIOGEN; break;
//                case disable: RCC->AHB4ENR &= ~RCC_AHB4ENR_GPIOGEN; break;
//                default: break;
//            }
//            break;
//        case GPIO_H:
//            switch (state) {
//                case enable:  RCC->AHB4ENR |=  RCC_AHB4ENR_GPIOHEN; break;
//                case disable: RCC->AHB4ENR &= ~RCC_AHB4ENR_GPIOHEN; break;
//                default: break;
//            }
//            break;
//        default: break; // TODO: improve error handling
//    }
//}
