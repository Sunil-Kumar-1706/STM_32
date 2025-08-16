#include <stdio.h>              // For printf
#include "stm32l4xx_hal.h"      // HAL library
#include "adc.h"                // ADC functions
#include "uart.h"               // UART functions

ADC_HandleTypeDef hadc1;        // ADC1 handle
uint32_t sensor_value;          // Variable to store ADC result

int main()
{
    HAL_Init();                 // Initialize HAL library
    uart2_init();               // Initialize UART2
    adc_pa0_interrupt_init();   // Initialize ADC on PA0 with interrupt

    HAL_ADC_Start_IT(&hadc1);   // Start ADC in interrupt mode

    while(1)                    // Infinite loop
    {
        printf("The sensor value : %d \n\r", (int)sensor_value); // Print sensor value
    }
}

// Callback when ADC conversion is complete
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    sensor_value = pa0_adc_read(); // Read ADC value from PA0
}
