#include "led.h"

void ledInit(){

	RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOBEN_Pos;
    
	GPIOB->MODER |= (1<<GPIO_MODER_MODE1_Pos) | (1<<GPIO_MODER_MODE2_Pos) | (1<<GPIO_MODER_MODE10_Pos);
}

void initPwm(uint32_t clockFreq){
    RCC->APB1ENR |= 1 << RCC_APB1ENR_TIM2EN_Pos;
    TIM2->DIER |= TIM_DIER_UIE_Msk;
    TIM2->DIER |= TIM_DIER_CC1IE_Msk |TIM_DIER_CC2IE_Msk |TIM_DIER_CC3IE_Msk;
    TIM2->PSC = clockFreq/100000-1;
    TIM2->ARR = 100;
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM2_IRQn);
    
}


void TIM2_IRQHandler(){
    if(TIM2->SR & TIM_SR_UIF_Msk){
       
        TIM2->SR &= ~TIM_SR_UIF;
        
        TIM2->CCR1 = ledBright[0];
        TIM2->CCR2 = ledBright[1];
        TIM2->CCR3 = ledBright[2];

        if(TIM2->CCR1 != 0){
            GPIOB->BSRR = GPIO_BSRR_BS1;
        }
        if(TIM2->CCR2 != 0){
            GPIOB->BSRR = GPIO_BSRR_BS2;
        }
        if(TIM2->CCR3 != 0){
            GPIOB->BSRR = GPIO_BSRR_BS10;
        }

    }
    if(TIM2->SR & TIM_SR_CC1IF_Msk){
       
        TIM2->SR &= ~TIM_SR_CC1IF;
        GPIOB->BSRR = GPIO_BSRR_BR1;
    }
    if(TIM2->SR & TIM_SR_CC2IF_Msk){
       
        TIM2->SR &= ~TIM_SR_CC2IF;
        GPIOB->BSRR = GPIO_BSRR_BR2;
    }
    if(TIM2->SR & TIM_SR_CC3IF_Msk){
       
        TIM2->SR &= ~TIM_SR_CC3IF;
        GPIOB->BSRR = GPIO_BSRR_BR10;
    }
}