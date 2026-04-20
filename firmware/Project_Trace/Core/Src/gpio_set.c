/*
 * gpio_set.c
 *
 *  Created on: 20 Apr 2026
 *      Author: Marius H. Tyson
 */

// TODO: Review this is almost certainly wrong in some way first lines for the project
// TODO: There is a lot of repeated code perhaps replace with functions
void set_GPIOx_MODER(char x, int bit, char mode ) {
	// x is the letter of the GPIO you are changing, bit is the number of the bit you are changing, char is the state and can have 4 states:
	// 'I' for input, 'O' for output, 'L' for alternative mode and 'N' for analog mode TODO: better name for last 2?
	GPIO_TypeDef *GPIOx; // Defines pointer to typedef so it can be changed later
    switch (x) {
        case 'A': GPIOx = GPIOA; break;
        case 'B': GPIOx = GPIOB; break;
        case 'C': GPIOx = GPIOC; break;
        case 'D': GPIOx = GPIOD; break;
        case 'E': GPIOx = GPIOE; break;
        case 'F': GPIOx = GPIOF; break;
        case 'G': GPIOx = GPIOG; break;
        case 'H': GPIOx = GPIOH; break;
        case 'M': GPIOx = GPIOM; break;
        case 'O': GPIOx = GPIOO; break;
        case 'P': GPIOx = GPIOP; break;

        default:
        	//return 0; // TODO: improve error handling
        	break;
    }

    switch (mode) {
    case 'I':
    	GPIOx ->MODER &= ~(0x3 << (bit * 2));
    	break;
    case 'O':
    	GPIOx ->MODER &= ~(0x3 << (bit * 2));
        GPIOx ->MODER |=  (0x1 << (bit * 2));
        break;
    case 'L':
    	GPIOx ->MODER &= ~(0x3 << (bit * 2));
    	GPIOx ->MODER |=  (0x2 << (bit * 2));
    	break;
    case 'N':
    	GPIOx ->MODER &= ~(0x3 << (bit * 2));
    	GPIOx ->MODER |=  (0x3 << (bit * 2));
    	break;

    default:
           //return 0; // TODO: improve error handling
           break;
    }
}

void set_GPIOx_OTYPER(char x, int bit, char mode) {
	  GPIO_TypeDef *GPIOx; // Defines pointer to typedef so it can be changed later
	    switch (x) {
	        case 'A': GPIOx = GPIOA; break;
	        case 'B': GPIOx = GPIOB; break;
	        case 'C': GPIOx = GPIOC; break;
	        case 'D': GPIOx = GPIOD; break;
	        case 'E': GPIOx = GPIOE; break;
	        case 'F': GPIOx = GPIOF; break;
	        case 'G': GPIOx = GPIOG; break;
	        case 'H': GPIOx = GPIOH; break;
	        case 'M': GPIOx = GPIOM; break;
	        case 'O': GPIOx = GPIOO; break;
	        case 'P': GPIOx = GPIOP; break;

	        default:
	        	//return 0; // TODO: improve error handling
	        	break;
	    }
	    switch (mode) {
	        case 'P': // push pull
	        	GPIOx ->OTYPER &= ~(0x3 << (bit));
	        	break;
	        case 'D': // open drain
	        	GPIOx ->OTYPER &= ~(0x3 << (bit));
	            GPIOx ->OTYPER |=  (0x1 << (bit));
	            break;

	        default:
	               //return 0; // TODO: improve error handling
	               break;

}
void set_GPIOx_OSPEEDR(char x, int bit, char mode) {
	GPIO_TypeDef *GPIOx; // Defines pointer to typedef so it can be changed later
		    switch (x) {
		        case 'A': GPIOx = GPIOA; break;
		        case 'B': GPIOx = GPIOB; break;
		        case 'C': GPIOx = GPIOC; break;
		        case 'D': GPIOx = GPIOD; break;
		        case 'E': GPIOx = GPIOE; break;
		        case 'F': GPIOx = GPIOF; break;
		        case 'G': GPIOx = GPIOG; break;
		        case 'H': GPIOx = GPIOH; break;
		        case 'M': GPIOx = GPIOM; break;
		        case 'O': GPIOx = GPIOO; break;
		        case 'P': GPIOx = GPIOP; break;

		        default:
		        	//return 0; // TODO: improve error handling
		        	break;
		    }
		    switch (mode) {
		   	        case 'L': // low speed
		   	        	GPIOx ->OSPEEDR &= ~(0x3 << (bit*2));
		   	        	break;
		   	        case 'M': // med speed
		   	        	GPIOx ->OSPEEDR &= ~(0x3 << (bit*2));
		   	            GPIOx ->OSPEEDR |=  (0x1 << (bit*2));
		   	            break;
		   	        case 'H': // high speed
		   	        	GPIOx ->OSPEEDR &= ~(0x3 << (bit*2));
		   	            GPIOx ->OSPEEDR |=  (0x2 << (bit*2));
		   	        case 'V': // very high speed
		   	        	GPIOx ->OSPEEDR &= ~(0x3 << (bit*2));
		   	            GPIOx ->OSPEEDR |=  (0x3 << (bit*2));
		   	        default:
		   	               //return 0; // TODO: improve error handling
		   	        	 break;
}
void set_GPIOx_PUPDR(char x, int bit, char mode) {
	GPIO_TypeDef *GPIOx; // Defines pointer to typedef so it can be changed later
			    switch (x) {
			        case 'A': GPIOx = GPIOA; break;
			        case 'B': GPIOx = GPIOB; break;
			        case 'C': GPIOx = GPIOC; break;
			        case 'D': GPIOx = GPIOD; break;
			        case 'E': GPIOx = GPIOE; break;
			        case 'F': GPIOx = GPIOF; break;
			        case 'G': GPIOx = GPIOG; break;
			        case 'H': GPIOx = GPIOH; break;
			        case 'M': GPIOx = GPIOM; break;
			        case 'O': GPIOx = GPIOO; break;
			        case 'P': GPIOx = GPIOP; break;

			        default:
			        	//return 0; // TODO: improve error handling
			        	break;
			    }
			    switch (mode) {
			       case 'N': // no pull up
			    	   GPIOx ->PUPDR &= ~(0x3 << (bit*2));
			    		   	        	break;
			       case 'U': // only pull up
			    	   GPIOx ->PUPDR &= ~(0x3 << (bit*2));
			    	   GPIOx ->PUPDR |=  (0x1 << (bit*2));
			    		   	            break;
			       case 'D': // only pull down
			    	   GPIOx ->PUPDR &= ~(0x3 << (bit*2));
			    	   GPIOx ->PUPDR |=  (0x2 << (bit*2));
			    	   	   	   	   	    break;
			       case 'A': // both pull up and pull down
			    	   GPIOx ->PUPDR &= ~(0x3 << (bit*2));
			    	   GPIOx ->PUPDR |=  (0x3 << (bit*2));
			    	   	   	   	   	    break;
			       default:
			    	   //return 0; // TODO: improve error handling
			    	   break;
}
void
