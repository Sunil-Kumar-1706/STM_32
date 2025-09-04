#include"STM32476.h"
#include"GPIO_driver.h"
#include"Timer_driver.h"

int main(void)
{
    timer2_pa5_output_compare();   // Configure TIM2 CH1 on PA5 for toggle output

    while(1)
    {
        // Main loop does nothing, timer hardware toggles PA5 automatically
    }
}
