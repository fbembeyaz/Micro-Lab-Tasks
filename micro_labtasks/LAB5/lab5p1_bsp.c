#include "bsp.h"


void BSP_system_init(void){

	__disable_irq();
	BSP_UART_init(9600);
	__enable_irq();

}


void uart_tx(uint8_t c){

	USART2->TDR = (uint16_t)c;
	while (!(USART2->ISR & (1 << 6)));

}

uint8_t uart_rx(void){

	while (!(USART2->ISR & (1 << 5)));

	return (uint8_t) USART2->RDR;
}


void BSP_UART_init(uint32_t baud ){

	RCC->IOPENR |= (1U << 0);
	RCC->APBENR1 |= (1U << 17);

	//PA2
    GPIOA->MODER &= ~(3U << 2*2);
	GPIOA->MODER |= (2U << 2*2);
	//Alternate func
	GPIOA->AFR[0] &= ~(0xFU << 4*2);
	GPIOA->AFR[0] |= (1U << 4*2);

	//PA3
	GPIOA->MODER &= ~(3U << 2*3);
	GPIOA->MODER |= (2U << 2*3);
	//Alternate func
	GPIOA->AFR[0] &= ~(0xFU << 4*3);
	GPIOA->AFR[0] |= (1U << 4*3);

	//UART2
	//reset cr1
	USART2->CR1 = 0;
	USART2->CR1 |= (1 << 3);  //TE
	USART2->CR1 |= (1 << 2);  //RE
	USART2->CR1 |= (1 << 5);  //RXNEIE

	USART2->BRR = (uint16_t)(SystemCoreClock / baud);
	USART2->CR1 |= (1 << 0);  //UE


}

