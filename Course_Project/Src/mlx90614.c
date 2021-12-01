#include "mlx90614.h"

uint16_t temp; 									// temperature
uint8_t	temp_lsb;								// low temperature bits
float temp_result; 								// the resulting recalculated temperature
float temp_float;								// temperature converted to float format
extern float emissivity;

/*
  The function returns the temperature value in degrees Celsius and the float type.

  Input data:
  address - 				sensor address MLX90614

  ram_address the RAM address to read ( see constants in .h file ) :
 	MLX90614_TA - 		sensor crystal temperature
  	MLX90614_TOBJ_1 - 	temperature of the first IR sensor
*/

double getTemp_mlx90614(uint16_t address, uint8_t ram_address){

	 address = address<<1; 						// shift one to the left (so the sensor accepts the address plus the first write/read bit)
	 I2C2->CR1 |= I2C_CR1_START;
	 while (!(I2C2->SR1 & I2C_SR1_SB)){}

	 (void) I2C2->SR1;

	 I2C2->DR = address | MLX90614_WRITE; 		// recording mode, MLX90614 address transmission
	 while (!(I2C2->SR1 & I2C_SR1_ADDR)){}		// we are waiting for the address to be sent

	 (void) I2C2->SR1;
	 (void) I2C2->SR2;

	 I2C2->DR= ram_address;						// transmitting the RAM address of the MLX90614 sensor
	 while (!(I2C2->SR1 & I2C_SR1_TXE)){}

	 I2C2->CR1 |= I2C_CR1_START;				// restart
	 while (!(I2C2->SR1 & I2C_SR1_SB)){}

	 (void) I2C2->SR1;

	 I2C2->DR = address | MLX90614_READ;		// accessing the sensor for reading
	 while (!(I2C2->SR1 & I2C_SR1_ADDR)){}

	 (void) I2C2->SR1;
	 (void) I2C2->SR2;

	 while(!(I2C2->SR1 & I2C_SR1_RXNE)){}

	 temp_lsb = I2C2->DR;						// reading the lowest byte

	 while(!(I2C2->SR1 & I2C_SR1_RXNE)){}

	 temp = I2C2->DR;							// reading the highest byte

	 I2C2->CR1 |= I2C_CR1_STOP;

	 temp = (temp & 0x007F) << 8;				//remove the error bit, shift it to the highest byte
	 temp |= temp_lsb;
	 temp_float = (float) temp; 				// bring it to the float format
	 temp_result =  ((temp_float * 0.02)- 0.01 );			// multiply by the measurement resolution
	 temp_result =  (temp_result - 273.15)/emissivity; 		// reduce it to degrees Celsius and divide it by the radiation coefficient of the material

	 return temp_result;
}
