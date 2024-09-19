
#include "hal_gpio.h"
#include "gpio.h"

void GPIO_Init(void){
    //Initialise All GPIOs
    HAL_GPIO_ConfigPin(HAL_GPIO_PORT_E, HAL_GPIO_PIN_1, HAL_GPIO_CFG_OUTPUT);
    HAL_GPIO_WritePin(HAL_GPIO_PORT_E, HAL_GPIO_PIN_1, HAL_GPIO_LOW);
}