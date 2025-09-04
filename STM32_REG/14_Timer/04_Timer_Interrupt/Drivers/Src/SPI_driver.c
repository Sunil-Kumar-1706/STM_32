#include "SPI_driver.h"

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHANDLE);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHANDLE);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHANDLE);

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

void SPI_DeInit(SPI_RegDef_t *pSPIx);

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

/*IRQ and ISR  configuration */
void SPI_IRQInterruptConfig(uint8_t IRQNumber,  uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			*NVIC_ISER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			*NVIC_ISER3 |= ( 1 << (IRQNumber % 64) );
		}
	}
	else
	{
		if(IRQNumber <= 31)
		{
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 6 && IRQNumber < 96 )
		{
			*NVIC_ICER3 |= ( 1 << (IRQNumber % 64) );
		}
	}
}

void SPI_PRIORITY_CONFIG(uint8_t IRQNumber,uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;

	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );
}

void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
	uint8_t temp1,temp2;

	temp1=pHandle->pSPIx->SR &(1<<1);
	temp2=pHandle->pSPIx->CR2 &(1<<7);

	if(temp1 & temp2)
	{
		spi_txe_interrupt_handle(pHandle);
	}

	temp1=pHandle->pSPIx->SR &(1<<0);
	temp2=pHandle->pSPIx->CR2 &(1<<6);

	if(temp1 & temp2)
	{
		spi_rxne_interrupt_handle(pHandle);
	}

	temp1=pHandle->pSPIx->SR &(1<<6);
	temp2=pHandle->pSPIx->CR2 &(1<<5);
	if(temp1 & temp2)
	{
		spi_ovr_err_interrupt_handle(pHandle);
	}
}

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
	pSPIHandle->TxLen--;
	pSPIHandle->pTxBuffer++;

	if(!pSPIHandle ->TxLen)
	{
		SPI_CloseTransmisson(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}
}

static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	*(pSPIHandle->pRxBuffer)=(uint8_t)pSPIHandle->pSPIx->DR;
	pSPIHandle->RxLen--;
	pSPIHandle->pRxBuffer++;

		if(!pSPIHandle ->RxLen)
		{
			SPI_CloseReception(pSPIHandle);
			SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
		}
}

static void  spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;
	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1 << 7);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1 << 6);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

__attribute__((weak))void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
{

}
