#ifndef INC_TIMER_DRIVER_H_
#define INC_TIMER_DRIVER_H_

#include"STM32476.h"

void timer2_1hz_init(void);           // Init Timer2 for 1Hz tick
void timer2_pa5_output_compare(void); // Timer2 Output Compare on PA5
void timer3_pa6_input_capture(void);  // Timer3 Input Capture on PA6

#define SR_UIF    (1U<<0)   // Update interrupt flag
#define SR_CC1IF  (1U<<1)   // Capture/Compare 1 interrupt flag

#endif /* INC_TIMER_DRIVER_H_ */
