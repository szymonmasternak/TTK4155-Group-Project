#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "hal_uart.h"
#include "log.h"
#include "gpio.h"
#include "hal_gpio.h"

int main(void) {
    HAL_UART0_Init(9600);
    LOG_Setup();
    GPIO_Init();
    SRAM_Init();

    printf("Testing This Function %d\r\n", 8);
    SRAM_Test();
    printf("Test Finished\r\n");

    while(1){

    }

    return 0;
}