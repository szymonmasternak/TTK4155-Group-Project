
#include "hal_gpio.h"
#include "gpio.h"
#include "sn54als573.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void GPIO_Init(void){
    //Initialise All GPIOs
    HAL_GPIO_ConfigPin(HAL_GPIO_PORT_E, HAL_GPIO_PIN_1, HAL_GPIO_CFG_OUTPUT);
    SN54ALS573_LatchDisable();

    //Left & Right Button
    HAL_GPIO_ConfigPin(HAL_GPIO_PORT_D, HAL_GPIO_PIN_2, HAL_GPIO_CFG_INPUT);
    HAL_GPIO_ConfigPin(HAL_GPIO_PORT_D, HAL_GPIO_PIN_3, HAL_GPIO_CFG_INPUT);
    
    //Joystick Button
    HAL_GPIO_ConfigPin(HAL_GPIO_PORT_E, HAL_GPIO_PIN_0, HAL_GPIO_CFG_INPUT);
    
    // Configure INT0 and INT1 for rising edge
    MCUCR |= (1 << ISC00) | (1 << ISC01); // INT0 on rising edge
    MCUCR |= (1 << ISC10) | (1 << ISC11); // INT1 on rising edge

    MCUCSR &= ~(1 << ISC2); // INT2 on falling edge

    // Enable external interrupts
    GICR |= (1 << INT0); // Enable INT0
    GICR |= (1 << INT1); // Enable INT1
    GICR |= (1 << INT2); // Enable INT2
    sei();
}


ISR(INT0_vect) {
    printf("Right Button\n");
}

ISR(INT1_vect) {
    printf("Left Button\n");
}

ISR(INT2_vect) {
    printf("Joystick Button\n");
}
