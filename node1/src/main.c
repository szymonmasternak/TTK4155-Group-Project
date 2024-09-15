#include <stdio.h>
#include "hal_uart.h"
#include "log.h"

int main(void) {
    HAL_UART0_Init(9600);
    LOG_Setup();

    printf("Testing This Function %d\r\n", 8);

    while (1) {
        char i = HAL_UART0_ReceiveChar();
        HAL_UART0_SendChar(i+1);
    }

    return 0;
}