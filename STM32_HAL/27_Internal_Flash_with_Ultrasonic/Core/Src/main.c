#include "main.h"                 // Include main HAL header
#include <stdio.h>                // For sprintf/snprintf
#include <string.h>               // For strlen
#include <stdlib.h>               // For standard functions

#define FLASH_USER_START_ADDR  0x080FF800UL  // Flash memory start address for user data
#define FLASH_PAGE_SIZE        2048          // Flash page size in bytes
#define NUM_SLOTS              (FLASH_PAGE_SIZE / 8) // Number of 8-byte slots per page

TIM_HandleTypeDef htim2;          // Timer2 handle
UART_HandleTypeDef huart2;        // UART2 handle

uint32_t ic_val1 = 0;             // First input capture value
uint32_t ic_val2 = 0;             // Second input capture value
uint32_t diff = 0;                // Difference between captures
uint8_t is_first_capture = 0;     // Capture state flag
uint32_t distance_cm = 0;         // Measured distance in cm

void SystemClock_Config(void);    // System clock configuration prototype
static void MX_GPIO_Init(void);   // GPIO initialization prototype
static void MX_USART2_UART_Init(void); // UART2 initialization prototype
static void MX_TIM2_Init(void);   // Timer2 initialization prototype

void HCSR04_Trigger(void)         // Send trigger pulse to ultrasonic sensor
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); // Set trigger pin high
  for (volatile int i = 0; i < 160; i++);             // Short delay (~10µs)
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // Set trigger pin low
}

void Start_HCSR04(void)           // Start ultrasonic measurement
{
  is_first_capture = 0;           // Reset capture state
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); // Start input capture with interrupt
  HCSR04_Trigger();               // Send trigger pulse
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) // Callback for capture event
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) // Check channel
  {
    if (is_first_capture == 0)   // First edge detected
    {
      ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // Read first value
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING); // Switch to falling edge
      is_first_capture = 1;      // Mark first capture done
    }
    else if (is_first_capture == 1) // Second edge detected
    {
      ic_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // Read second value
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); // Reset to rising edge
      HAL_TIM_IC_Stop_IT(htim, TIM_CHANNEL_1); // Stop capture interrupt
      if (ic_val2 > ic_val1)     // If no overflow
        diff = ic_val2 - ic_val1;
      else                       // If overflow occurred
        diff = (0xFFFF - ic_val1 + ic_val2);
      distance_cm = (float)diff * 0.0343 / 2.0; // Convert ticks to distance in cm
    }
  }
}

void Flash_Write(uint32_t addr, uint32_t data) // Write 32-bit data to flash
{
    HAL_FLASH_Unlock();           // Unlock flash
    uint64_t data64 = data;       // Store data in 64-bit variable (flash writes in 8 bytes)
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, data64); // Write doubleword
    HAL_FLASH_Lock();             // Lock flash
}

uint32_t Flash_Read(uint32_t addr) // Read 32-bit value from flash
{
    return *(uint32_t*)addr;      // Return value at given address
}

void Flash_Erase_Page(uint32_t pageAddr) // Erase one flash page
{
    HAL_FLASH_Unlock();           // Unlock flash
    FLASH_EraseInitTypeDef eraseInit; // Erase structure
    uint32_t pageError = 0;       // Error variable
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES; // Page erase mode
    eraseInit.Page = (pageAddr - FLASH_BASE) / FLASH_PAGE_SIZE; // Calculate page number
    eraseInit.NbPages = 1;        // Erase one page
    eraseInit.Banks = FLASH_BANK_2; // Use bank 2
    HAL_FLASHEx_Erase(&eraseInit, &pageError); // Perform erase
    HAL_FLASH_Lock();             // Lock flash
}

int main(void)                    // Main program
{
  HAL_Init();                     // Initialize HAL
  SystemClock_Config();           // Configure system clock
  MX_GPIO_Init();                 // Initialize GPIO
  MX_USART2_UART_Init();          // Initialize UART2
  MX_TIM2_Init();                 // Initialize Timer2
  char msg[64];                   // UART message buffer
  uint32_t addr = FLASH_USER_START_ADDR; // Start flash address
  Flash_Erase_Page(FLASH_USER_START_ADDR); // Erase flash page before writing

  while (1)                       // Main loop
  {
    Start_HCSR04();               // Start distance measurement
    for (volatile int i = 0; i < 10000; i++); // Small delay for capture

    snprintf(msg, sizeof(msg), "Measured Distance: %lu cm\r\n", distance_cm); // Format distance
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY); // Send over UART

    Flash_Write(addr, distance_cm); // Write distance to flash
    uint32_t readData = Flash_Read(addr); // Read back stored value
    snprintf(msg, sizeof(msg), "Sensor: %lu, Addr: 0x%08lX\r\n", readData, addr); // Format log
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY); // Send over UART

    addr += 8;                    // Move to next 8-byte slot
    if(addr >= FLASH_USER_START_ADDR + FLASH_PAGE_SIZE) // If page is full
    {
        Flash_Erase_Page(FLASH_USER_START_ADDR); // Erase page
        addr = FLASH_USER_START_ADDR; // Reset address
    }
    HAL_Delay(500);               // Wait 500ms before next reading
  }
}

