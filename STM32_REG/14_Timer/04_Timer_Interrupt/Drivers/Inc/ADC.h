#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>
#include "STM32476.h"

void adc_init(void);   //adc intialisation
uint32_t adc_read(void); //read the sensor value
void start_conversion(void); //start the conversion

#endif /* INC_ADC_H_ */
