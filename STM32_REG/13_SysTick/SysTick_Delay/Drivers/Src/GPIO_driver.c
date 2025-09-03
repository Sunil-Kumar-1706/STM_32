#include "GPIO_driver.h"

/*peripheral clock setup */
void GPIO_PeriClockControl(GPIO_Regdef_t *pGPIOx , uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx==GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx==GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx==GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx==GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx==GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx==GPIOG)
		{
			GPIOG_PCLK_EN();
		}
		else if(pGPIOx==GPIOH)
		{
			GPIOH_PCLK_EN();
		}
	}
	else
	{
		if(pGPIOx == GPIOA)
				{
					GPIOA_PCLK_DI();
				}
				else if(pGPIOx==GPIOB)
				{
					GPIOB_PCLK_DI();
				}
				else if(pGPIOx==GPIOC)
				{
					GPIOC_PCLK_DI();
				}
				else if(pGPIOx==GPIOD)
				{
					GPIOD_PCLK_DI();
				}
				else if(pGPIOx==GPIOE)
				{
					GPIOE_PCLK_DI();
				}
				else if(pGPIOx==GPIOF)
				{
					GPIOF_PCLK_DI();
				}
				else if(pGPIOx==GPIOG)
				{
					GPIOG_PCLK_DI();
				}
				else if(pGPIOx==GPIOH)
				{
					GPIOH_PCLK_DI();
				}
	}
}

/*Init and Deinit */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp=0;
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		temp=( pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0X03 << (2* pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle ->pGPIOx->MODER |= temp;
		temp=0;
	}
	else{
		temp=( pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber));
				pGPIOHandle->pGPIOx->MODER &= ~(0X03 << (2* pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber));
				pGPIOHandle ->pGPIOx->MODER |= temp;
				temp=0;
		if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			EXTI->FTSR1 |=1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;

			EXTI->RTSR1 &=~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			EXTI->RTSR1 |=1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;

			EXTI->FTSR1 &=~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}
		else if(pGPIOHandle -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			EXTI->FTSR1 |=1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;

			EXTI->RTSR1 |=1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
		}
	 uint8_t temp1=pGPIOHandle ->GPIO_PinConfig.GPIO_PinNumber/4;
	 uint8_t temp2=pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber%4;
	 uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
	 SYSCFG_PCLK_EN();
	 SYSCFG->EXTICR[temp1]=portcode << (temp2 * 4);
	 EXTI -> IMR1 |=1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	}

	temp=0;
	temp=(pGPIOHandle -> GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle ->pGPIOx->OSPEEDR &= ~(0X03 << (2* pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	temp=0;

	temp=(pGPIOHandle -> GPIO_PinConfig.GPIO_PinPuPdControl<< (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle ->pGPIOx->PUPDR &= ~(0X03 << (2* pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	temp=0;

	temp=(pGPIOHandle -> GPIO_PinConfig.GPIO_PinOPType<<  pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle ->pGPIOx->OTYPER &= ~(0X01 << ( pGPIOHandle -> GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	temp=0;

	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		uint32_t temp1 ,temp2;
		temp1=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/8;
		temp2=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0X0f<< (4*temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4*temp2));
	}
}
void GPIO_DeInit(GPIO_Regdef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
			{
				GPIOA_REG_RESET();
			}
			else if(pGPIOx==GPIOB)
			{
				GPIOB_REG_RESET();
			}
			else if(pGPIOx==GPIOC)
			{
				GPIOC_REG_RESET();
			}
			else if(pGPIOx==GPIOD)
			{
				GPIOD_REG_RESET();
			}
			else if(pGPIOx==GPIOE)
			{
				GPIOE_REG_RESET();
			}
			else if(pGPIOx==GPIOF)
			{
				GPIOF_REG_RESET();
			}
			else if(pGPIOx==GPIOG)
			{
				GPIOG_REG_RESET();
			}
			else if(pGPIOx==GPIOH)
			{
				GPIOH_REG_RESET();
			}
}

/*read and write */

uint8_t  GPIO_ReadFromInputPin(GPIO_Regdef_t *pGPIOx , uint8_t PinNumber)
{
	uint8_t value;
	value= (uint8_t) (( pGPIOx->IDR >> PinNumber ) & 0X01);
	return value;
}

uint16_t GPIO_ReadFromInputPort(GPIO_Regdef_t *pGPIOx)
{
	uint16_t value;
	value= (uint16_t) ( pGPIOx->IDR );
	return value;
}
void GPIO_WriteToOutputPin(GPIO_Regdef_t *pGPIOx , uint8_t PinNumber, uint8_t value)
{
	if(value==GPIO_PIN_SET)
	{
		pGPIOx->ODR |= 1<<PinNumber;
	}
	else
	{
		pGPIOx->ODR &= ~(1<<PinNumber);
	}
}
void GPIO_WriteToOutputPort(GPIO_Regdef_t *pGPIOx,uint16_t value)
{
	pGPIOx->ODR=value;
}
void GPIO_ToggleOutputPin(GPIO_Regdef_t *pGPIOx,uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1<<PinNumber);
}

/*IRQ and ISR  configuration */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <=31)
		{
			*NVIC_ISER0 |=1<<IRQNumber;
		}
		else if(IRQNumber >32 && IRQNumber <64)
		{
			*NVIC_ISER1 |=(1<<(IRQNumber%32));
		}
		else if(IRQNumber >= 64 && IRQNumber <96)
		{
			*NVIC_ISER2 |=(1<<(IRQNumber %64));
		}
	}
	else{
		if(IRQNumber <=31)
		{
			*NVIC_ICER0 |=1<<IRQNumber;
		}
		else if(IRQNumber >32 && IRQNumber <64)
		{
			*NVIC_ICER1 |=(1<<(IRQNumber%32));
		}
		else if(IRQNumber >= 64 && IRQNumber <96)
		{
			*NVIC_ICER2 |=(1<<(IRQNumber %64));
		}
	}
}

void GPIO_PRIORITY_CONFIG(uint8_t IRQNumber,uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber /4;
	uint8_t iprx_section =IRQNumber %4;

	uint8_t shift_amount = (8 * iprx_section ) + (8- NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR+(iprx *4))|= IRQPriority <<(8* shift_amount);

}

void GPIO_IRQHandling(uint8_t PinNumber)
{
	if(EXTI->PR1 & (1<< PinNumber))
	{
		EXTI->PR1 |=(1<<PinNumber);
	}
}
