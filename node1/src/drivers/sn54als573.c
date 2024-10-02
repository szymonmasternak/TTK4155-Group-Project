#include <stdint.h>
#include "sn54als573.h"
#include "hal_gpio.h"

uint8_t latchState = 0;

void SN54ALS573_LatchEnable(void){
    HAL_GPIO_WritePin(HAL_GPIO_PORT_E, HAL_GPIO_PIN_1, HAL_GPIO_HIGH);
    latchState = 1;
}

void SN54ALS573_LatchDisable(void){
    HAL_GPIO_WritePin(HAL_GPIO_PORT_E, HAL_GPIO_PIN_1, HAL_GPIO_LOW);
    latchState = 0;
}