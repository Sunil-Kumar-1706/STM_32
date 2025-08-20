#include "main.h"              // Main header file
#include <stdio.h>             // Standard I/O library

UART_HandleTypeDef huart2;     // UART2 handle

// Function to send characters using SWO (ITM)
int __io_putchar(int ch)
{
    ITM_SendChar(ch);          // Send character to debugger console
    return ch;                 // Return the character
}

void SystemClock_Config(void); // Function to configure system clock
static void MX_GPIO_Init(void);// Function to initialize GPIO
static void MX_USART2_UART_Init(void); // Function to initialize UART2

int main(void)
{
    HAL_Init();                 // Initialize HAL
    SystemClock_Config();       // Setup system clock
    MX_GPIO_Init();             // Setup GPIO pins
    MX_USART2_UART_Init();      // Setup UART2

    while (1)                   // Infinite loop
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);   // Turn LED ON
        printf("LED ON\r\n");   // Print message
        HAL_Delay(1000);        // Wait 1 second

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // Turn LED OFF
        printf("LED OFF\r\n");  // Print message
        HAL_Delay(1000);        // Wait 1 second
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0}; // Oscillator settings
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}; // Clock settings

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
        Error_Handler();        // Handle error if voltage scaling fails

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;   // Use HSI
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                     // Enable HSI
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // Default calibration
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                 // Enable PLL
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;         // PLL source HSI
    RCC_OscInitStruct.PLL.PLLM = 1;                              // Divider M
    RCC_OscInitStruct.PLL.PLLN = 10;                             // Multiplier N
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;                  // Divider P
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;                  // Divider Q
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;                  // Divider R
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        Error_Handler();    // Handle error if oscillator setup fails

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2; // Clock types
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // Use PLL as system clock
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        // AHB divider
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;         // APB1 divider
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         // APB2 divider

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
        Error_Handler();    // Handle error if clock setup fails
}

static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;                     // Use USART2
    huart2.Init.BaudRate = 115200;                // Baud rate
    huart2.Init.WordLength = UART_WORDLENGTH_8B;  // 8-bit data
    huart2.Init.StopBits = UART_STOPBITS_1;       // 1 stop bit
    huart2.Init.Parity = UART_PARITY_NONE;        // No parity
    huart2.Init.Mode = UART_MODE_TX_RX;           // TX and RX enabled
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;  // No flow control
    huart2.Init.OverSampling = UART_OVERSAMPLING_16; // Oversampling 16
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE; // Disable one-bit sampling
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; // No advanced features
    if (HAL_UART_Init(&huart2) != HAL_OK)
        Error_Handler();        // Handle error if UART setup fails
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0}; // GPIO settings

    __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC clock
    __HAL_RCC_GPIOH_CLK_ENABLE(); // Enable GPIOH clock
    __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock
    __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable GPIOB clock

    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); // Reset LED pin

    GPIO_InitStruct.Pin = LD2_Pin;                // LED pin
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   // Output push-pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;           // No pull-up/pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // Low speed
    HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct); // Initialize LED pin
}

void Error_Handler(void)
{
    __disable_irq();      // Disable interrupts
    while (1) {}          // Infinite loop if error occurs
}
