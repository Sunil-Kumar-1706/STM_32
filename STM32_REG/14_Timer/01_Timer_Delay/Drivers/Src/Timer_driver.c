#include"Timer_driver.h"

#define TIM2EN      (1U<<0)   // APB1ENR1: TIM2 clock enable
#define CR1_CEN     (1U<<0)

void timer2_1hz_init(void)
{
    /* Enable clock access to TIM2 */
    RCC->APB1ENR1 |= TIM2EN;

    /* Set prescaler */
    TIM2->PSC = 16000 - 1;   // 16 MHz / 16000 = 1 kHz

    /* Set auto-reload value */
    TIM2->ARR = 1000 - 1;    // 1000 / 1000 Hz = 1 Hz

    /* Clear counter */
    TIM2->CNT = 0;

    /* Enable timer */
    TIM2->CR1 = CR1_CEN;
}

