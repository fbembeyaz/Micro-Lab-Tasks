#include "stm32g0xx.h"


void delay(volatile uint32_t);
enum mode{mode0, mode1, mode2, mode3, mode4, mode5};
volatile enum mode state;
volatile long unsigned int x,y;

typedef struct{
	uint8_t Digits[4];
}tsSSD;

tsSSD Display;

void SSD_init();
void SSD_display();
void SSD_printDigit(uint8_t);
void SSD_bolum(float var);

void EXTI2_3_IRQHandler(void) {
	if (EXTI->RPR1 & (1U << 3)){
		state=0;
		SSD_bolum((float)state);
		}
		EXTI->RPR1 |= (1U << 3);

		for(uint32_t i=0; i> 0xFFFFFFFF; i++);



}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void){

	// işlem burada gerçekleşecek
	SSD_bolum();

	TIM1->SR &= ~(1U << 0);  //clear update status register
}

void init_timer1(){

	RCC->APBENR2 |= (1U << 11);  // enablee TIM1 modeule clock

	TIM1->CR1 = 0;  			 // zero out the control register
	TIM1->CR1 |= (1 << 7);      // ARPE
	TIM1->CNT = 0;  			 // zero out counter

	// second interrupt
	TIM1->PSC = y;   //perisclar
	TIM1->ARR = x;   // count

	TIM1->DIER |= (1 << 0);   // update interrupt enable
	TIM1->CR1  |= (1 << 0);   // TIM1 enable

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1);
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}



int main(void) {
    /* Enable GPIOB clock */
    RCC->IOPENR |= (1U << 1);

    /* Setup PB3 as input */
    GPIOB->MODER &= ~(3U << 2*3);

    /* Setup PB0 as output */
    GPIOB->MODER &= ~(3U);
    GPIOB->MODER |= (1U);

    EXTI->RTSR1 |= (1U << 3); // B3
    EXTI->EXTICR[0] |= (1U << 8*3);
    EXTI->IMR1 |= (1U << 3);

    NVIC_SetPriority(EXTI2_3_IRQn, 0);
    NVIC_EnableIRQ(EXTI2_3_IRQn);


    while(1){
        int value = GPIOB->IDR &= (1U << 0); //Getting button value
        	switch(state){
        		case 0:
        			SSD_display();
        			break;
        		case 1:
        			SSD_display();
        			x = 16000;     // işlemci 16 000 000 calisiyor bunu yazsak 1 sn ye de bir yanacak, 16 bit i astiğindan
        			y = 999;	  // prescalarla bu 16 bit e sıgacak sayıya boluyoruz
        			init_timer1();
        			while(1){
        					//value = GPIOB->IDR &= (1U << 0);
        					if(value == 0x100)
        						break;
        			}
        		case 2:
        			SSD_display();
        			x = 16000;     // işlemci 16 000 000 calisiyor bunu yazsak 1 sn ye de bir yanacak, 16 bit i astiğindan
        			y = 1999;	  // prescalarla bu 16 bit e sıgacak sayıya boluyoruz
        			init_timer1();
        			while(1){
        			        if(value == 0x100)
        			            break;
        			}

        		case 3:
        			SSD_display();
        			x = 1600;     // işlemci 16 000 000 calisiyor bunu yazsak 1 sn ye de bir yanacak, 16 bit i astiğindan
        			y = 999;	  // prescalarla bu 16 bit e sıgacak sayıya boluyoruz
        			init_timer1();
        			 while(1){
        			        		if(value == 0x100)
        			        			  break;
        			        			}
        		case 4:
        			SSD_display();
        			x = 1600;     // işlemci 16 000 000 calisiyor bunu yazsak 1 sn ye de bir yanacak, 16 bit i astiğindan
        			y = 9999;	  // prescalarla bu 16 bit e sıgacak sayıya boluyoruz
        			init_timer1();
        			 while(1){
        			        	if(value == 0x100)
        			        			   break;
        			        			   }
        		case 5:
        			SSD_display();
        			x = 160;     // işlemci 16 000 000 calisiyor bunu yazsak 1 sn ye de bir yanacak, 16 bit i astiğindan
        			y = 99;	  // prescalarla bu 16 bit e sıgacak sayıya boluyoruz
        			init_timer1();
        			   while(1){
        			        if(value == 0x100)
        			        	break;
        			        			 }

        	}
        }
  }


void delay(volatile uint32_t s) {
    for(; s>0; s--);
}

void SSD_bolum(float var){

	int number = (int)var;


	int temp = number / 10;
	Display.Digits[0] = (uint8_t)(number - (temp*10));

	temp = number / 100;
	Display.Digits[1] = (uint8_t)((number - (temp * 100)) / 10);

	temp = number / 1000;
	Display.Digits[2] = (uint8_t)((number - (temp * 1000)) / 100);

	temp = number / 10000;
	Display.Digits[3] = (uint8_t)((number - (temp * 10000)) / 1000);

}


