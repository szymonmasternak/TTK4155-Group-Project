#include "hal_uart.h"

int main(void) {
    HAL_UART0_Init(9600);
    
    while (1) {
        char i = HAL_UART0_ReceiveChar();
        HAL_UART0_SendChar(i+1);
    }

    return 0;
}