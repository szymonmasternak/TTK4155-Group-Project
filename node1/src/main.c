#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "hal_uart.h"
#include "log.h"
#include "gpio.h"
#include "hal_gpio.h"

#include "sram.h"
#include <avr/io.h>

#include "max156.h"
#include <util/delay.h>

#include "joystick.h"

void init_pwm(void) {
    //Page 95 of Datasheet
    //Max pwm freq => f_oc = f_clk_io / 2
    //f_oc = f_clk_io / (2 * N * (1 + OCR1))

    //Set PD5 as Output
    DDRD |= (1 << PD5);

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

int main(void) {
    SRAM_Init();

    HAL_UART0_Init(9600);
    LOG_Setup();
    GPIO_Init();

    printf("Testing This Function %d\r\n", 8);
    SRAM_Test();
    printf("Test Finished\r\n");
    
    init_pwm();

    while(1){
        uint8_t adc_reading = MAX156_ReadData(MAX156_CHANNEL_0);
        uint8_t adc_reading1 = MAX156_ReadData(MAX156_CHANNEL_1);
        uint8_t adc_reading2 = MAX156_ReadData(MAX156_CHANNEL_2);
        uint8_t adc_reading3 = MAX156_ReadData(MAX156_CHANNEL_3);
        JOYSTICK_POS_t pos = JOYSTICK_GetAnalogPosition(adc_reading, adc_reading1);
        JOYSTICK_DIR_t dir = JOYSTICK_GetDirection(adc_reading, adc_reading1);

        printf("MAX156 CH0: %03d CH1: %03d CH2: %03d CH3: %03d JoystickPos: %02d, %02d JoystickDir: %s\n", adc_reading, adc_reading1, adc_reading2, adc_reading3, pos.x, pos.y, JOYSTICK_DIR_toString(dir));
        _delay_ms(100);
    }

    return 0;
}