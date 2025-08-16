#include "main.h"     // Main HAL header
#include <stdio.h>    // Standard I/O for printf

ADC_HandleTypeDef hadc1;   // Handle for ADC1
UART_HandleTypeDef huart2; // Handle for UART2

uint32_t data = 0;   // Variable to store ADC value

// Function to redirect printf to UART
int __io_putchar(int ch) {
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY); // Send one character over UART
  return ch; // Return the character
}

int main(void) {
  HAL_Init();             // Initialize HAL (reset peripherals, setup SysTick)
  SystemClock_Config();   // Configure system clock
  MX_GPIO_Init();         // Initialize GPIO
  MX_USART2_UART_Init();  // Initialize UART2
  MX_ADC1_Init();         // Initialize ADC1

  HAL_ADC_Start(&hadc1);  // Start ADC conversion

  while (1) {
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // Wait until ADC conversion finishes
    data = HAL_ADC_GetValue(&hadc1);                 // Read ADC result
    printf("ADC Value: %lu\r\n", data);              // Print ADC value over UART
    HAL_Delay(500);                                  // Delay 500ms
  }
}

// Configure system clock
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1); // Set voltage scaling

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // Use HSI oscillator
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                   // Enable HSI
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // Default calibration
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;               // Enable PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;       // Use HSI as PLL source
  RCC_OscInitStruct.PLL.PLLM = 1;                            // PLLM divider
  RCC_OscInitStruct.PLL.PLLN = 10;                           // PLLN multiplier
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;                // PLLP divider
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;                // PLLQ divider
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;                // PLLR divider
  HAL_RCC_OscConfig(&RCC_OscInitStruct);                     // Apply oscillator config

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2; // Configure bus clocks
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;  // Use PLL as system clock
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;         // AHB clock = SYSCLK
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;          // APB1 clock = HCLK
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;          // APB2 clock = HCLK
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);  // Apply clock config
}

// Initialize ADC1
static void MX_ADC1_Init(void) {
  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;                       // Use ADC1
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1; // No clock division
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;  // 12-bit resolution
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;  // Right aligned data
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;  // Single channel only
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV; // End of single conversion
  hadc1.Init.LowPowerAutoWait = DISABLE;       // Disable low power wait
  hadc1.Init.ContinuousConvMode = ENABLE;      // Enable continuous conversion
  hadc1.Init.NbrOfConversion = 1;              // Only 1 channel
  hadc1.Init.DiscontinuousConvMode = DISABLE;  // Disable discontinuous mode
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // Software trigger
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; // No external trigger
  hadc1.Init.DMAContinuousRequests = DISABLE;  // Disable DMA
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED; // Preserve old data on overrun
  hadc1.Init.OversamplingMode = DISABLE;       // Disable oversampling
  HAL_ADC_Init(&hadc1);                        // Initialize ADC

  multimode.Mode = ADC_MODE_INDEPENDENT;       // Independent mode
  HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode);

  sConfig.Channel = ADC_CHANNEL_5;             // Use channel 5
  sConfig.Rank = ADC_REGULAR_RANK_1;           // First rank
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5; // Sampling time
  sConfig.SingleDiff = ADC_SINGLE_ENDED;       // Single-ended input
  sConfig.OffsetNumber = ADC_OFFSET_NONE;      // No offset
  sConfig.Offset = 0;                          // Offset = 0
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);     // Apply channel config
}

// Initialize UART2
static void MX_USART2_UART_Init(void) {
  huart2.Instance = USART2;                 // Use USART2
  huart2.Init.BaudRate = 115200;            // Baudrate 115200
  huart2.Init.WordLength = UART_WORDLENGTH_8B; // 8 data bits
  huart2.Init.StopBits = UART_STOPBITS_1;   // 1 stop bit
  huart2.Init.Parity = UART_PARITY_NONE;    // No parity
  huart2.Init.Mode = UART_MODE_TX_RX;       // Enable TX and RX
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE; // No flow control
  huart2.Init.OverSampling = UART_OVERSAMPLING_16; // Oversampling 16
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE; // Disable 1-bit sampling
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; // No advanced features
  HAL_UART_Init(&huart2);                   // Initialize UART
}

// Initialize GPIO
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable clock for GPIOC
  __HAL_RCC_GPIOH_CLK_ENABLE(); // Enable clock for GPIOH
  __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable clock for GPIOA
  __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable clock for GPIOB

  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); // Set LED off

  GPIO_InitStruct.Pin = LD2_Pin;              // Select LED2 pin
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL;         // No pull-up/pull-down
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct); // Initialize LED2
}

// Error handler (infinite loop)
void Error_Handler(void) {
  __disable_irq(); // Disable interrupts
  while (1) { }    // Stay here forever
}
