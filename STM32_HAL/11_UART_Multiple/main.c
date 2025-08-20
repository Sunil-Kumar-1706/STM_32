#include "main.h"   // include main header file

uint8_t rx1_data;   // variable to store received data from UART1
uint8_t rx2_data;   // variable to store received data from UART2

UART_HandleTypeDef huart1;   // handle for UART1
UART_HandleTypeDef huart2;   // handle for UART2

void SystemClock_Config(void);        // function to configure system clock
static void MX_GPIO_Init(void);       // function to initialize GPIO
static void MX_USART2_UART_Init(void); // function to initialize UART2
static void MX_USART1_UART_Init(void); // function to initialize UART1

// callback function runs when UART receives data
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)   // check if data came from USART1
  {
    HAL_UART_Transmit(&huart1, &rx1_data, 1, 100); // send back received data
    HAL_UART_Receive_IT(&huart1, &rx1_data, 1);    // enable next receive interrupt
  }
  else if (huart->Instance == USART2)  // check if data came from USART2
  {
    HAL_UART_Transmit(&huart2, &rx2_data, 1, 100); // send back received data
    HAL_UART_Receive_IT(&huart2, &rx2_data, 1);    // enable next receive interrupt
  }
}

int main(void)
{
  HAL_Init();                // initialize HAL library
  SystemClock_Config();      // configure system clock
  MX_GPIO_Init();            // initialize GPIO
  MX_USART2_UART_Init();     // initialize UART2
  MX_USART1_UART_Init();     // initialize UART1
  HAL_UART_Receive_IT(&huart1, &rx1_data, 1); // enable UART1 receive interrupt
  HAL_UART_Receive_IT(&huart2, &rx2_data, 1); // enable UART2 receive interrupt

  while (1)   // infinite loop
  {
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};   // oscillator config structure
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};   // clock config structure

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) // set voltage scaling
  {
    Error_Handler();   // go to error handler if failed
  }

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;  // use HSI oscillator
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                   // turn on HSI
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // set calibration
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;               // enable PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;       // PLL source is HSI
  RCC_OscInitStruct.PLL.PLLM = 1;                            // set PLLM
  RCC_OscInitStruct.PLL.PLLN = 10;                           // set PLLN
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;                // set PLLP
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;                // set PLLQ
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;                // set PLLR
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)       // apply oscillator config
  {
    Error_Handler();   // error if config fails
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2; // select clock types
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;   // system clock source is PLL
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;          // set AHB divider
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // set APB1 divider
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // set APB2 divider

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) // apply config
  {
    Error_Handler();   // error if fails
  }
}

static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;                         // select USART1
  huart1.Init.BaudRate = 115200;                    // baud rate
  huart1.Init.WordLength = UART_WORDLENGTH_8B;      // 8 data bits
  huart1.Init.StopBits = UART_STOPBITS_1;           // 1 stop bit
  huart1.Init.Parity = UART_PARITY_NONE;            // no parity
  huart1.Init.Mode = UART_MODE_TX_RX;               // enable TX and RX
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;      // no flow control
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;  // oversampling by 16
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE; // disable one-bit sampling
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; // no advanced feature
  if (HAL_UART_Init(&huart1) != HAL_OK)             // initialize UART1
  {
    Error_Handler();   // error if fails
  }
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;                         // select USART2
  huart2.Init.BaudRate = 115200;                    // baud rate
  huart2.Init.WordLength = UART_WORDLENGTH_8B;      // 8 data bits
  huart2.Init.StopBits = UART_STOPBITS_1;           // 1 stop bit
  huart2.Init.Parity = UART_PARITY_NONE;            // no parity
  huart2.Init.Mode = UART_MODE_TX_RX;               // enable TX and RX
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;      // no flow control
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;  // oversampling by 16
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE; // disable one-bit sampling
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; // no advanced feature
  if (HAL_UART_Init(&huart2) != HAL_OK)             // initialize UART2
  {
    Error_Handler();   // error if fails
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};   // GPIO config structure

  __HAL_RCC_GPIOC_CLK_ENABLE();   // enable clock for GPIOC
  __HAL_RCC_GPIOH_CLK_ENABLE();   // enable clock for GPIOH
  __HAL_RCC_GPIOA_CLK_ENABLE();   // enable clock for GPIOA
  __HAL_RCC_GPIOB_CLK_ENABLE();   // enable clock for GPIOB

  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); // turn off LED

  GPIO_InitStruct.Pin = LD2_Pin;               // select LED2 pin
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // set pin as push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL;          // no pull-up/pull-down
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // set low speed
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct); // initialize LED2 pin
}

void Error_Handler(void)
{
  __disable_irq();   // disable interrupts
  while (1)          // stay here forever
  {
  }
}