void SystemClock_Config(void)     // Configure system clock
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0}; // Oscillator config
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}; // Clock config

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) // Set voltage scaling
  {
    Error_Handler();              // Error if failed
  }
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // Use HSI oscillator
  RCC_OscInitStruct.HSIState = RCC_HSI_ON; // Turn on HSI
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // Default calibration
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON; // Enable PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI; // PLL source = HSI
  RCC_OscInitStruct.PLL.PLLM = 1; // PLLM divider
  RCC_OscInitStruct.PLL.PLLN = 10; // PLLN multiplier
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7; // PLLP divider
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2; // PLLQ divider
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2; // PLLR divider
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) // Apply oscillator config
  {
    Error_Handler();              // Error if failed
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2; // Select clocks
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // System clock from PLL
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1; // No division for AHB
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1; // No division for APB1
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1; // No division for APB2
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) // Apply clock config
  {
    Error_Handler();              // Error if failed
  }
}

static void MX_TIM2_Init(void)    // Initialize Timer2
{
  TIM_MasterConfigTypeDef sMasterConfig = {0}; // Master config
  TIM_IC_InitTypeDef sConfigIC = {0};          // Input capture config

  htim2.Instance = TIM2;          // Use TIM2
  htim2.Init.Prescaler = 0;       // No prescaler
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP; // Count up
  htim2.Init.Period = 4294967295; // Max 32-bit period
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // No division
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; // Disable preload
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK) // Initialize timer input capture
  {
    Error_Handler();              // Error if failed
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET; // Reset trigger
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; // Disable master/slave
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) // Apply config
  {
    Error_Handler();              // Error if failed
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING; // Capture on rising edge
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI; // Direct input
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1; // No prescaler
  sConfigIC.ICFilter = 0;        // No filter
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK) // Configure channel
  {
    Error_Handler();              // Error if failed
  }
}

static void MX_USART2_UART_Init(void) // Initialize UART2
{
  huart2.Instance = USART2;       // Use USART2
  huart2.Init.BaudRate = 115200;  // Baud rate = 115200
  huart2.Init.WordLength = UART_WORDLENGTH_8B; // 8-bit data
  huart2.Init.StopBits = UART_STOPBITS_1; // 1 stop bit
  huart2.Init.Parity = UART_PARITY_NONE; // No parity
  huart2.Init.Mode = UART_MODE_TX_RX; // Enable TX and RX
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE; // No hardware flow control
  huart2.Init.OverSampling = UART_OVERSAMPLING_16; // 16x oversampling
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE; // Disable one-bit sampling
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; // No advanced features
  if (HAL_UART_Init(&huart2) != HAL_OK) // Initialize UART
  {
    Error_Handler();              // Error if failed
  }
}

static void MX_GPIO_Init(void)    // Initialize GPIO
{
  GPIO_InitTypeDef GPIO_InitStruct = {0}; // GPIO config structure

  __HAL_RCC_GPIOC_CLK_ENABLE();   // Enable GPIOC clock
  __HAL_RCC_GPIOH_CLK_ENABLE();   // Enable GPIOH clock
  __HAL_RCC_GPIOA_CLK_ENABLE();   // Enable GPIOA clock
  __HAL_RCC_GPIOB_CLK_ENABLE();   // Enable GPIOB clock

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // Set PA1 low initially

  GPIO_InitStruct.Pin = B1_Pin;   // Configure button pin
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // Falling edge interrupt
  GPIO_InitStruct.Pull = GPIO_NOPULL; // No pull-up/down
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct); // Initialize button

  GPIO_InitStruct.Pin = GPIO_PIN_1; // Configure PA1 as output
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL; // No pull
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); // Initialize PA1
}

void Error_Handler(void)          // Error handler
{
  __disable_irq();                // Disable interrupts
  while (1)                       // Infinite loop
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) // Assertion failure handler
{
}
#endif
