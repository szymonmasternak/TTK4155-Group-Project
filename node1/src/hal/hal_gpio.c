#include "hal_gpio.h"
#include <config.h>
#include <avr/io.h>

void HAL_GPIO_ConfigPin(HAL_GPIO_PORT_t port, HAL_GPIO_PIN_t pin, HAL_GPIO_CFG_t cfg){
    // DDR of any PORT is offset by -1
    if (cfg == HAL_GPIO_CFG_OUTPUT)
        *(volatile uint8_t*)(port - 1) |= (1 << pin);
    else
        *(volatile uint8_t*)(port - 1) &= ~(1 << pin);
}

void HAL_GPIO_WritePin(HAL_GPIO_PORT_t port, HAL_GPIO_PIN_t pin, HAL_GPIO_STATE_t state){
    if (state == HAL_GPIO_HIGH)
        *(volatile uint8_t*)port |= (1 << pin);
    else
        *(volatile uint8_t*)port &= ~(1 << pin);
}

HAL_GPIO_STATE_t HAL_GPIO_ReadPin(HAL_GPIO_PORT_t port, HAL_GPIO_PIN_t pin){
    // PIN register of any PORT is offset by -2
    if (*(volatile uint8_t*)(port - 2) & (1 << pin))
        return HAL_GPIO_HIGH;
    else
        return HAL_GPIO_LOW;
}