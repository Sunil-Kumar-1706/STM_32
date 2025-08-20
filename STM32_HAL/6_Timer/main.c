#include "main.h"                            // Main header file

TIM_HandleTypeDef htim3;                     // Timer3 handle
UART_HandleTypeDef huart2;                   // UART2 handle

void SystemClock_Config(void);               // System clock config function
static void MX_GPIO_Init(void);              // GPIO init function
static void MX_USART2_UART_Init(void);       // UART2 init function
static void MX_TIM3_Init(void);              // Timer3 init function

int main(void)
{
  HAL_Init();                                // Initialize HAL
  SystemClock_Config();                      // Setup system clock
  MX_GPIO_Init();                            // Init GPIO
  MX_USART2_UART_Init();                     // Init UART2
  MX_TIM3_Init();                            // Init Timer3
  HAL_TIM_Base_Start_IT(&htim3);             // Start Timer3 with interrupt

  while (1)                                  // Infinite loop
  {
    // Nothing, work done in interrupt
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3)                // Check if Timer3 triggered
  {
    HAL_UART_Transmit(&huart2, (uint8_t*)"HELLO\r\n", 7, 1000); // Send HELLO
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0}; // Oscillator config struct
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}; // Clock config struct

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1); // Set voltage

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // Use HSI
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                   // Turn on HSI
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // Default trim
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;               // Enable PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;       // PLL source HSI
  RCC_OscInitStruct.PLL.PLLM = 1;                            // PLL divider M
  RCC_OscInitStruct.PLL.PLLN = 10;                           // PLL multiplier N
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;                // PLL divider P
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;                // PLL divider Q
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;                // PLL divider R
  HAL_RCC_OscConfig(&RCC_OscInitStruct);                     // Apply config

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2; // Select clocks
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;  // System clock = PLL
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;         // AHB divider
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;          // APB1 divider
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;          // APB2 divider
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);  // Apply config
}

static void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};            // Clock config struct
  TIM_MasterConfigTypeDef sMasterConfig = {0};                // Master config struct

  htim3.Instance = TIM3;                                      // Select Timer3
  htim3.Init.Prescaler = 7999;                                // Prescaler
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;                // Up counter
  htim3.Init.Period = 9999;                                   // Auto reload
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;          // Clock division
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // Enable preload
  HAL_TIM_Base_Init(&htim3);                                  // Init timer

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;  // Internal clock
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);     // Apply clock config

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;         // Trigger reset
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;// Disable master/slave
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig); // Apply config
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;                                   // Select USART2
  huart2.Init.BaudRate = 115200;                              // Baud rate
  huart2.Init.WordLength = UART_WORDLENGTH_8B;                // 8 data bits
  huart2.Init.StopBits = UART_STOPBITS_1;                     // 1 stop bit
  huart2.Init.Parity = UART_PARITY_NONE;                      // No parity
  huart2.Init.Mode = UART_MODE_TX_RX;                         // TX and RX
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;                // No flow control
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;            // Oversampling
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;   // Disable one-bit
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;// No advanced features
  HAL_UART_Init(&huart2);                                     // Init UART
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};                     // GPIO config struct

  __HAL_RCC_GPIOC_CLK_ENABLE();                               // Enable GPIOC
  __HAL_RCC_GPIOH_CLK_ENABLE();                               // Enable GPIOH
  __HAL_RCC_GPIOA_CLK_ENABLE();                               // Enable GPIOA
  __HAL_RCC_GPIOB_CLK_ENABLE();                               // Enable GPIOB

  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);  // Reset LED pin

  GPIO_InitStruct.Pin = LD2_Pin;                              // Select LD2 pin
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;                 // Push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL;                         // No pull
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;                // Low speed
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);             // Init LED GPIO
}

void Error_Handler(void)
{
  __disable_irq();                                            // Disable interrupts
  while (1)                                                   // Infinite loop
  {
    // Stay here if error occurs
  }
}
