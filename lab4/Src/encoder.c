#include "encoder.h"

//TIM4_CH2 - PB7
//TIM4_CH3 - PB8

void encoderInit(uint32_t clockFreq){
    
    RCC->APB1ENR |= 1 << RCC_APB1ENR_TIM4EN_Pos;
    
    GPIOB->MODER |= (2<<GPIO_MODER_MODE7_Pos) | (2<<GPIO_MODER_MODE8_Pos);
    GPIOA->AFR[0] &= ~(2<<GPIO_AFRL_AFSEL7_Pos);
    GPIOA->AFR[0] |= 2<<GPIO_AFRL_AFSEL7_Pos;
    GPIOA->AFR[1] &= ~(2<<GPIO_AFRH_AFSEL8_Pos);
    GPIOA->AFR[1] |= 2<<GPIO_AFRH_AFSEL8_Pos;
    
    
    TIM4->PSC = clockFreq/1000000-1;
    TIM4->ARR = 0xFFFF;
    TIM4->CCER &= ~(1 << TIM_CCER_CC3P_Pos);
    TIM4->CCER |= (1 << TIM_CCER_CC4P_Pos);
    TIM4->CCER |= (1 << TIM_CCER_CC3E_Pos) | (1 << TIM_CCER_CC4E_Pos);
    TIM4->CCMR2 &= ~((3 << TIM_CCMR2_CC3S_Pos) | (3 << TIM_CCMR2_CC4S_Pos));
    TIM4->CCMR2 |= (1 << TIM_CCMR2_CC3S_Pos) | (2 << TIM_CCMR2_CC4S_Pos);
    TIM4->DIER |= (1 << TIM_DIER_CC3IE_Pos) | (1 << TIM_DIER_CC4IE_Pos);
    TIM4->CR1 |= (1 << TIM_CR1_CEN_Pos);
    NVIC_EnableIRQ(TIM4_IRQn);
}

