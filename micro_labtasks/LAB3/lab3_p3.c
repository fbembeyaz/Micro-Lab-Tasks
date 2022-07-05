#include "stm32g0xx.h"

#define LEDDELAY    1600000

int k = 0;

void delay(volatile uint32_t);

void EXTI2_3_IRQHandler(void)
{
	int s;
	if(EXTI->RPR1 & (1U << 3))
	{
		for(s=LEDDELAY ; s>0; s--);
		k++;
		if(k>5)
			k=0;
		EXTI->RPR1 |= (1U << 3);
	}

}

void interrupt(void)
{
	RCC->IOPENR |= (1U << 1 );
	GPIOB->MODER &= ~(3U << 2*3);
	GPIOB->MODER |= (2U << 2*3);

	EXTI->EXTICR[0] |= (1U << 8*3);
	EXTI->RTSR1 |= (1U << 3);
	EXTI->IMR1 = 0;
	EXTI->IMR1  |= (1U << 3);


	NVIC_SetPriority(EXTI2_3_IRQn,0);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
}


int main(void) {

	RCC->IOPENR |= (1U << 0 );

	GPIOA->MODER &= ~(3U << 2*7);
	GPIOA->MODER |= (1U << 2*7);
	interrupt();
    while(1) {
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
	for(; s>0; s--);

}
