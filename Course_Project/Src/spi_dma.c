#include "spi_dma.h"

uint8_t spi_buffer_tx[BUFFER_SPI_SIZE];
uint8_t spi_buffer_rx[BUFFER_SPI_SIZE];

void GPIOA_init_spi_TFT(){
	/**************************** FOR TFT DISPLAY
	 * PA4 - CS
	 * PA2 - RESET
	 * PA3 - DC (data command)
	 * PA7 - MOSI
	 * PA5 - SCK
	 * 3.3V - LED
	 * 5V - VCC
	 * **/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1| GPIO_MODER_MODER7_1;
	GPIOA->AFR[0] |= 0x555 << 20;
	GPIOA->MODER |= GPIO_MODER_MODER4_0|GPIO_MODER_MODER3_0|GPIO_MODER_MODER2_0;
	SET_CS();
}


void DMA2_Stream3_IRQHandler(){
	if (DMA2->LISR & DMA_LISR_TCIF3) {
		DMA2->LIFCR |= DMA_LIFCR_CTCIF3;
		status_dma_tx = 1;
		Micro_tick_delay(1);
		SET_CS();
	}
}

void dma_tx_init(){
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream3->CR |= DMA_SxCR_DIR_0; // Memory-to-peripheral mode
	DMA2_Stream3->M0AR = (uint32_t)(&spi_buffer_tx[0]);
	DMA2_Stream3->PAR = (uint32_t)(&(SPI1->DR));
	DMA2_Stream3->CR |= 0x3 << DMA_SxCR_CHSEL_Pos; // Channel 3 selected
	DMA2_Stream3->CR |= DMA_SxCR_MINC; // Memory increment mode
	DMA2_Stream3->CR |= DMA_SxCR_TCIE; // Transfer complete interrupt enable
	DMA2_Stream3->CR |= DMA_SxCR_PL_1; //Priority level high
	NVIC_SetPriority(DMA2_Stream3_IRQn, 3);
	NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}


void spi1_master_init() {
	GPIOA_init_spi_TFT();
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST; // MSB transmits first
	SPI1->CR1 &= ~SPI_CR1_CPOL; // SPI1 mode: 0
	SPI1->CR1 &= ~SPI_CR1_CPHA; //
	SPI1->CR1 |= SPI_CR1_MSTR;	// Master mode
	SPI1->CR1 |= SPI_CR1_SSM; // Software NSS control
	SPI1->CR1 |= SPI_CR1_SSI; // NSS in high level by default

	SPI1->CR1 &= ~SPI_CR1_DFF; // 8 bit format

	dma_tx_init();
	SPI1->CR2 |= SPI_CR2_TXDMAEN;

	SPI1->CR1 |= SPI_CR1_SPE;
}

void spi1_SendDataDMA_1Byte(uint8_t* data, uint16_t count_byte) {
	while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE);
	status_dma_tx = 0;

	for (int i = 0; i < count_byte; i++) {
		spi_buffer_tx[i] = data[i];
	}

	DMA2_Stream3->CR &= ~DMA_SxCR_EN;
	DMA2_Stream3->NDTR = count_byte;
	RESET_CS();
	Start_DMA_Send_Data();
}
