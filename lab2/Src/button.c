#include "button.h"

extern volatile uint8_t pinNumber;

void buttonInit(uint32_t clockPeriod){
    GPIOB->MODER &= ~GPIO_MODER_MODE12_Msk;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI12;
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PB;
    GPIOB->PUPDR |= 1<<GPIO_PUPDR_PUPD12_Pos;
    EXTI->IMR |= EXTI_IMR_IM12;
    EXTI->FTSR |= 1 << EXTI_FTSR_TR12_Pos;
    
    RCC->APB2ENR |= 1 << RCC_APB2ENR_TIM10EN_Pos;
    
    TIM10->CR1 |= TIM_CR1_OPM_Msk;
    TIM10->DIER |= TIM_DIER_UIE_Msk;
    
    TIM10->PSC = clockPeriod/10000-1;
    TIM10->ARR = 200-1;
    
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

void EXTI15_10_IRQHandler(){
    if(EXTI->PR & EXTI_PR_PR12_Msk){
        
        TIM10->CNT = 0;
        TIM10->CR1 |= TIM_CR1_CEN_Msk;
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->PR = EXTI_PR_PR12_Msk;
        EXTI->IMR &= ~EXTI_IMR_IM12;
    }
}

void TIM1_UP_TIM10_IRQHandler(){
    if(TIM10->SR & TIM_SR_UIF_Msk){
       
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->IMR |= EXTI_IMR_IM12;
        if((GPIOB->IDR & GPIO_IDR_ID12_Msk) == 0){
            pinNumber = pinNumber > 5 ? 0: pinNumber+1;
        }
    }
}