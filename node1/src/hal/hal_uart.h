#pragma once

#include <stdint.h>

void HAL_UART0_Init(uint32_t baudrate);
void HAL_UART0_SendChar(char data);
char HAL_UART0_ReceiveChar(void);