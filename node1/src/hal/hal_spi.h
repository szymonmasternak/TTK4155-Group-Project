#pragma once
#include <stdint.h>

void HAL_SPI_Init();
uint8_t HAL_SPI_TransmitByte(uint8_t data);
uint8_t HAL_SPI_ReceiveByte(void);

void HAL_SPI_SlaveSelectLow();
void HAL_SPI_SlaveSelectHigh();
