#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#include"STM32476.h"

void systickDelayMs(int delay);
void systick_1hz_Interrupt(void);

#endif /* INC_SYSTICK_H_ */
