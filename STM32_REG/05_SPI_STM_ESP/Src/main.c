#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "STM32476.h"

// Function prototypes
void delay(void);
void SPI2_GPIOInits(void);

// Function to initialize GPIO pins for SPI2
void SPI2_GPIOInits(void)
{
    GPIO_Handle_t SPIPins;

    // Select GPIOB for SPI2 pins
    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;       // Use pins in Alternate Function mode
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;               // AF5 = SPI2
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;     // Push-pull output
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;   // No pull-up, no pull-down
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;      // High speed for SPI
    GPIO_PeriClockControl(GPIOB, ENABLE);                        // Enable clock for GPIOB

    // NSS (Chip Select) pin -> PB12
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIO_Init(&SPIPins);

    // SCK (Clock) pin -> PB13
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    // MISO (Master In Slave Out) pin -> PB14
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&SPIPins);

    // MOSI (Master Out Slave In) pin -> PB15
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);
}

// Function to initialize SPI2 peripheral
void SPI2_Inits(void)
{
    SPI_Handle_t SPI2Handle;

    SPI2Handle.pSPIx = SPI2;                                     // Select SPI2
    SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER; // Configure as Master
    SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;      // Full-Duplex mode
    SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;   // Set clock speed (PCLK/32)
    SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;                // Data frame = 8 bits
    SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;                // Clock polarity low
    SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;                // Clock phase low
    SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI;                   // Hardware slave management enabled

    SPI_Init(&SPI2Handle);                                       // Initialize SPI2
}

int main(void)
{
    uint8_t data = 10;  // Data to be sent

    SPI2_GPIOInits();                // Configure SPI2 pins
    SPI2_Inits();                    // Configure SPI2 peripheral
    SPI_SSOEConfig(SPI2, ENABLE);    // Enable NSS output (hardware control)
    SPI_PeripheralControl(SPI2, ENABLE); // Enable SPI2 peripheral

    while(1)
    {
        SPI_SendData(SPI2, &data, 1);    // Send 1 byte of data
        while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG)); // Wait until SPI is not busy
        delay();                          // Small delay before sending again
    }
}

// Simple delay function
void delay()
{
    for(volatile int i = 0; i < 500000/2; i++);
}
