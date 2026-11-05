#include "button.h"

volatile uint8_t selected = 0;
volatile uint16_t pressedButton = 0;
uint16_t imrButtonsMask = 0;

volatile uint8_t pinNumber = 0;

void buttonInit(uint32_t clockFreq){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    
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
    imrButtonsMask = EXTI_IMR_IM15 | EXTI_IMR_IM9 | EXTI_IMR_IM3;
    EXTI->IMR |= imrButtonsMask;
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

//TIM4_CH2 - PB7
//TIM4_CH3 - PB8

void encoderInit(uint32_t clockFreq){
    
    RCC->APB1ENR |= 1 << RCC_APB1ENR_TIM4EN_Pos;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    
    GPIOB->MODER &= ~GPIO_MODER_MODE7_Msk;
    GPIOB->MODER &= ~GPIO_MODER_MODE8_Msk;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI7;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PB;

    
    GPIOB->PUPDR |= (1<<GPIO_PUPDR_PUPD7_Pos) | (1<<GPIO_PUPDR_PUPD8_Pos);
    EXTI->IMR |= EXTI_IMR_IM7;
    EXTI->FTSR |= (1 << EXTI_FTSR_TR7_Pos);
    
    TIM4->CR1 |= TIM_CR1_OPM_Msk;
    TIM4->DIER |= TIM_DIER_UIE_Msk;
    
    TIM4->PSC = clockFreq/10000-1;
    TIM4->ARR = 20-1;
    
    NVIC_EnableIRQ(TIM4_IRQn);
}


void EXTI15_10_IRQHandler(){
    if(EXTI->PR & EXTI_PR_PR15_Msk){
        
        TIM10->CNT = 0;
        TIM10->CR1 |= TIM_CR1_CEN_Msk;
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->PR = EXTI_PR_PR15_Msk;
        EXTI->IMR &= ~imrButtonsMask;
        pressedButton = GPIO_IDR_ID15_Msk;
    }
}

void EXTI9_5_IRQHandler(){
    if(EXTI->PR & EXTI_PR_PR9_Msk){
        
        TIM10->CNT = 0;
        TIM10->CR1 |= TIM_CR1_CEN_Msk;
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->PR = EXTI_PR_PR9_Msk;
        EXTI->IMR &= ~imrButtonsMask;
        pressedButton = GPIO_IDR_ID9_Msk;
    }
    if(EXTI->PR & EXTI_PR_PR7_Msk){
        TIM4->CNT = 0;
        TIM4->CR1 |= TIM_CR1_CEN_Msk;
        TIM4->SR &= ~TIM_SR_UIF;
        EXTI->PR = EXTI_PR_PR7_Msk;
        EXTI->IMR &= ~EXTI_IMR_IM7;
    }
}

void EXTI3_IRQHandler(){
    if(EXTI->PR & EXTI_PR_PR3_Msk){
        
        TIM10->CNT = 0;
        TIM10->CR1 |= TIM_CR1_CEN_Msk;
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->PR = EXTI_PR_PR3_Msk;
        EXTI->IMR &= ~imrButtonsMask;
        pressedButton = GPIO_IDR_ID3_Msk;
    }
}

volatile uint16_t ledBright[3] = {0};

void TIM1_UP_TIM10_IRQHandler(){
    if(TIM10->SR & TIM_SR_UIF_Msk){
       
        TIM10->SR &= ~TIM_SR_UIF;
        EXTI->IMR |= imrButtonsMask;
        if((GPIOB->IDR & GPIO_IDR_ID15_Msk) == 0 && pressedButton == GPIO_IDR_ID15_Msk){
            ledBright[selected] = 100;
        }
        if((GPIOB->IDR & GPIO_IDR_ID9_Msk) == 0 && pressedButton == GPIO_IDR_ID9_Msk){
            selected = selected == 2? 0: selected + 1;
        }
        if((GPIOB->IDR & GPIO_IDR_ID3_Msk) == 0  && pressedButton == GPIO_IDR_ID3_Msk){
            ledBright[selected] = 0;
        }
    }
}



void TIM4_IRQHandler(){
    if(TIM4->SR & TIM_SR_UIF_Msk){
        EXTI->IMR |= EXTI_IMR_IM7;
        TIM4->SR &= ~TIM_SR_UIF;
        
        if (!(GPIOB->IDR & GPIO_IDR_ID7)) { 
            if(GPIOB->IDR & GPIO_IDR_ID8){
                ledBright[selected] = ledBright[selected]>100-5? 100: ledBright[selected]+5;
            }
            else{
                ledBright[selected] = ledBright[selected]<5? 0: ledBright[selected]-5;
            }
        }
        
    }
}