################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/ADC.c \
../Drivers/Src/GPIO_driver.c \
../Drivers/Src/I2C_driver.c \
../Drivers/Src/RCC_driver.c \
../Drivers/Src/SPI_driver.c \
../Drivers/Src/SysTick.c \
../Drivers/Src/Timer_driver.c \
../Drivers/Src/USART_driver.c 

OBJS += \
./Drivers/Src/ADC.o \
./Drivers/Src/GPIO_driver.o \
./Drivers/Src/I2C_driver.o \
./Drivers/Src/RCC_driver.o \
./Drivers/Src/SPI_driver.o \
./Drivers/Src/SysTick.o \
./Drivers/Src/Timer_driver.o \
./Drivers/Src/USART_driver.o 

C_DEPS += \
./Drivers/Src/ADC.d \
./Drivers/Src/GPIO_driver.d \
./Drivers/Src/I2C_driver.d \
./Drivers/Src/RCC_driver.d \
./Drivers/Src/SPI_driver.d \
./Drivers/Src/SysTick.d \
./Drivers/Src/Timer_driver.d \
./Drivers/Src/USART_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/%.o Drivers/Src/%.su Drivers/Src/%.cyclo: ../Drivers/Src/%.c Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DNUCLEO_L476RG -DSTM32L476RGTx -c -I../Inc -I"C:/Users/SUNILKUMAR P/STM32CubeIDE/workspace_1.19.0/UART_DMA/Drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Src

clean-Drivers-2f-Src:
	-$(RM) ./Drivers/Src/ADC.cyclo ./Drivers/Src/ADC.d ./Drivers/Src/ADC.o ./Drivers/Src/ADC.su ./Drivers/Src/GPIO_driver.cyclo ./Drivers/Src/GPIO_driver.d ./Drivers/Src/GPIO_driver.o ./Drivers/Src/GPIO_driver.su ./Drivers/Src/I2C_driver.cyclo ./Drivers/Src/I2C_driver.d ./Drivers/Src/I2C_driver.o ./Drivers/Src/I2C_driver.su ./Drivers/Src/RCC_driver.cyclo ./Drivers/Src/RCC_driver.d ./Drivers/Src/RCC_driver.o ./Drivers/Src/RCC_driver.su ./Drivers/Src/SPI_driver.cyclo ./Drivers/Src/SPI_driver.d ./Drivers/Src/SPI_driver.o ./Drivers/Src/SPI_driver.su ./Drivers/Src/SysTick.cyclo ./Drivers/Src/SysTick.d ./Drivers/Src/SysTick.o ./Drivers/Src/SysTick.su ./Drivers/Src/Timer_driver.cyclo ./Drivers/Src/Timer_driver.d ./Drivers/Src/Timer_driver.o ./Drivers/Src/Timer_driver.su ./Drivers/Src/USART_driver.cyclo ./Drivers/Src/USART_driver.d ./Drivers/Src/USART_driver.o ./Drivers/Src/USART_driver.su

.PHONY: clean-Drivers-2f-Src

