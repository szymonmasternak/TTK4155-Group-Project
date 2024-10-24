#include <avr/io.h> 
#include "clock.h"
#include "hal_gpio.h"

void CLOCK_Init(void) {
    //Page 95 of Datasheet
    //Max pwm freq => f_oc = f_clk_io / 2
    //f_oc = f_clk_io / (2 * N * (1 + OCR1))

    //Set PD5 as Output
    HAL_GPIO_ConfigPin(HAL_GPIO_PORT_D, HAL_GPIO_PIN_5, HAL_GPIO_CFG_OUTPUT);

    //Fast PWM no prescaler (N=1 from formulae above)
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);

    //Clock counts from 0-1
    ICR1H = 0;
    ICR1L = 1;

    //50% duty cycle
    OCR1AH = 0;
    OCR1AL = 0;
}