#ifndef I2C_H_
#define I2C_H_

#include "global.h"

#define F_APB1 36 										// APB1 bus frequency
#define TPCLK1 ( 1000/F_APB1 ) 							// the frequency period is APB1 ns. ~ 28
#define CCR_VALUE ( 10000 /(TPCLK1 * 2 ) ) 				// The value of the CCR register For 36 Mhz is ~ 179
#define TRISE_VALUE ( 1000 / TPCLK1)

void I2C2_Init(void);

#endif /* MLX90614_H_ */
