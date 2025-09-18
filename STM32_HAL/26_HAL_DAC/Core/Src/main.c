#include "main.h"
#include <stdio.h>

ADC_HandleTypeDef hadc1;   // ADC handle
DAC_HandleTypeDef hdac1;   // DAC handle
UART_HandleTypeDef huart2; // UART handle

void SystemClock_Config(void);
void MX_ADC1_Init(void);
void MX_DAC1_Init(void);
void MX_USART2_UART_Init(void);
void MX_GPIO_Init(void);

// printf redirection to UART2
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY); // send char
    return ch; // return character
}

int main(void)
{
    HAL_Init();                  // HAL init
    SystemClock_Config();        // clock config
    MX_GPIO_Init();              // GPIO init
    MX_USART2_UART_Init();       // UART init
    MX_DAC1_Init();              // DAC init
    MX_ADC1_Init();              // ADC init

    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1); // enable DAC

    while (1)
    {
        for (uint32_t val = 0; val <= 4095; val += 512) // DAC sweep
        {
            HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, val); // set DAC
            HAL_Delay(10); // wait

            HAL_ADC_Start(&hadc1);                            // start ADC
            HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // wait ADC
            uint32_t adcVal = HAL_ADC_GetValue(&hadc1);       // read ADC

            float voltage = (3.3f * adcVal) / 4095.0f;        // calc voltage

            printf("DAC: %lu -> ADC: %lu -> Voltage: %.2f V\r\n",
                   val, adcVal, voltage); // print

            HAL_Delay(500); // delay
        }
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0}; // osc config
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}; // clk config

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1); // power scale

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // HSI
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                   // HSI on
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // calib
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;               // PLL on
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;       // PLL src HSI
    RCC_OscInitStruct.PLL.PLLM = 1;                            // PLLM=1
    RCC_OscInitStruct.PLL.PLLN = 10;                           // PLLN=10
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;                // PLLP=7
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;                // PLLQ=2
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;                // PLLR=2
    HAL_RCC_OscConfig(&RCC_OscInitStruct);                     // apply osc

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2; // clock types
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;  // sysclk PLL
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;         // AHB=1
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;          // APB1=1
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;          // APB2=1
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);  // apply clk
}

void MX_ADC1_Init(void)
{
    ADC_MultiModeTypeDef multimode = {0};   // multi mode
    ADC_ChannelConfTypeDef sConfig = {0};   // channel config

    hadc1.Instance = ADC1;                               // ADC1
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;    // no prescaler
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;          // 12-bit
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;          // right align
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;          // single channel
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;       // single conv
    hadc1.Init.ContinuousConvMode = DISABLE;             // single shot
    hadc1.Init.NbrOfConversion = 1;                      // 1 channel
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;    // sw trigger
    HAL_ADC_Init(&hadc1);                                // init ADC

    multimode.Mode = ADC_MODE_INDEPENDENT;               // independent
    HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode);// apply mode

    sConfig.Channel = ADC_CHANNEL_5;                     // channel 5
    sConfig.Rank = ADC_REGULAR_RANK_1;                   // rank 1
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;     // sample time
    sConfig.SingleDiff = ADC_SINGLE_ENDED;               // single end
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);             // apply config
}

void MX_DAC1_Init(void)
{
    DAC_ChannelConfTypeDef sConfig = {0}; // DAC config

    hdac1.Instance = DAC1;                // DAC1
    HAL_DAC_Init(&hdac1);                 // init DAC

    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;       // disable S/H
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;                      // no trigger
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;          // buffer on
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_ENABLE;// internal conn
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;             // factory trim
    HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1);      // config DAC
}

void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;               // UART2
    huart2.Init.BaudRate = 115200;          // baud
    huart2.Init.WordLength = UART_WORDLENGTH_8B; // 8-bit
    huart2.Init.StopBits = UART_STOPBITS_1;      // 1 stop
    huart2.Init.Parity = UART_PARITY_NONE;       // no parity
    huart2.Init.Mode = UART_MODE_TX_RX;          // TX/RX
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE; // no flow
    huart2.Init.OverSampling = UART_OVERSAMPLING_16; // oversample
    HAL_UART_Init(&huart2);                 // init UART
}

void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0}; // GPIO config

    __HAL_RCC_GPIOC_CLK_ENABLE();   // clk GPIOC
    __HAL_RCC_GPIOH_CLK_ENABLE();   // clk GPIOH
    __HAL_RCC_GPIOA_CLK_ENABLE();   // clk GPIOA
    __HAL_RCC_GPIOB_CLK_ENABLE();   // clk GPIOB

    GPIO_InitStruct.Pin = LD2_Pin;                 // LED pin
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;    // output
    GPIO_InitStruct.Pull = GPIO_NOPULL;            // no pull
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;   // low speed
    HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);// init LED

    GPIO_InitStruct.Pin = B1_Pin;                  // button pin
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;   // falling edge
    GPIO_InitStruct.Pull = GPIO_NOPULL;            // no pull
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct); // init button
}

void Error_Handler(void)
{
    __disable_irq(); // disable IRQ
    while (1)
    {

    }     // infinite loop
}
