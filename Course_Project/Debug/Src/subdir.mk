################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/button.c \
../Src/ds18b20.c \
../Src/i2c.c \
../Src/library.c \
../Src/main.c \
../Src/mlx90614.c \
../Src/spi_dma.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/tft.c 

OBJS += \
./Src/button.o \
./Src/ds18b20.o \
./Src/i2c.o \
./Src/library.o \
./Src/main.o \
./Src/mlx90614.o \
./Src/spi_dma.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/tft.o 

C_DEPS += \
./Src/button.d \
./Src/ds18b20.d \
./Src/i2c.d \
./Src/library.d \
./Src/main.d \
./Src/mlx90614.d \
./Src/spi_dma.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/tft.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -c -I"D:/Programs for study/STM32CubeIDE_1.7.0/Projects/Course_Project/Inc" -I"D:/Programs for study/STM32CubeIDE_1.7.0/Projects/Course_Project/CMSIS/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

