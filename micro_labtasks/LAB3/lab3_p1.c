#include "stm32g0xx.h"

#define LEDDELAY    1600000

void delay(volatile uint32_t);

int main(void) {

    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U << 0);

    /* Setup PA7 as output */
    GPIOA->MODER &= ~(3U << 2*7);
    GPIOA->MODER |= (1U << 2*7);

    /* Turn on LED */
    GPIOA->ODR |= (1U << 7);

    while(1) {
        delay(LEDDELAY);
        /* Toggle LED */
        GPIOA->ODR ^= (1U << 7);
    }

    return 0;
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}
