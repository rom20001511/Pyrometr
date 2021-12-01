#include "i2c.h"

void I2C2_Init(void){
	/*
			SCL -> PB10
			SDA -> PB11
	*/

    //enabling the clocking of ports and the I2C module
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |=	RCC_APB1ENR_I2C2EN;

	//alternative function, open drain output, 50 MHz
	GPIOB->AFR[1] |= (0x04<<2*4);
	GPIOB->AFR[1] |= (0x04<<3*4);

	GPIOB->MODER |= GPIO_MODER_MODER10_1;
	GPIOB->OTYPER |= GPIO_OTYPER_OT_10;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;

	GPIOB->MODER |= GPIO_MODER_MODER11_1;
	GPIOB->OTYPER |= GPIO_OTYPER_OT_11;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11;

	//setting up the module in I2C mode
	I2C2->CR1 &= ~I2C_CR1_SMBUS;

    //specify the clock frequency of the module
	I2C2->CR2 &= ~I2C_CR2_FREQ;					// throw off the bits of the clock bus frequency APB1
	I2C2->CR2 |= F_APB1;  						// set the frequency of the APB1 bus from which the I2C module is clocked

	I2C2->CR1 &= ~I2C_CR1_PE; 					// turn off the I2C module to configure the CCR register

	I2C2->CCR &= ~(I2C_CCR_FS | I2C_CCR_DUTY);	//configuring I2C, standart mode, 100 KHz duty cycle 1/2
	I2C2->CCR |= CCR_VALUE;
	I2C2->TRISE = TRISE_VALUE;

	I2C2->CR1 |= I2C_CR1_ENPEC; 				// allow sending PEC
	I2C2->CR1 |= I2C_CR1_PE;					// turning on the I2C module
	I2C2->CR1 |= I2C_CR1_ACK; 					// allowing ACK
}
