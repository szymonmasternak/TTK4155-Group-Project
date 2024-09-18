#pragma once

typedef enum{
    HAL_GPIO_CFG_INPUT = 0,
    HAL_GPIO_CFG_OUTPUT = 1,
} HAL_GPIO_CFG_t;

typedef enum{
    HAL_GPIO_PORT_A = 0x3B,
    HAL_GPIO_PORT_B = 0x38,
    HAL_GPIO_PORT_C = 0x35,
    HAL_GPIO_PORT_D = 0x32,
    HAL_GPIO_PORT_E = 0x27,
} HAL_GPIO_PORT_t;

typedef enum{
    HAL_GPIO_PIN_0 = 0,
    HAL_GPIO_PIN_1 = 1,
    HAL_GPIO_PIN_2 = 2,
    HAL_GPIO_PIN_3 = 3,
    HAL_GPIO_PIN_4 = 4,
    HAL_GPIO_PIN_5 = 5,
    HAL_GPIO_PIN_6 = 6,
    HAL_GPIO_PIN_7 = 7,
} HAL_GPIO_PIN_t;

typedef enum{
    HAL_GPIO_HIGH,
    HAL_GPIO_LOW,
} HAL_GPIO_STATE_t;

void HAL_GPIO_ConfigPin(HAL_GPIO_PORT_t port, HAL_GPIO_PIN_t pin, HAL_GPIO_CFG_t cfg);
void HAL_GPIO_WritePin(HAL_GPIO_PORT_t port, HAL_GPIO_PIN_t pin, HAL_GPIO_STATE_t state);
