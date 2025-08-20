#include <stdio.h>                  // Standard I/O for printf
#include "stm32l4xx_hal.h"          // HAL library header

#define BTN_PORT    GPIOC           // Button on Port C
#define BTN_PIN     GPIO_PIN_13     // Button pin PC13
#define LED_PORT    GPIOA           // LED on Port A
#define LED_PIN     GPIO_PIN_5      // LED pin PA5

UART_HandleTypeDef huart2;          // UART2 handle

void gpio_pc13_interrupt_init(void); // Function to init GPIO + interrupts
void uart2_init(void);               // Function to init UART2

// Redirect printf to UART2
int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY); // Send 1 char
    return ch;                                                     // Return sent char
}

int main()
{
    HAL_Init();                  // Initialize HAL library
    uart2_init();                // Initialize UART2
    gpio_pc13_interrupt_init();  // Initialize GPIO + interrupts

    while(1)                     // Infinite loop
    {
        // Do nothing, wait for interrupts
    }
}

void uart2_init(void) {
    __HAL_RCC_USART2_CLK_ENABLE();   // Enable clock for USART2
    __HAL_RCC_GPIOA_CLK_ENABLE();    // Enable clock for GPIOA

    GPIO_InitTypeDef GPIO_InitStruct = {0}; // Create GPIO config structure

    // Configure PA2 (TX) and PA3 (RX) for UART2
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;  // Select PA2 and PA3
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;         // Alternate function push-pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;             // No pull-up or pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;    // Low speed
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;    // Alternate function AF7 = USART2
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);         // Init GPIOA pins

    // Configure UART2 settings
    huart2.Instance = USART2;                // Select USART2
    huart2.Init.BaudRate = 115200;           // Baud rate 115200
    huart2.Init.WordLength = UART_WORDLENGTH_8B; // 8-bit data
    huart2.Init.StopBits = UART_STOPBITS_1;      // 1 stop bit
    huart2.Init.Parity = UART_PARITY_NONE;       // No parity
    huart2.Init.Mode = UART_MODE_TX_RX;          // Enable TX and RX
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE; // No hardware flow control
    huart2.Init.OverSampling = UART_OVERSAMPLING_16; // Oversampling by 16
    HAL_UART_Init(&huart2);                     // Initialize UART2
}

void gpio_pc13_interrupt_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0}; // Create GPIO config structure

    __HAL_RCC_GPIOC_CLK_ENABLE();   // Enable clock for GPIOC
    __HAL_RCC_GPIOA_CLK_ENABLE();   // Enable clock for GPIOA

    // Configure PC13 button with interrupt
    GPIO_InitStruct.Pin = GPIO_PIN_13;        // Select PC13
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // Interrupt on rising edge
    GPIO_InitStruct.Pull = GPIO_NOPULL;       // No pull
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed
    HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);  // Init PC13

    // Configure PA0 button with interrupt
    GPIO_InitStruct.Pin = GPIO_PIN_0;         // Select PA0
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // Interrupt on rising edge
    GPIO_InitStruct.Pull = GPIO_NOPULL;       // No pull
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);   // Init PA0

    // Configure PA5 as LED output
    GPIO_InitStruct.Pin = LED_PIN;            // Select PA5
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Output push-pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;       // No pull
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct); // Init PA5

    // Enable interrupt for EXTI0 (PA0)
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);  // Priority 0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);          // Enable IRQ

    // Enable interrupt for EXTI15_10 (PC13)
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0); // Priority 0
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         // Enable IRQ
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_13) {            // If PC13 triggered
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN); // Toggle LED
        printf("Button 13 pressed ! \n\r");   // Print message
    }

    if (GPIO_Pin == GPIO_PIN_0) {             // If PA0 triggered
        printf("Button 0 pressed ! \n\r");    // Print message
    }
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);    // Handle PC13 interrupt
}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);     // Handle PA0 interrupt
}

void Error_Handler(void) {
    __disable_irq();   // Disable interrupts
    while (1) {        // Infinite loop if error
        // Stay here
    }
}
