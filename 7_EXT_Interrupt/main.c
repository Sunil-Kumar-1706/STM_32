#include <stdio.h>                  // Standard I/O header
#include "stm32l4xx_hal.h"          // HAL library for STM32L4

#define BTN_PORT    GPIOC           // Button port is GPIOC
#define BTN_PIN     GPIO_PIN_13     // Button pin is PC13
#define LED_PORT    GPIOA           // LED port is GPIOA
#define LED_PIN     GPIO_PIN_5      // LED pin is PA5

void gpio_pc13_interrupt_init(void); // Function to configure GPIO and interrupt

int main()
{
    HAL_Init();                      // Initialize HAL library
    gpio_pc13_interrupt_init();      // Setup button and LED with interrupt

    while(1)                         // Infinite loop
    {
        // Do nothing, wait for interrupt
    }
}

void gpio_pc13_interrupt_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};   // Create GPIO config structure

    __HAL_RCC_GPIOC_CLK_ENABLE();    // Enable clock for GPIOC
    __HAL_RCC_GPIOA_CLK_ENABLE();    // Enable clock for GPIOA

    GPIO_InitStruct.Pin = BTN_PIN;   // Select PC13
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // Interrupt on rising edge
    GPIO_InitStruct.Pull = GPIO_NOPULL;         // No pull-up or pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed
    HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);  // Init button pin

    GPIO_InitStruct.Pin = LED_PIN;   // Select PA5
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Output push-pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;         // No pull
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);  // Init LED pin

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0); // Set EXTI interrupt priority
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         // Enable EXTI interrupt
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);   // Toggle LED when button pressed
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BTN_PIN);       // Handle PC13 interrupt
}
