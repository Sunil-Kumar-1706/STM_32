################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/GPIO_driver.c \
../drivers/Src/SPI_driver.c 

OBJS += \
./drivers/Src/GPIO_driver.o \
./drivers/Src/SPI_driver.o 

C_DEPS += \
./drivers/Src/GPIO_driver.d \
./drivers/Src/SPI_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/%.o drivers/Src/%.su drivers/Src/%.cyclo: ../drivers/Src/%.c drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DNUCLEO_L476RG -DSTM32L476RGTx -c -I../Inc -I"C:/Users/SUNILKUMAR P/STM32CubeIDE/workspace_1.19.0/05_SPI_STM_ESP/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-drivers-2f-Src

clean-drivers-2f-Src:
	-$(RM) ./drivers/Src/GPIO_driver.cyclo ./drivers/Src/GPIO_driver.d ./drivers/Src/GPIO_driver.o ./drivers/Src/GPIO_driver.su ./drivers/Src/SPI_driver.cyclo ./drivers/Src/SPI_driver.d ./drivers/Src/SPI_driver.o ./drivers/Src/SPI_driver.su

.PHONY: clean-drivers-2f-Src

