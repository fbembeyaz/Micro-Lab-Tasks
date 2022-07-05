/*
 * main.c
 *
 * author: Furkan
 *
 * description: SysTick
 */

#include "stm32g0xx.h"

#define LEDDELAY    1600000
volatile int32_t TimeDelay =0;


void delay(volatile uint32_t);
void SysTick_Initalize(volatile uint32_t ticks);

int main(void) {




	SysTick_Initalize(1000);     // Interrupt period = 1000 cycles
	delay(100);             	  // Delay 100 ticks

    /* Enable GPIOabC clock */
    RCC->IOPENR |= (1U << 0);

    /* Setup PA6 as output */
    GPIOA->MODER &= ~(3U << 2*6);
    GPIOA->MODER |= (1U << 2*6);

    /* Turn on LED */
    GPIOA->ODR |= (1U << 6);

    while(1) {
        delay(LEDDELAY);
        /* Toggle LED */
        GPIOA->ODR ^= (1U << 6);
    }

    return 0;
}

void SysTick_Initalize(volatile uint32_t ticks){

	SysTick->CTRL = 0;       		// Disable SysTick
	SysTick->LOAD = ticks-1; 		// Set reload register

	// Set interrupt priority of Systick to least urgency
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS)-1);

	/* Select processor closk: 1 = procsessor clok ; 0= external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;

	//Enables SysTick interrupt, 1 = enable, 0 = disable
	SysTick->CTRL |= SysTick_CTRL_TICKNIT;

	//Enable SysTick
	SysTick->CTRL |= SysTick_CTRL_ENABLE; */

	//enable 0-1-2. bit
	SysTick->CTRL = 0x7;

}

void SysTick_Handler(void){
	if (TimeDelay > 0)
		TimeDelay--;
}

void delay(volatile uint32_t s) {
	for(; s>0; s--); // busy wait
}