void SSD_display(){

	static int i = 0;

		if(i == 1){
			GPIOA->ODR |= (1U << 7); //PA7
			GPIOB->ODR &= ~(1U << 6); //PB6
			GPIOB->ODR &= ~(1U << 7); //PB7
			GPIOB->ODR &= ~(1U << 1); //PB1
			SSD_printDigit(Display.Digits[0]);


		}
		else if(i == 10){
			GPIOA->ODR &= ~(1U << 7); //PA7
			GPIOB->ODR |= (1U << 6); //PB6
			GPIOB->ODR &= ~(1U << 7); //PB7
			GPIOB->ODR &= ~(1U << 1); //PB1
			SSD_printDigit(Display.Digits[1]);


		}

		else if(i == 20){
			GPIOA->ODR &= ~(1U << 7); //PA7
			GPIOB->ODR &= ~(1U << 6); //PB6
			GPIOB->ODR |= (1U << 7); //PB7
			GPIOB->ODR &= ~(1U << 1); //PB1
			SSD_printDigit(Display.Digits[2]);

		}
		else if(i == 30){
			GPIOA->ODR &= ~(1U << 7); //PA7
			GPIOB->ODR &= ~(1U << 6); //PB6
			GPIOB->ODR &= ~(1U << 7); //PB7
			GPIOB->ODR |= (1U << 1); //PB1
			SSD_printDigit(Display.Digits[3]);

		}
		else if(i == 40) i = 0;

		i++;


}

void SSD_init(){

	RCC->IOPENR |= (3U << 0);

	GPIOB->MODER &= ~(3U << 2*1);
	GPIOB->MODER |= (1U << 2*1);//PB1 is output

	GPIOB->MODER &= ~(3U << 2*3);
	GPIOB->MODER |= (1U << 2*3);//PB3 is output

	GPIOB->MODER &= ~(3U << 2*6);
	GPIOB->MODER |= (1U << 2*6);//PB6 is output

	GPIOB->MODER &= ~(3U << 2*7);
	GPIOB->MODER |= (1U << 2*7);//PB7 is output

	GPIOA->MODER &= ~(3U << 2*0);
	GPIOA->MODER |= (1U << 2*0);//PA0 is output

	GPIOB->MODER &= ~(3U << 2*4);
	GPIOB->MODER |= (1U << 2*4);//PB4 is output

	GPIOA->MODER &= ~(3U << 2*4);
	GPIOA->MODER |= (1U << 2*4);//PA4 is output

	GPIOB->MODER &= ~(3U << 2*0);
	GPIOB->MODER |= (1U << 2*0);//PB0 is output

	GPIOA->MODER &= ~(3U << 2*6);
	GPIOA->MODER |= (1U << 2*6);//PA6 is output

	GPIOA->MODER &= ~(3U << 2*7);
	GPIOA->MODER |= (1U << 2*7);//PA7 is output

	GPIOA->MODER &= ~(3U << 2*11);
	GPIOA->MODER |= (1U << 2*11);//PA11 is output

	GPIOA->MODER &= ~(3U << 2*12);
	GPIOA->MODER |= (1U << 2*12);//PA12 is output


}

void SSD_display(){

	static int i = 0;

		if(i == 1){
			GPIOA->ODR |= (1U << 7); //PA7
			GPIOB->ODR &= ~(1U << 6); //PB6
			GPIOB->ODR &= ~(1U << 7); //PB7
			GPIOB->ODR &= ~(1U << 1); //PB1
			SSD_printDigit(Display.Digits[0]);


		}
		else if(i == 10){
			GPIOA->ODR &= ~(1U << 7); //PA7
			GPIOB->ODR |= (1U << 6); //PB6
			GPIOB->ODR &= ~(1U << 7); //PB7
			GPIOB->ODR &= ~(1U << 1); //PB1
			SSD_printDigit(Display.Digits[1]);


		}

		else if(i == 20){
			GPIOA->ODR &= ~(1U << 7); //PA7
			GPIOB->ODR &= ~(1U << 6); //PB6
			GPIOB->ODR |= (1U << 7); //PB7
			GPIOB->ODR &= ~(1U << 1); //PB1
			SSD_printDigit(Display.Digits[2]);

		}
		else if(i == 30){
			GPIOA->ODR &= ~(1U << 7); //PA7
			GPIOB->ODR &= ~(1U << 6); //PB6
			GPIOB->ODR &= ~(1U << 7); //PB7
			GPIOB->ODR |= (1U << 1); //PB1
			SSD_printDigit(Display.Digits[3]);

		}
		else if(i == 40) i = 0;

		i++;


}



