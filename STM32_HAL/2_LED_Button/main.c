#include "main.h"   // Include main header

UART_HandleTypeDef huart2;   // UART2 handle

void SystemClock_Config(void);   // Function prototype
static void MX_GPIO_Init(void);  // Function prototype
static void MX_USART2_UART_Init(void);  // Function prototype

int main(void)
{
  HAL_Init();                 // Init HAL
  SystemClock_Config();       // Setup system clock
  MX_GPIO_Init();             // Init GPIO
  MX_USART2_UART_Init();      // Init UART2

  while (1)   // Infinite loop
  {
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)   // Check button press
    {
      HAL_GPIO_WritePin(GPIOA, LD2_Pin, GPIO_PIN_SET);   // Turn LED ON
      while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET); // Wait for release
    }
    else
    {
      HAL_GPIO_WritePin(GPIOA, LD2_Pin, GPIO_PIN_RESET); // Turn LED OFF
    }

    HAL_Delay(1000); // 1s delay
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};   // Oscillator config struct
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};   // Clock config struct

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)  // Power scaling
    Error_Handler();

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;   // Use HSI
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;   // Turn HSI on
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;   // Default calibration
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;   // Enable PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;   // PLL source HSI
  RCC_OscInitStruct.PLL.PLLM = 1;   // PLLM divider
  RCC_OscInitStruct.PLL.PLLN = 10;  // PLLN multiplier
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;   // PLLP divider
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;   // PLLQ divider
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;   // PLLR divider
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)   // Apply oscillator config
    Error_Handler();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;   // Select clocks
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;   // SYSCLK from PLL
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;   // AHB divider
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;   // APB1 divider
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;   // APB2 divider

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)   // Apply clock config
    Error_Handler();
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;   // Use USART2
  huart2.Init.BaudRate = 115200;   // Set baudrate
  huart2.Init.WordLength = UART_WORDLENGTH_8B;   // 8 data bits
  huart2.Init.StopBits = UART_STOPBITS_1;   // 1 stop bit
  huart2.Init.Parity = UART_PARITY_NONE;   // No parity
  huart2.Init.Mode = UART_MODE_TX_RX;   // Enable TX & RX
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;   // No flow control
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;   // Oversampling 16
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;   // No one-bit sample
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;   // No extra features
  if (HAL_UART_Init(&huart2) != HAL_OK)   // Init UART
    Error_Handler();
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};   // GPIO struct

  __HAL_RCC_GPIOC_CLK_ENABLE();   // Enable port C clock
  __HAL_RCC_GPIOH_CLK_ENABLE();   // Enable port H clock
  __HAL_RCC_GPIOA_CLK_ENABLE();   // Enable port A clock
  __HAL_RCC_GPIOB_CLK_ENABLE();   // Enable port B clock

  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);   // LED OFF initially

  GPIO_InitStruct.Pin = GPIO_PIN_13;   // Select pin 13
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;   // Set as input
  GPIO_InitStruct.Pull = GPIO_NOPULL;   // No pull-up/down
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);   // Init PC13

  GPIO_InitStruct.Pin = LD2_Pin;   // Select LED pin
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   // Output mode
  GPIO_InitStruct.Pull = GPIO_NOPULL;   // No pull
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;   // Low speed
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);   // Init PA5
}

void Error_Handler(void)
{
  __disable_irq();   // Disable interrupts
  while (1)   // Infinite loop
  {
  }
}
