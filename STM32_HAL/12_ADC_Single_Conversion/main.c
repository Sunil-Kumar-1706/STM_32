#include "main.h"
#include <stdio.h>

// Create ADC and UART handler structures
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

// Variable to store ADC data
uint32_t data = 0;

// Redirect printf output to UART2
int __io_putchar(int ch) {
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY); // Send one character over UART2
  return ch; // Return the sent character
}

int main(void)
{
  HAL_Init();                 // Initialize HAL library (reset peripherals, init Flash, SysTick)
  SystemClock_Config();       // Configure system clock
  MX_GPIO_Init();             // Initialize GPIO pins
  MX_USART2_UART_Init();      // Initialize UART2
  MX_ADC1_Init();             // Initialize ADC1

  HAL_ADC_Start(&hadc1);      // Start ADC conversion

  while (1)                   // Infinite loop
  {
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // Wait until ADC conversion completes
    data = HAL_ADC_GetValue(&hadc1);                  // Read ADC conversion result
    printf("ADC Value: %lu\r\n", data);               // Print ADC value via UART
    HAL_Delay(500);                                   // Wait 500ms before next read
  }
}

// Configure system clock
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0}; // Oscillator config structure
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}; // Clock config structure

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1); // Set voltage scaling

  // Configure internal oscillator (HSI) and PLL
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; 
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                    
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  // Configure AHB, APB bus clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // Use PLL as system clock
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        // AHB = SYSCLK
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;         // APB1 = HCLK
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         // APB2 = HCLK
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4); // Apply config
}

// Initialize ADC1
static void MX_ADC1_Init(void)
{
  ADC_MultiModeTypeDef multimode = {0}; // For ADC multi-mode
  ADC_ChannelConfTypeDef sConfig = {0}; // ADC channel config

  hadc1.Instance = ADC1;                          // Use ADC1
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1; // No prescaler
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;     // 12-bit resolution
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;     // Align result to right
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;     // Single channel
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;  // End of one conversion
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;        // One conversion at a time
  hadc1.Init.NbrOfConversion = 1;                 // One channel conversion
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // Software start
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  HAL_ADC_Init(&hadc1);

  multimode.Mode = ADC_MODE_INDEPENDENT;           // Independent ADC mode
  HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode);

  sConfig.Channel = ADC_CHANNEL_5;                 // Select channel 5
  sConfig.Rank = ADC_REGULAR_RANK_1;               // First rank
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5; // Sampling time
  sConfig.SingleDiff = ADC_SINGLE_ENDED;           // Single-ended input
  sConfig.OffsetNumber = ADC_OFFSET_NONE;          // No offset
  sConfig.Offset = 0;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

// Initialize UART2
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;                        // Use USART2
  huart2.Init.BaudRate = 115200;                   // Baud rate 115200
  huart2.Init.WordLength = UART_WORDLENGTH_8B;     // 8-bit data
  huart2.Init.StopBits = UART_STOPBITS_1;          // 1 stop bit
  huart2.Init.Parity = UART_PARITY_NONE;           // No parity
  huart2.Init.Mode = UART_MODE_TX_RX;              // Enable TX and RX
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;     // No flow control
  huart2.Init.OverSampling = UART_OVERSAMPLING_16; // Oversampling by 16
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart2);
}

// Initialize GPIO
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC clock
  __HAL_RCC_GPIOH_CLK_ENABLE(); // Enable GPIOH clock
  __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock
  __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable GPIOB clock

  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); // Set LED pin low

  GPIO_InitStruct.Pin = LD2_Pin;            // Select LED pin
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL;         // No pull-up or pull-down
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct); // Initialize LED pin
}

// Error handler
void Error_Handler(void)
{
  __disable_irq(); // Disable interrupts
  while (1)        // Stay here in case of error
  {
  }
}
