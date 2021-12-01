#ifndef BUTTON_H_
#define BUTTON_H_

#include "global.h"

void GPIOA_Button_init();
void EXTI0_IRQHandler();
void EXTI1_IRQHandler();
void TIM6_OPM_init();
void TIM6_DAC_IRQHandler();

#endif /* MLX90614_H_ */
