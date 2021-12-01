#include "global.h"
extern float temp_result;

uint8_t status_dma_tx=1;

int main(void)
{
	Init_Clock();
	I2C2_Init();
	GPIOA_Button_init();
	TFT_init();

	while (1)
	{
		ds18b20_RealTemp();
		getTemp_mlx90614(0x5A,  MLX90614_TA);
		FloatToCharTemperature(temp_result, 66, 20);
		getTemp_mlx90614(0x5A,  MLX90614_TOBJ_1);
		FloatToCharTemperature(temp_result, 90, 40);
	}
}

