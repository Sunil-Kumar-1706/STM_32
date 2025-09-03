#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>
#include"STM32476.h"

//Initialize ADC peripheral (clock, GPIO, calibration, setup)
void adc_init(void);

// Trigger ADC conversion (continuous mode supported)
void start_conversion(void);

// Read latest ADC conversion result (blocking read)
uint16_t adc_read(void);

#endif /* INC_ADC_H_ */
