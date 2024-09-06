#include "log.h"
#include "hal_uart.h"

FILE customstdout = FDEV_SETUP_STREAM(LOG_PutChar, NULL, _FDEV_SETUP_WRITE);

void LOG_Setup(void){
    stdout = &customstdout;
}

int LOG_PutChar(char c, FILE *stream){
    HAL_UART0_SendChar(c);
    return 0;
} 