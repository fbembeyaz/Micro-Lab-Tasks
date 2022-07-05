



void init_timer1(){

	RCC->APBENR2 |= (1U<< 11);// enable time1 module clock

	TIM1->CR1=0;// zero out the control register just in case
	TIM1->CR1 |= (1<<7);	// ARPE
	TIM1->CNT=0;// zero out counter

	/*0.1 ms interrupt	 */

	TIM1->PSC=y;
	TIM1->ARR=x;

	TIM1->DIER |= (1 << 0);// update interrupt enable
	TIM1->CR1 |= (1 << 0);// 	tım1 enable

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,3);
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

}

void TIM1_BRK_UP_TRG_COM_IRQHandler()
{
	TIM1->SR &= ~(1U << 0); 				// update SR registers when jump the code TIM1 handler
}


void TIM3_init(void){

	RCC->APBENR1 |= (1U<< 1);// enable time3 module clock

	TIM3->CR1=0;// zero out the control register just in case
	TIM3->CR1 |= (1U << 7);	// ARPE
	TIM3->CNT=0;// zero out counter


	TIM3->PSC = y;
	TIM3->ARR = x;

	TIM3->DIER |= (1 << 0);// update interrupt enable
	TIM3->CR1 |= (1 << 0);// 	tım3 enable

	NVIC_SetPriority(TIM3_IRQn,3);
	NVIC_EnableIRQ(TIM3_IRQn);

}

void TIM3_IRQHandler(void){

	TIM3->SR &= ~(1U << 0); //clear update status register
}



void BSP_button_init(void){
		/* Enable GPIOA clock */
    	RCC->IOPENR |= (1U << 0);

	    /* Setup PA1 as input */
	    GPIOA->MODER &= ~(3U << 2*1);
	    GPIOA->PUPDR |= (2U << 2*1); // Pull-down mode

	    /*setup interrrupts for inputs*/
	    EXTI->EXTICR[0] |= (0U << 8*1);//PA1

	    /* MASK*/
	    EXTI->IMR1 |= (1U << 1);

	    /*rising edge*/
	    EXTI->RTSR1 |= (1U << 1);

	    /*NVIC*/
	    NVIC_SetPriority(EXTI0_1_IRQn,0);
	    NVIC_EnableIRQ(EXTI0_1_IRQn);
}

void BSP_LedInit(void){


    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);


    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /*Clear led*/
    GPIOC->BRR |= (1U << 6);

}
void BSP_LedOn(void){


    GPIOC->ODR |= (1U << 6);
}
void BSP_LedOff(void){


    GPIOC->BRR |= (1U << 6);
}
void BSP_LedToggle(void){


    GPIOC->ODR ^= (1U << 6);
}

void SysTick_Handler(void)
{
	if (TimeDelay > 0)
		--TimeDelay;
}




void delay_ms(volatile unsigned int s){

	for(int i=s; i>0; i--){
	 SysTick_Config(SystemCoreClock / 1000); // 16 MHz / 1000 ile 1 ms elde edildi.
	 }
}