#include<stdint.h>
#include<string.h>
#include<stdio.h>
#include"STM32476.h"
#include"GPIO_driver.h"
#include"USART_driver.h"
#include"ADC.h"

USART_Handle_t usart2_handle;  // USART2 handle structure

// Function to initialize USART2 peripheral
void USART2_Init(void)
{
	usart2_handle.pUSARTx = USART2;                          // select USART2 peripheral
	usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;   // set baud rate 115200
	usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE; // no hardware flow control
	usart2_handle.USART_Config.USART_Mode = USART_MODE_TXRX; // enable TX and RX
	usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1; // 1 stop bit
	usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS; // 8-bit data
	usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE; // no parity
	USART_Init(&usart2_handle);                             // initialize USART2 with settings
}

// Function to configure GPIO pins for USART2
void USART2_GPIOInit(void)
{
	GPIO_Handle_t usart2_gpio;                            // GPIO handle structure
	memset(&usart2_gpio,0,sizeof(usart2_gpio));           // clear structure

	usart2_gpio.pGPIOx = GPIOA;                           // use GPIOA for USART2
	usart2_gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;  // alternate function mode
	usart2_gpio.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; // push-pull
	usart2_gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU; // pull-up
	usart2_gpio.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;  // fast speed
	usart2_gpio.GPIO_PinConfig.GPIO_PinAltFunMode = 7;     // AF7 = USART2

	usart2_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;   // PA2 = USART2_TX
	GPIOA_PCLK_EN();                                           // enable GPIOA clock
	GPIO_Init(&usart2_gpio);                                   // initialize PA2

	usart2_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;   // PA3 = USART2_RX
	GPIO_Init(&usart2_gpio);                                   // initialize PA3
}

// Simple delay function
void delay2(void)
{
	for(uint32_t i = 0 ; i < 250000 ; i ++);
}

uint16_t adc_val;  // variable to hold ADC result

int main(void)
{
	char msg[30];   // buffer for message string

    USART2_GPIOInit();                      // init GPIO pins for USART2
    USART2_Init();                          // init USART2
    USART_PeripheralControl(USART2, ENABLE); // enable USART2 peripheral
    adc_init();                             // initialize ADC
    start_conversion();                     // start ADC conversion

    while (1)
    {
        adc_val = adc_read();               // read ADC value (0–4095 for 12-bit ADC)

        sprintf(msg,"ADC-Value: %u\r\n", adc_val); // format ADC value into string

        USART_SendData(&usart2_handle, (uint8_t*) msg, strlen(msg)); // send string via USART2

        delay2();                           // small delay between transmissions
    }
}
