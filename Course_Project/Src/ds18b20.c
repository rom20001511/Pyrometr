#include "ds18b20.h"

GPIO_TypeDef *CurrGPIO;
uint16_t curr_pin;
float real_temp = 0;
uint32_t array_temp[2];

#define SET_OUT()	(CurrGPIO->MODER |= 1 << curr_pin * 2)
#define SET_IN()	(CurrGPIO->MODER &= ~(1 << curr_pin*2))
#define OUT_1()	(CurrGPIO->BSRR = 1 << curr_pin)
#define OUT_0()	(CurrGPIO->BSRR = 1 << (curr_pin + 16))


static void InitGPIO_RCC(GPIO_TypeDef *GPIO) {
	if (GPIO == GPIOA) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}
	else if (GPIO == GPIOB) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	}
	else if (GPIO == GPIOC) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	}
	else if (GPIO == GPIOD) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	}
}

void Init_Clock(){
	FLASH->ACR |= FLASH_ACR_LATENCY_1WS; // 1 wait state for 48 MHz
	RCC->CR |= RCC_CR_HSEON; // Activate external clock (HSE: 8 MHz)
	while ((RCC->CR & RCC_CR_HSERDY) == 0); // Wait until HSE is ready
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ;
	RCC->PLLCFGR |= 4 << RCC_PLLCFGR_PLLQ_Pos; // PLL-Q: /4
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE; // PLL source is HSE
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP; // PLL-P: /2
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
	RCC->PLLCFGR |= 96 << RCC_PLLCFGR_PLLN_Pos; // PLL-N: x96
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
	RCC->PLLCFGR |= 4 << RCC_PLLCFGR_PLLM_Pos; // PLL-M: /4
	RCC->CR |= RCC_CR_PLLON; // Activate the PLL (Output: 96 MHz)
	while ((RCC->CR & RCC_CR_PLLRDY) == 0); // Wait until PLL is ready
	RCC->CFGR |= RCC_CFGR_HPRE_DIV2; // AHB divider: /2 (48 MHz)
	RCC->CFGR |= RCC_CFGR_SW_PLL; // Switching to PLL clock source
}

void Delay_tick(uint16_t tick) {
	for (uint32_t i = 0; i < tick; i++);
}

static uint8_t reset_ds18() {
	SET_OUT();	//write
	OUT_0();
	Delay_tick(1700);	// > 500 uS
	SET_IN();	//read...
	Delay_tick(300);	//88 uS
	if ((CurrGPIO->IDR & (1 << curr_pin))) {
		SET_OUT();
		OUT_1();
		return DS18B20_ERR_NO_SENSOR;
	}
	Delay_tick(1030);	//300 uS

	SET_IN();
	return 0;
}

static void write_ds18(uint32_t data, uint8_t num_bit) {
	SET_OUT();

	for (uint8_t i = 0; i < num_bit; i++)
	{
		OUT_0();
		Delay_tick(1);	//approx 1 uS

		if ((data & (1 << i))) {
			OUT_1();
		}
		Delay_tick(300);	//88uS

		OUT_1();
		Delay_tick(4);
	}
	SET_IN();
}

static uint32_t read_ds18(uint8_t count_bit) {
	uint32_t data = 0;
	for (uint8_t i = 0; i < count_bit; i++) {
		SET_OUT();
		OUT_0();
		Delay_tick(10);
		SET_IN();
		Delay_tick(10);	//
		data |= ((CurrGPIO->IDR & (1 << curr_pin)) >> curr_pin) << i;
		Delay_tick(150);
	}

	SET_IN();
	return data;
}

void ds18b20_Init(GPIO_TypeDef *GPIO, uint16_t num_pin) {

	InitGPIO_RCC(GPIO);
	CurrGPIO = GPIO;
	curr_pin = num_pin;

	SET_OUT();
	reset_ds18();
	ds18b20_SkipRom();
}

uint32_t ds18b20_GetTemperature(){
	uint32_t temperature = 0;

	reset_ds18();

	ds18b20_SkipRom();	//0xCC

	Delay_tick(30);	//10uS

	write_ds18(0xBE, 8);	//read scratchpad
	Delay_tick(2);	//2uS
	temperature = read_ds18(16);	//read data from device....
	return temperature;
}

void ds18b20_StartMeas() {
	reset_ds18();

	ds18b20_SkipRom();
	Delay_tick(30);	//10 uS

	write_ds18(0x44, 8);	//convert_T init single temperature conversion
}

void ds18b20_SkipRom() {
	write_ds18(0xCC, 8);
}

void ds18b20_RealTemp(){
	ds18b20_Init(GPIOB, 8);
	uint32_t temperature = 0;
	ds18b20_StartMeas();
	for (int i = 0; i < 1000; i++) {
		Delay_tick(3400);
	}
	temperature = ds18b20_GetTemperature();
	real_temp = (temperature & 0xFFF) * 0.0625;
	Delay_tick(100);
}
