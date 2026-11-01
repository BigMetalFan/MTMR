#include "button.h"

volatile uint8_t selected = 0;

void buttonInit(uint32_t clockFreq){
    GPIOB->MODER &= ~GPIO_MODER_MODE15_Msk;
    GPIOB->MODER &= ~GPIO_MODER_MODE9_Msk;
    GPIOB->MODER &= ~GPIO_MODER_MODE3_Msk;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI15;
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PB;
    SYSCFG->EXTICR[2] &= ~SYSCFG_EXTICR3_EXTI9;
    SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PB;
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB;
    
    GPIOB->PUPDR |= (1<<GPIO_PUPDR_PUPD15_Pos) | (1<<GPIO_PUPDR_PUPD3_Pos)| (1<<GPIO_PUPDR_PUPD9_Pos);
    EXTI->IMR |= EXTI_IMR_IM15 | EXTI_IMR_IM9 | EXTI_IMR_IM3;
    EXTI->FTSR |= (1 << EXTI_FTSR_TR15_Pos) | (1 << EXTI_FTSR_TR9_Pos) | (1 << EXTI_FTSR_TR3_Pos);
    
    RCC->APB2ENR |= 1 << RCC_APB2ENR_TIM10EN_Pos;
    
    TIM10->CR1 |= TIM_CR1_OPM_Msk;
    TIM10->DIER |= TIM_DIER_UIE_Msk;
    
    TIM10->PSC = clockFreq/10000-1;
    TIM10->ARR = 200-1;
    
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    
}

void EXTI15_10_IRQHandler(){
    if(EXTI->PR & EXTI_PR_PR15_Msk){
        
        TIM10->CNT = 0;
        TIM10->CR1 |= TIM_CR1_CEN_Msk;
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->PR = EXTI_PR_PR15_Msk;
        EXTI->IMR &= ~EXTI_IMR_IM15;
    }
}

void EXTI9_5_IRQHandler(){
    if(EXTI->PR & EXTI_PR_PR9_Msk){
        
        TIM10->CNT = 0;
        TIM10->CR1 |= TIM_CR1_CEN_Msk;
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->PR = EXTI_PR_PR9_Msk;
        EXTI->IMR &= ~EXTI_IMR_IM9;
    }
}

void EXTI3_IRQHandler(){
    if(EXTI->PR & EXTI_PR_PR3_Msk){
        
        TIM10->CNT = 0;
        TIM10->CR1 |= TIM_CR1_CEN_Msk;
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->PR = EXTI_PR_PR3_Msk;
        EXTI->IMR &= ~EXTI_IMR_IM3;
    }
}

void TIM1_UP_TIM10_IRQHandler(){
    if(TIM10->SR & TIM_SR_UIF_Msk){
       
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->IMR |= EXTI_IMR_IM15;
        EXTI->IMR |= EXTI_IMR_IM3;
        if((GPIOB->IDR & GPIO_IDR_ID15_Msk) == 0){
            selected = selected == 2? 0: selected + 1;
        }
        if((GPIOB->IDR & GPIO_IDR_ID9_Msk) == 0){
            selected = selected == 2? 0: selected + 1;
        }
        if((GPIOB->IDR & GPIO_IDR_ID3_Msk) == 0){
            selected = selected == 2? 0: selected + 1;
        }
    }
}