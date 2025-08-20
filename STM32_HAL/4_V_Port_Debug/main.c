#include "main.h"  // Include main header

UART_HandleTypeDef huart2;  // UART2 handle

void SystemClock_Config(void);       // System clock setup
static void MX_GPIO_Init(void);      // GPIO setup
static void MX_USART2_UART_Init(void); // UART2 setup

int main(void)
{
  HAL_Init();                 // Init HAL
  SystemClock_Config();       // Set clock
  MX_GPIO_Init();             // Init GPIO
  MX_USART2_UART_Init();      // Init UART2

  while (1)                   // Infinite loop
  {
    HAL_UART_Transmit(&huart2, (uint8_t*)"Hello From UART\r\n", 17, 1000); // Send text via UART
    HAL_Delay(1000);          // Wait 1 second
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};  // Oscillator config struct
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};  // Clock config struct

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) // Voltage scaling
  {
    Error_Handler();          // Error handler
  }

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // Use HSI
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                   // Enable HSI
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // Default trim
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;               // Enable PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;       // Source = HSI
  RCC_OscInitStruct.PLL.PLLM = 1;                            // PLLM = 1
  RCC_OscInitStruct.PLL.PLLN = 10;                           // PLLN = 10
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;                // PLLP = /7
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;                // PLLQ = /2
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;                // PLLR = /2

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)       // Apply oscillator config
  {
    Error_Handler();          // Error handler
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK   // Select clocks
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;  // SYSCLK from PLL
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;         // AHB prescaler /1
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;          // APB1 prescaler /1
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;          // APB2 prescaler /1

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) // Apply clock config
  {
    Error_Handler();          // Error handler
  }
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;                       // Use USART2
  huart2.Init.BaudRate = 115200;                  // Baud = 115200
  huart2.Init.WordLength = UART_WORDLENGTH_8B;    // 8 data bits
  huart2.Init.StopBits = UART_STOPBITS_1;         // 1 stop bit
  huart2.Init.Parity = UART_PARITY_NONE;          // No parity
  huart2.Init.Mode = UART_MODE_TX_RX;             // Enable TX/RX
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;    // No flow control
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;// Oversample x16
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE; // Disable 1-bit
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; // No advanced

  if (HAL_UART_Init(&huart2) != HAL_OK)           // Init UART
  {
    Error_Handler();          // Error handler
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};         // GPIO config struct

  __HAL_RCC_GPIOC_CLK_ENABLE();   // Enable GPIOC clock
  __HAL_RCC_GPIOH_CLK_ENABLE();   // Enable GPIOH clock
  __HAL_RCC_GPIOA_CLK_ENABLE();   // Enable GPIOA clock
  __HAL_RCC_GPIOB_CLK_ENABLE();   // Enable GPIOB clock

  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); // Reset LED pin

  GPIO_InitStruct.Pin = LD2_Pin;            // Select LD2 pin
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Output mode
  GPIO_InitStruct.Pull = GPIO_NOPULL;       // No pull
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct); // Init LED pin
}

void Error_Handler(void)
{
  __disable_irq();              // Disable interrupts
  while (1)                     // Infinite loop
  {
    // Stay here on error
  }
}
