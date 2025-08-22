#include "SPI_driver.h"

/*peripheral clock setup */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx , uint8_t EnorDi)
{
		if(EnorDi == ENABLE)
		{
			if(pSPIx == SPI1)
			{
				SPI1_PCLK_EN();
			}
			else if(pSPIx==SPI2)
			{
				SPI2_PCLK_EN();
			}
			else if(pSPIx==SPI3)
			{
				SPI3_PCLK_EN();
			}
		}
		else
		{
			if(pSPIx == SPI1)
			{
				SPI1_PCLK_DI();
			}
			else if(pSPIx==SPI2)
			{
				SPI2_PCLK_DI();
			}
			else if(pSPIx==SPI3)
			{
				SPI3_PCLK_DI();
			}
		}
}

/*Init and Deinit */
void SPI_Init(SPI_Handle_t *pSPIHANDLE)
{
	uint32_t tempreg=0;
	SPI_PeriClockControl(pSPIHANDLE->pSPIx, ENABLE);

	tempreg |=pSPIHANDLE->SPIConfig.SPI_DeviceMode<<2;


	if(pSPIHANDLE->SPIConfig.SPI_BusConfig==SPI_BUS_CONFIG_FD)
	{
		tempreg &=~(1<<15);
	}
	else if(pSPIHANDLE->SPIConfig.SPI_BusConfig==SPI_BUS_CONFIG_HD)
	{
		tempreg |=(1<<15);
	}
	else if(pSPIHANDLE->SPIConfig.SPI_BusConfig==SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		tempreg &=~(1<<15);
		tempreg |=(1<<10);
	}
	tempreg |=pSPIHANDLE->SPIConfig.SPI_SclkSpeed<<3;

	tempreg |=pSPIHANDLE->SPIConfig.SPI_CPOL<<1;
	tempreg |=pSPIHANDLE->SPIConfig.SPI_CPHA<<0;

	pSPIHANDLE->pSPIx->CR1 |=tempreg;

	tempreg=0;
	tempreg |=pSPIHANDLE->SPIConfig.SPI_DFF<<8;
	pSPIHANDLE->pSPIx->CR2 |=tempreg;
}

/*read and write */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer,uint32_t Len)
{
	while(Len >0)
	{
		while(((pSPIx->SR >> 1)&1)==0);

		pSPIx ->DR =*pTxBuffer;
		Len--;
		pTxBuffer++;
	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer,uint32_t Len)
{
	while(Len>0)
	{
		while(((pSPIx->SR >>0)&1)==0);
		*(pRxBuffer) = pSPIx -> DR;
		Len--;
		pRxBuffer++;
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t*pSPIx, uint32_t FlagName)
{
	if(pSPIx -> SR &FlagName){
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (1 << 6);
	}else
	{
		pSPIx->CR1 &=  ~(1 << 6);
	}
}

void  SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (1 << 8);
	}else
	{
		pSPIx->CR1 &=  ~(1 << 8);
	}
}

void  SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |=  (1 << 2);
	}else
	{
		pSPIx->CR2 &=  ~(1 << 2);
	}
}


uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX)
	{
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;
		pSPIHandle->TxState = SPI_BUSY_IN_TX;
		pSPIHandle->pSPIx->CR2 |= ( 1 << 7 );
	}
	return state;
}

uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX)
	{
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;
		pSPIHandle->RxState = SPI_BUSY_IN_RX;
		pSPIHandle->pSPIx->CR2 |= ( 1 << 6 );
	}
	return state;
}
