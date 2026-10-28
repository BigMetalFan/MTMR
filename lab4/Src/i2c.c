#include "i2c.h"

void i2cInit(uint32_t clockPeriod){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
    
    
    I2C3->CR1 |= I2C_CR1_SWRST;
    I2C3->CR1 &= ~I2C_CR1_SWRST;
    
    GPIOA->MODER |= 2<<GPIO_MODER_MODE8_Pos;
    GPIOA->OTYPER  |= GPIO_OTYPER_OT_8;
    GPIOB->MODER |= 2<<GPIO_MODER_MODE4_Pos;
    GPIOB->OTYPER  |= GPIO_OTYPER_OT_4;
    GPIOA->AFR[1] &= ~(0xFU << GPIO_AFRH_AFSEL8_Pos);
    GPIOA->AFR[1] |=  (4U   << GPIO_AFRH_AFSEL8_Pos);
    GPIOB->AFR[0] &= ~(0xFU << GPIO_AFRL_AFSEL4_Pos);
    GPIOB->AFR[0] |=  (9U   << GPIO_AFRL_AFSEL4_Pos);
    
    I2C3->CR2 |= (clockPeriod/1000000)<<I2C_CR2_FREQ_Pos;
    I2C3->CCR = clockPeriod/(2*100000);
    I2C3->TRISE = (clockPeriod/1000000+1)&I2C_TRISE_TRISE;
    I2C3->CR1 |= I2C_CR1_PE;
}

void i2cEmergencyStop(){
        if (I2C3->SR1 & I2C_SR1_AF) {
        I2C3->SR1 &= ~I2C_SR1_AF;
    }
    I2C3->CR1 |= I2C_CR1_STOP;
}

void i2cWriteRegister(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t len){
    uint16_t timeout = TIMEOUT;
    
    I2C3->CR1 |= I2C_CR1_START;
    while(!(I2C3->SR1 & I2C_SR1_SB)&&--timeout);
    if(!timeout){
        i2cEmergencyStop();
        return;
    }
    timeout = TIMEOUT;
    I2C3->DR = addr<<1;
    while(!(I2C3->SR1 & I2C_SR1_ADDR)&&--timeout);
    if(!timeout){
        i2cEmergencyStop();
        return;
    }
    I2C3->SR1;
    I2C3->SR2;
    I2C3->DR = reg;
    while(!(I2C3->SR1 & I2C_SR1_TXE)&&--timeout);
    if(!timeout){
        i2cEmergencyStop();
        return;
    }
    timeout = TIMEOUT;
    for(int i = 0; i < len; i++){
        I2C3->DR = data[i];
        while(!(I2C3->SR1 & I2C_SR1_TXE)&&--timeout);
        if(!timeout){
            i2cEmergencyStop();
            return;
        }
        timeout = TIMEOUT;
    }
    while(!(I2C3->SR1 & I2C_SR1_BTF)&&--timeout);
    
    I2C3->CR1 |= I2C_CR1_STOP;
}