#ifndef INC_SPI_DRIVER_H_
#define INC_SPI_DRIVER_H_

#include<stdint.h>
#include "STM32476.h"

/*Configuration Structure for SPI*/
typedef struct{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_Config_t;

/*Handle Structure for SPI*/
typedef struct{
	SPI_RegDef_t  *pSPIx;
	SPI_Config_t SPIConfig;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxState;
	uint8_t RxState;
}SPI_Handle_t;

/*Device mode*/
#define SPI_DEVICE_MODE_MASTER				1
#define SPI_DEVICE_MODE_SLAVE				0

/*bus config*/
#define SPI_BUS_CONFIG_FD					1
#define SPI_BUS_CONFIG_HD					2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY		3

/*clk speed*/
#define SPI_SCLK_SPEED_DIV2					0
#define SPI_SCLK_SPEED_DIV4					1
#define SPI_SCLK_SPEED_DIV8					2
#define SPI_SCLK_SPEED_DIV16				3
#define SPI_SCLK_SPEED_DIV32				4
#define SPI_SCLK_SPEED_DIV64				5
#define SPI_SCLK_SPEED_DIV128				6
#define SPI_SCLK_SPEED_DIV256				7

/*dff BITS*/
#define SPI_DFF_8BITS						0
#define SPI_DFF_16BITS						1

/*CPOL */
#define SPI_CPOL_HIGH						1
#define SPI_CPOL_LOW						0

/*CPHA*/
#define SPI_CPHA_HIGH						1
#define SPI_CPHA_LOW						0

/*SPI SSM*/
#define SPI_SSM_DI							0
#define SPI_SSM_EN							1

/*peripheral clock setup */
void SPI_PeriClockControl(SPI_RegDef_t *pGPIOx , uint8_t EnorDi);

/*Init and Deinit */
void SPI_Init(SPI_Handle_t *pSPIHANDLE);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*read and write */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer,uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer,uint32_t Len);

/*IRQ and ISR  configuration */
void SPI_IRQInterruptConfig(uint8_t IRQNumber,  uint8_t EnorDi);
void SPI_PRIORITY_CONFIG(uint8_t IRQNumber,uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

#endif /* INC_SPI_DRIVER_H_ */
