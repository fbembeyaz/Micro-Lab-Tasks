
#ifndef BSP_H_
#define BSP_H_


#include "stm32g0xx.h"


void init_timer1();
void TIM3_init();
void TIM1_init();
void BSP_LedInit(void);
void BSP_LedOn(void);
void BSP_LedOff(void);
void BSP_LedToggle(void);
void SysTick_Handler(void)
void BSP_button_init();
void delay_ms(volatile uint32_t );


#endif 
