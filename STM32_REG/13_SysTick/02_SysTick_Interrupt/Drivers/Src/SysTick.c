#include"SysTick.h"

#define CTRL_ENABLE      (1U<<0)
#define CTRL_CLKSRC      (1U<<2)
#define CTRL_COUNTFLAG   (1U<<16)
#define CTRL_TICKINT     (1U<<1)

void systickDelayMs(int delay)
{
    // Calculate reload value based on current system clock

    SysTick->RVR = (4000000/1000) - 1;

    // Clear current value register
    SysTick->CVR = 0;

    // Enable SysTick with processor clock
    SysTick->CSR = CTRL_ENABLE | CTRL_CLKSRC;

    for(int i = 0; i < delay; i++)
    {
        // Wait until COUNTFLAG is set
        while((SysTick->CSR & CTRL_COUNTFLAG) == 0);
    }

    // Disable SysTick
    SysTick->CSR = 0;
}

void systick_1hz_Interrupt(void)
{
	   // Calculate reload value based on current system clock

	    SysTick->RVR = 4000000;

	    // Clear current value register
	    SysTick->CVR = 0;

	    // Enable SysTick with processor clock
	      SysTick->CSR = CTRL_ENABLE | CTRL_CLKSRC;

	      // Enable SysTick interrupt
	    SysTick->CSR |=CTRL_TICKINT;

}
