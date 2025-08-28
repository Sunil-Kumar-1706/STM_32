#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include "STM32476.h"
#include "USART_driver.h"
#include "GPIO_driver.h"

// Message to send over UART
uint8_t msg[50] = "HELLO\r\n";

// USART2 handle
USART_Handle_t usart2_handle;

// Simple delay function
void delay(void)
{
    for(uint32_t i = 0 ; i < 250000 ; i++);
}

// Initialize USART2 peripheral
void USART2_Init(void)
{
    usart2_handle.pUSARTx = USART2;
    usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;   // baud rate 115200
    usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE; // no HW flow
    usart2_handle.USART_Config.USART_Mode = USART_MODE_TXRX;          // TX & RX mode
    usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1; // 1 stop bit
    usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS; // 8-bit word
    usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE; // no parity
    USART_Init(&usart2_handle);  // initialize USART2
}

// Initialize GPIO pins for USART2
void USART2_GPIOInit(void)
{
    GPIO_Handle_t usart2_gpio;
    memset(&usart2_gpio,0,sizeof(usart2_gpio));

    usart2_gpio.pGPIOx = GPIOA;
    usart2_gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;      // alternate function
    usart2_gpio.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;     // push-pull
    usart2_gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;   // pull-up
    usart2_gpio.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;     // fast speed
    usart2_gpio.GPIO_PinConfig.GPIO_PinAltFunMode = 7;              // AF7 for USART2

    // TX pin (PA2)
    usart2_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
    GPIOA_PCLK_EN(); // enable clock for GPIOA
    GPIO_Init(&usart2_gpio);

    // RX pin (PA3)
    usart2_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
    GPIO_Init(&usart2_gpio);
}

int main(void)
{
    // LED GPIO configuration
    GPIO_Handle_t GpioLed;
    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;      // LED on PA5
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;        // output mode
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;    // push-pull
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;  // no pull-up/down
    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_Init(&GpioLed);

    // Initialize UART GPIO and peripheral
    USART2_GPIOInit();
    USART2_Init();
    USART_PeripheralControl(USART2, ENABLE);  // enable USART2 peripheral

    // Send initial message over UART
    USART_SendData(&usart2_handle, msg, 19);
    delay();

    // Receive a single byte from UART
    uint8_t c;
    USART_ReceiveData(&usart2_handle, &c, 1);

    // If received byte is 'a', turn on LED
    if(c == 'a')
    {
    	GPIOA->BSRR=1<<5;
        //GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_5, SET);  // turn on LED
        delay();
    }

    // Send back the received byte
    USART_SendData(&usart2_handle, &c, 1);

    while(1);  // infinite loop
}
