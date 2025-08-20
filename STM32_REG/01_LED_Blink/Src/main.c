#include <stdint.h>
#include "STM32476.h"
#include "GPIO_driver.h"

// Simple delay function
void delay()
{
	for(uint32_t i = 0; i < 500000; i++);  // Short pause
}

int main(void)
{
    /* LED setup */
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOA;                        // Use port A
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5; // Pin 5
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;   // Output mode
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; // Fast speed
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; // Push-pull
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD; // No pull-up/pull-down

	GPIO_PeriClockControl(GPIOA, ENABLE); // Turn on clock for GPIOA
	GPIO_Init(&GpioLed);                  // Apply settings to pin

	while(1)  // Loop forever
	{
		GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5); // Toggle LED
		delay();                                    // Wait a bit
	}
}
