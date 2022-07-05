#include "bsp.h"
#include <stdlib.h>
#include <stdio.h>



static volatile uint32_t tick;

void SysTick_Handler(){
	if (tick > 0)
		--tick;
}

void delay_ms(uint32_t s){
	tick = s;
	while(tick)
		SysTick_Handler();
}

void USART2_IRQHandler(void){
	uint8_t data = (uint8_t)(USART2->RDR);
	//RXNE is automatically cleared when read
	printChar(data);
}

void printChar(uint8_t c){
	USART2->TDR = (uint16_t)c;
	while (!(USART2->ISR & (1 << 6)));
}

int _write(int fd, char* ptr, int len){
	(void)fd;
	for (int i = 0; i <= len; ++i){
		printChar(ptr[i]);
	}
	return len;
}

void print(char* buf){
	int len = 0;
	while(buf[len++] != '\0');
	_write(0, buf, len);
}
/*
 * PA2 PA3 are connected to the Virtual COM port
 * USART2 module
 * */
void BSP_UART_init(uint32_t baud){
	RCC->IOPENR |= (1U << 0);
	RCC->APBENR1 |= (1U << 17); //or RCC->APBENR1 |= (1U << RCC_APBENR1_USART2EN);

	/*Setup PA2 as AF1*/
	GPIOA->MODER &= ~(3U << 2*2);
	GPIOA->MODER |= (2U << 2*2);
	/*Choose AF1 from mux*/
	GPIOA->AFR[0] &= ~(0xFU << 4*2);
	GPIOA->AFR[0] |= (1U << 4*2);

	/*Setup PA3 as AF1*/
	GPIOA->MODER &= ~(3U << 2*3);
	GPIOA->MODER |= (2U << 2*3);
	/*Choose AF1 from mux*/
	GPIOA->AFR[0] &= ~(0xFU << 4*3);
	GPIOA->AFR[0] |= (1U << 4*3);

	//setup uart2
	//reset uart cr1
	USART2->CR1 = 0;
	USART2->CR1 |= (1 << 3); //Transmit enable
	USART2->CR1 |= (1 << 2); //Receiver enable
	USART2->CR1 |= (1 << 5); //RXNEIE for interrupt

	USART2->BRR = (uint16_t)(SystemCoreClock / baud);

	USART2->CR1 |= (1 << 0); //USART enable

	NVIC_SetPriority(USART2_IRQn, 1);
	NVIC_EnableIRQ(USART2_IRQn);
}

