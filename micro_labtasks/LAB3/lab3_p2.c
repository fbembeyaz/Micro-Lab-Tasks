#include "stm32g0xx.h"

#define LEDDELAY    1600000
int k = 0;

void delay(volatile uint32_t);

int main(void) {

    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U << 0);
    /* Setup PA7 as output */
    GPIOA->MODER &= ~(3U << 2*7);
    GPIOA->MODER |= (1U << 2*7);




    GPIOA->MODER &= ~(3U << 2*1);
    GPIOA->ODR &= ~(1U << 7); // led turn off


while(1){
	    	switch(k){
	    	      case 0:
	    	      delay(LEDDELAY);
	    		  GPIOA->ODR &= ~(1U << 7);
	    		  break;
	    	      case 1:
	    	      delay(2*LEDDELAY);
	    	      GPIOA->ODR ^= (1U << 7);
	    	      break;
	    	      case 2:
	    	      delay(LEDDELAY);
	    	      GPIOA->ODR ^= (1U << 7);
	    	      break;
	    	      case 3:
	              delay(5*LEDDELAY);
	              GPIOA->ODR ^= (1U << 7);
	              break;
	    	      case 4:
	    	      delay(LEDDELAY);
	    	      GPIOA->ODR ^= (1U << 7);
	    	      break;
	    	      case 5:
	    	      delay(LEDDELAY);
	    		  GPIOA->ODR |= (1U << 7);
	    		  break;
	    	      default:
	    	    	  k=0;
	    	    	  break;
	         } 
	    } 
return 0;
	} 

void delay(volatile uint32_t s)
	{
	    for(; s>0; s--)
	    {
	    	if(GPIOA->IDR >> 1 == 1)
	    	{
	    		for(s=LEDDELAY ; s>0; s--);
	    		k++;
				if(k>5)
					k=0;
					break;

	        }
	    }
	}
