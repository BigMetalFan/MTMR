#include "led.h"

volatile uint8_t pinNumber = 0;
//uint16_t pinsArray[] = {4,8,16,32,64,128,1};
ledPin_t pinsArray[] = {{&GPIOA->ODR, 4},
                        {&GPIOA->ODR, 8},
                        {&GPIOA->ODR, 16},
                        {&GPIOA->ODR, 32},
                        {&GPIOA->ODR, 64},
                        {&GPIOA->ODR, 128},
                        {&GPIOB->ODR, 1}};

void ledInit(){

	RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOAEN_Pos;
	RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOBEN_Pos;
    
	GPIOA->MODER |= (1<<GPIO_MODER_MODE2_Pos) | (1<<GPIO_MODER_MODE3_Pos)| (1<<GPIO_MODER_MODE4_Pos) |
		 (1<<GPIO_MODER_MODE5_Pos) |  (1<<GPIO_MODER_MODE6_Pos)|  (1<<GPIO_MODER_MODE7_Pos);
	GPIOB->MODER |= (1<<GPIO_MODER_MODE0_Pos);
}

void initPwm(uint32_t clockPeriod){
    RCC->APB2ENR |= 1 << RCC_APB2ENR_TIM11EN_Pos;
    TIM11->DIER |= TIM_DIER_UIE_Msk;
    TIM11->DIER |= TIM_DIER_CC1IE_Msk;
    TIM11->PSC = clockPeriod/1000000-1;
    TIM11->ARR = 4096-1;
    TIM11->CR1 |= TIM_CR1_CEN;
    //TIM11->OC
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    
}


void TIM1_TRG_COM_TIM11_IRQHandler(){
    if(TIM11->SR & TIM_SR_UIF_Msk){
       
        TIM11->SR &= ~TIM_SR_UIF;
        if(TIM11->CCR1 != 0){
            *pinsArray[pinNumber].port |= pinsArray[pinNumber].pin;
        }
        /*
        if(pinNumber<6){
            //GPIOA->ODR = pinsArray[pinNumber];
        }
        else{
            //GPIOB->ODR = pinsArray[pinNumber];
        }
        */

    }
    if(TIM11->SR & TIM_SR_CC1IF_Msk){
       
        TIM11->SR &= ~TIM_SR_CC1IF;
        GPIOA->ODR = 0;
        GPIOB->ODR = 0;
    }
}