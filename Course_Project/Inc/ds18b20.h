#ifndef DS18B20_H_
#define DS18B20_H_

#include "global.h"

#define	DS18B20_ERR_OK	0
#define	DS18B20_ERR_NO_SENSOR	1

void Delay_tick(uint16_t us);
void ds18b20_Init(GPIO_TypeDef *GPIO, uint16_t num_pin);
uint32_t ds18b20_GetTemperature();
void ds18b20_StartMeas();
void Init_Clock();
void ds18b20_SkipRom();
void ds18b20_RealTemp();

#endif /* DS18B20_H_ */
