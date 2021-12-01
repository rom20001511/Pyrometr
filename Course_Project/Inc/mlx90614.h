#ifndef MLX90614_H_
#define MLX90614_H_

#include "global.h"

#define MLX90614_TA 0x06								// sensor crystal temperature
#define MLX90614_TOBJ_1 0x07							// temperature from the first IR sensor


#define MLX90614_READ 1									// sensor reading mode
#define MLX90614_WRITE 0								// sensor recording mode


double getTemp_mlx90614(uint16_t address, uint8_t ram_address);
void getTemp_Mlx90614_CharArray( uint16_t address, uint8_t ram_address, char* buf);
void I2C2_Init(void);
void GPIOA_Button_init();
void EXTI0_IRQHandler();
void TIM6_OPM_init();
void TIM6_DAC_IRQHandler();
void getTemp_mlx90614_CharArray( uint16_t address, uint8_t ram_address, char* buf);

#endif /* MLX90614_H_ */
