

#include "stm32g0xx.h"
#include "system_stm32g0xx.h"
#include "bsp.h"

#define LEDDELAY    1600000
float  periyot, frekans, duty_cycle;

void delay(volatile uint32_t);

void read_frequency(){

	//Enable Tim3
	RCC->APBENR1 |= (1U << 1);

	// Set PB4 alternate    7.4.9 GPIO alternate function low register (GPIOx_AFRL)
	GPIOB->AFR[0] &= ~(1U << 16); //
	GPIOB->AFR[0] &= ~(1U << 17);
	GPIOB->AFR[0] |= (1U << 18);
	GPIOB->AFR[0] &= ~(1U << 19);

	 // 0 comes from a4
	   // GPIOB->AFR[0] |= (2U << 4*0);

	//PB4 Alternate
	GPIOB->MODER |= (1U << 9);
	GPIOB->MODER &= ~(1U << 8);

	//  TIM3_CH1 input
	TIM3->TISEL |= ~(15U << 4);

	//Select the active input for TIM3_CCR1
	TIM3->CCMR2 |= (1U << 0);

	// Select the active polarity for TI1FP1 (used both for capture in TIMx_CCR1 and counter clear) ?
	TIM3->CCER &= ~(1U << 1);  // CC1P  = 0
	TIM3->CCER &= ~(1U << 3);  // CC1NP = 0

	//Select the active input for TIMx_CCR2
	TIM3->CCMR1 |= (1U << 9);   // CC2S bits to 10

	//Select the active polarity for TI1FP2 (used for capture in TIMx_CCR2)
	TIM3->CCER |= (1U << 5);  // CC2P/CC2NP=’10’

	//Select the valid trigger input
	TIM3->SMCR |= (1U << 4);  // TS bits to 00101 , [21,20,6,5,4]
	TIM3->SMCR |= (1U << 6);

	//Configure the slave mode controller in reset mode
	TIM3->SMCR |= (1U << 2);  // SMS bits to 0100 , [16,2,1,0] bitleri

	// Enable the captures
	TIM3->CCER |= (1U << 0); // CC1E
	TIM3->CCER |= (1U << 4); // CC2E

	// TIM2Enable
	TIM3->CR1 |= TIM_CR1_CEN;

	NVIC_SetPriority(TIM2_IRQn, 1);
	NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM3_IRQHandler(void){

	//TIM3_CCR1 den periyot
	//TIM3_CCR2 den duty cycle

	float periyot = TIM3->CCR1 ;
	frekans = 1/ periyot;
	float duty_cycle = TIM3->CCR2;

	//Update interrupt flag
	TIM3->SR &= ~(1U << 0);
}

/**
 * Setup PWM output for
 * TIM2 CH2 on PB3
 */
void init_pwm2() {

    // Setup GPIO
    //

    // Enable GPIOB clock
    RCC->IOPENR |= (1U << 1);
    // Enable TIM2 clock
    RCC->APBENR1 |= RCC_APBENR1_TIM2EN;

    // Set alternate function to 2
    // 3 comes from PB3
    GPIOB->AFR[0] |= (2U << 4*3);
    // Select AF from Moder
    GPIOB->MODER &= ~(3U << 2*3);
    GPIOB->MODER |= (2U << 2*3);

    // zero out the control register just in case
    TIM2->CR1 = 0;

    // Select PWM Mode 1
    TIM2->CCMR1 |= (6U << 12);
    // Preload Enable
    TIM2->CCMR1 |= TIM_CCMR1_OC2PE;

    // Capture compare ch2 enable
    TIM2->CCER |= TIM_CCER_CC2E;

    // zero out counter
    TIM2->CNT = 0;
    // 1 ms interrupt
    TIM2->PSC = 3;
    TIM2->ARR = frekans; // 4000;

    // zero out duty
    TIM2->CCR2 = duty_cycle; //0;

    // Update interrupt enable
    TIM2->DIER |= (1 << 0);

    // TIM3Enable
    TIM2->CR1 |= TIM_CR1_CEN;

    NVIC_SetPriority(TIM3_IRQn, 1);
    NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM2_IRQHandler(void){
TIM2->SR &= ~(1U << 0);
}


int main(void) {

    

    read_frequency();
    init_pwm2();
    // trap
        for(;;);

  

    return 0;
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}
