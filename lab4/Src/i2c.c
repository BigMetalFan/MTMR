#include "i2c.h"
//PA8 ? I2C3_SCL
//PB4 ? I2C3_SDA

void i2cInit(uint32_t clockFreq){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
    
    I2C3->CR1 &= ~I2C_CR1_PE;
    I2C3->CR1 |= I2C_CR1_SWRST;
    I2C3->CR1 &= ~I2C_CR1_SWRST;
    GPIOA->MODER &= ~GPIO_MODER_MODE8;
    GPIOA->MODER |= 2<<GPIO_MODER_MODE8_Pos;
    GPIOA->OTYPER  |= GPIO_OTYPER_OT_8;
    GPIOB->MODER &= ~GPIO_MODER_MODE4;
    GPIOB->MODER |= 2<<GPIO_MODER_MODE4_Pos;
    GPIOB->OTYPER  |= GPIO_OTYPER_OT_4;
    GPIOA->AFR[1] &= ~(0xFU << GPIO_AFRH_AFSEL8_Pos);
    GPIOA->AFR[1] |=  (4U   << GPIO_AFRH_AFSEL8_Pos);
    GPIOB->AFR[0] &= ~(0xFU << GPIO_AFRL_AFSEL4_Pos);
    GPIOB->AFR[0] |=  (9U   << GPIO_AFRL_AFSEL4_Pos);
    
    I2C3->CR2 |= (clockFreq/1000000)<<I2C_CR2_FREQ_Pos;
    I2C3->CCR = clockFreq/(2*100000);
    I2C3->TRISE = (clockFreq/1000000+1)&I2C_TRISE_TRISE;
    I2C3->CR1 |= I2C_CR1_PE;
}

static int i2cStart();
static int i2cRestart();
static void i2cStop();
static int readByte(uint8_t *byte, uint8_t isLastByte);

static int i2cWriteByte(uint8_t byte);

static void i2cEmergencyStop(){
    if (I2C3->SR1 & I2C_SR1_AF) {
        I2C3->SR1 &= ~I2C_SR1_AF;
    }
    I2C3->CR1 |= I2C_CR1_STOP;
}

int i2cWriteRegister(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t len){
   uint8_t result = i2cStart();
    if(result){
        i2cEmergencyStop();
        return 1;
    }
    
    i2cWriteByte(addr<<1|0);
    (void)I2C3->SR1; (void)I2C3->SR2;
    
    if(i2cWriteByte(reg)){
        i2cEmergencyStop();
        return 1;
    }
    for(int i = 0; i < len; i++){
        if(i2cWriteByte(data[i])){
            i2cEmergencyStop();
            return 1;
        }
    }
    uint16_t timeout = TIMEOUT;
    while(!(I2C3->SR1 & I2C_SR1_BTF)&&--timeout){
        if(!timeout){
            i2cEmergencyStop();
            return 1;
        }
    }

    i2cStop();
    return 0;
}

int i2cWriteData(uint8_t addr, uint8_t* data, uint8_t len){
   uint8_t result = i2cStart();
    if(result){
        i2cEmergencyStop();
        return 1;
    }
    
    i2cWriteByte(addr<<1|0);
    (void)I2C3->SR1; (void)I2C3->SR2;
    
    for(int i = 0; i < len; i++){
        if(i2cWriteByte(data[i])){
            i2cEmergencyStop();
            return 1;
        }
    }
    uint16_t timeout = TIMEOUT;
    while(!(I2C3->SR1 & I2C_SR1_BTF)&&--timeout){
        if(!timeout){
            i2cEmergencyStop();
            return 1;
        }
    }
    
    i2cStop();
    return 0;
}

int i2cWriteConstant(uint8_t addr, uint8_t reg, uint8_t data, uint16_t len){
   uint8_t result = i2cStart();
    if(result){
        i2cEmergencyStop();
        return 1;
    }
    
    i2cWriteByte(addr<<1|0);
    (void)I2C3->SR1; (void)I2C3->SR2;
    
    if(i2cWriteByte(reg)){
        i2cEmergencyStop();
        return 1;
    }
    for(int i = 0; i < len; i++){
        if(i2cWriteByte(data)){
            i2cEmergencyStop();
            return 1;
        }
    }
    uint16_t timeout = TIMEOUT;
    while(!(I2C3->SR1 & I2C_SR1_BTF)&&--timeout){
        if(!timeout){
            i2cEmergencyStop();
            return 1;
        }
    }

    i2cStop();
    return 0;
}

int i2cReadRegister(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t count){
    uint8_t result = i2cStart();
     if(result){
        i2cEmergencyStop();
        return 1;
    }
    i2cWriteByte(addr<<1|0);
    (void)I2C3->SR1; (void)I2C3->SR2;
    if(i2cWriteByte(reg)){
        i2cEmergencyStop();
        return 1;
    }
    i2cRestart();
    i2cWriteByte(addr<<1|1);
    (void)I2C3->SR1; (void)I2C3->SR2;
    
    for(uint16_t i = 0; i < count; i++){
        uint8_t byte;
        if(readByte(&byte, (i+1)==count)){
            i2cEmergencyStop();
            return 1;
        }
        data[i] = byte;
    }
    return 0;
}

static int i2cStart(){
    uint16_t timeout = TIMEOUT;
    
    while((I2C3->SR2 & I2C_SR2_BUSY)&&--timeout);
    if(!timeout) return 1;
    timeout = TIMEOUT;
    
    I2C3->CR1 |= I2C_CR1_START;
    while(!(I2C3->SR1 & I2C_SR1_SB)&&--timeout);
    if(!timeout){
        return 1;
    }
    return 0;
}

static int i2cRestart(){
    uint16_t timeout = TIMEOUT;
    
    I2C3->CR1 |= I2C_CR1_START;
    while(!(I2C3->SR1 & I2C_SR1_SB)&&--timeout);
    if(!timeout){
        return 1;
    }
    return 0;
}
    
static void i2cStop(){
    uint16_t timeout = TIMEOUT;
    I2C3->CR1 |= I2C_CR1_STOP;
    while((I2C3->SR2 & I2C_SR2_BUSY) && --timeout); 
}

static int i2cWriteByte(uint8_t byte){
    uint16_t timeout = TIMEOUT;
    I2C3->DR = byte;
    while(!(I2C3->SR1 & I2C_SR1_TXE)&&--timeout);
    if(!timeout){
        return 1;
    }
    return 0;
}

static int readByte(uint8_t *byte, uint8_t isLastByte){
    if(isLastByte){
        I2C3->CR1 &= ~I2C_CR1_ACK;  
        I2C3->CR1 |= I2C_CR1_STOP;
    }
    uint16_t timeout = TIMEOUT;
    
    while (!(I2C3->SR1 & I2C_SR1_RXNE)&&--timeout);   
    *byte = I2C3->DR;
    if(isLastByte){
         I2C3->CR1 |= I2C_CR1_ACK;  
    }
    if(!timeout){
        return 1;
    }
    return 0;
}

