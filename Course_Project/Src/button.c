#include "button.h"

float emissivity = 0.99;						// the radiation coefficient of the material


void GPIOA_Button_init() /* Button is located on PA0 pin */
{
	TIM6_OPM_init();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	EXTI->IMR|= EXTI_IMR_IM0;
	EXTI->EMR|= EXTI_EMR_EM0;
	EXTI->RTSR|= EXTI_RTSR_TR0;
	GPIOA->MODER &= ~GPIO_MODER_MODER1;
	EXTI->IMR|= EXTI_IMR_IM1;
	EXTI->EMR|= EXTI_EMR_EM1;
	EXTI->RTSR|= EXTI_RTSR_TR1;
	SYSCFG->EXTICR[0] &= ~0xF;
	SYSCFG->EXTICR[0] &= ~(0xF << 4);
	NVIC_SetPriority(EXTI0_IRQn, 3);
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI1_IRQn, 3);
	NVIC_EnableIRQ(EXTI1_IRQn);
}

void EXTI0_IRQHandler()
{
	EXTI->PR |= EXTI_PR_PR0;
	TIM6->CR1 |= TIM_CR1_CEN; //Starting the timer
}

void EXTI1_IRQHandler()
{
	EXTI->PR |= EXTI_PR_PR1;
	TIM6->CR1 |= TIM_CR1_CEN; //Starting the timer
}

void TIM6_OPM_init()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->CR1 |= TIM_CR1_ARPE;
	TIM6->CR1 |= TIM_CR1_OPM; // One pulse mode
	TIM6->PSC = 2400-1; // 100ms delay
	TIM6->ARR = 2000-1; // Processing an interrupt once per second
	TIM6->DIER |= TIM_DIER_UIE; //Timer Interrupt Resolution
	NVIC_SetPriority(TIM6_DAC_IRQn, 7);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

void TIM6_DAC_IRQHandler()
{
	TIM6->SR &= ~0x1;
	if (GPIOA ->IDR & 0x1)
	{
		emissivity+=0.01;
		if (emissivity>=1){
			emissivity=0.01;
		}
	}
	if (GPIOA ->IDR & 0x2)
	{
		emissivity-=0.01;
		if (emissivity<=0.01){
			emissivity=0.99;
		}
	}
	FloatToCharEmissivity(emissivity);
}
