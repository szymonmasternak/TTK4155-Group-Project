#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include <util/delay.h>
#include "max156.h"

#define MAX156_ADDRESS 0x1400

// Page 17 of datasheet.
// With Conversion formulae of 9*1*2/(4915200/2)
// Approx 8uS rounded up
#define MAX156_CONVERSION_TIME_US (8)

uint8_t MAX156_ReadData(MAX156_SNGL_CHNNL_t channel){
    volatile uint8_t *adc = (uint8_t *) MAX156_ADDRESS;
    // 
    *adc =  (0b10000000 | (uint8_t)channel);

    _delay_us(MAX156_CONVERSION_TIME_US);

    return *adc;
}