#include "bsp.h"


void BSP_system_init(void){

	__disable_irq();

	init_timer1();

	__enable_irq();


}

void pwm_init(){

	RCC->IOPENR |= (1U << 3);

	GPIOC->MODER &= ~(3U << 2*6);
	GPIOC->MODER |= (2U << 2*6);


	TIM2->CCR3	= 800;

	TIM2->CCMR1 |= (0x6U << 4); // mode 1 enable
	TIM2->CCMR1 |= (1U << 3);
	TIM2->CCER |= (1U << 8);


}



void init_timer1(){

	RCC->APBENR1 |= (1U<< 0);// enable time2 module clock

	TIM2->CR1=0;// zero out the control register just in case
	TIM2->CR1 |= (1U << 7);	// ARPE
	TIM2->CNT=0;// zero out counter

	/*1 second interrupt	 */

	TIM2->PSC = 9;
	TIM2->ARR = 1600;

	TIM2->DIER |= (1 << 0);// update interrupt enable
	TIM2->CR1 |= (1 << 0);// 	tım1 enable

	NVIC_SetPriority(TIM2_IRQn,3);
	NVIC_EnableIRQ(TIM2_IRQn);

}

void TIM2_IRQHandler(void){

	pwm_init();
}