void SSD_printDigit(uint8_t x){

		switch(x){
		case 0:		//0

			GPIOB->ODR &= ~( 1U << 3); // PB3
			GPIOA->ODR &= ~( 1U << 0); // PA0
			GPIOB->ODR &= ~( 1U << 4); // PB4
			GPIOA->ODR &= ~( 1U << 4); // PA4
			GPIOB->ODR &= ~( 1U << 0); // PB0
			GPIOA->ODR &= ~( 1U << 12); // PA12
			GPIOA->ODR |=  ( 1U << 11); // PA11

			break;

		case 1:		//1
			GPIOB->ODR |= ( 1U << 3); // PB3
			GPIOA->ODR &= ~( 1U << 0); // PA0
			GPIOB->ODR &= ~( 1U << 4); // PB4
			GPIOA->ODR |= ( 1U << 4); // PA4
			GPIOB->ODR |= ( 1U << 0); // PB0
			GPIOA->ODR |= ( 1U << 12); // PA12
			GPIOA->ODR |= ( 1U << 11); // PA11

			break;

		case 2:		//2
			GPIOB->ODR &= ~( 1U << 3); // PB3
			GPIOA->ODR &= ~( 1U << 0); // PA0
			GPIOB->ODR |=  ( 1U << 4); // PB4
			GPIOA->ODR &= ~( 1U << 4); // PA4
			GPIOB->ODR &= ~( 1U << 0); // PB0
			GPIOA->ODR |=  ( 1U << 12); // PA12
			GPIOA->ODR &= ~( 1U << 11); // PA11

			break;

		case 3:		//3

			GPIOB->ODR &= ~( 1U << 3); // PB3
			GPIOA->ODR &= ~( 1U << 0); // PA0
			GPIOB->ODR &= ~( 1U << 4); // PB4
			GPIOA->ODR &= ~( 1U << 4); // PA4
			GPIOB->ODR |= ( 1U << 0); // PB0
			GPIOA->ODR |=  ( 1U << 12); // PA12
			GPIOA->ODR &= ~( 1U << 11); // PA11

			break;

		case 4:		//4
			GPIOB->ODR |=  ( 1U << 3); // PB3
			GPIOA->ODR &= ~( 1U << 0); // PA0
			GPIOB->ODR &= ~( 1U << 4); // PB4
			GPIOA->ODR |=  ( 1U << 4); // PA4
			GPIOB->ODR |= ( 1U << 0); // PB0
			GPIOA->ODR &= ~( 1U << 12); // PA12
			GPIOA->ODR &= ~( 1U << 11); // PA11

			break;

		case 5:		//5

			GPIOB->ODR &= ~( 1U << 3); // PB3
			GPIOA->ODR |=  ( 1U << 0); // PA0
			GPIOB->ODR &= ~( 1U << 4); // PB4
			GPIOA->ODR &= ~( 1U << 4); // PA4
			GPIOB->ODR |= ( 1U << 0); // PB0
			GPIOA->ODR &= ~( 1U << 12); // PA12
			GPIOA->ODR &= ~( 1U << 11); // PA11

			break;

		case 6:		//6
			GPIOB->ODR &= ~( 1U << 3); // PB3
			GPIOA->ODR |=  ( 1U << 0); // PA0
			GPIOB->ODR &= ~( 1U << 4); // PB4
			GPIOA->ODR &= ~( 1U << 4); // PA4
			GPIOB->ODR &= ~( 1U << 0); // PB0
			GPIOA->ODR &= ~( 1U << 12); // PA12
			GPIOA->ODR &= ~( 1U << 11); // PA11

			break;

		case 7:		//7

			GPIOB->ODR &= ~( 1U << 3); // PB3
			GPIOA->ODR &= ~( 1U << 0); // PA0
			GPIOB->ODR &= ~( 1U << 4); // PB4
			GPIOA->ODR |=  ( 1U << 4); // PA4
			GPIOB->ODR |= ( 1U << 0); // PB0
			GPIOA->ODR |=  ( 1U << 12); // PA12
			GPIOA->ODR |=  ( 1U << 11); // PA11

			break;

		case 8:		//8

			GPIOB->ODR &= ~( 1U << 3); // PB3
			GPIOA->ODR &= ~( 1U << 0); // PA0
			GPIOB->ODR &= ~( 1U << 4); // PB4
			GPIOA->ODR &= ~( 1U << 4); // PA4
			GPIOB->ODR &= ~( 1U << 0); // PB0
			GPIOA->ODR &= ~( 1U << 12); // PA12
			GPIOA->ODR &= ~( 1U << 11); // PA11

			break;

		case 9:		//9
			GPIOB->ODR &= ~( 1U << 3); // PB3
			GPIOA->ODR &= ~( 1U << 0); // PA0
			GPIOB->ODR &= ~( 1U << 4); // PB4
			GPIOA->ODR &= ~( 1U << 4); // PA4
			GPIOB->ODR |= ( 1U << 0); // PB0
			GPIOA->ODR &= ~( 1U << 12); // PA12
			GPIOA->ODR &= ~( 1U << 11); // PA11

			break;
		}
}

