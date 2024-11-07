#include <avr/io.h>
#include <stdio.h>
#include "config.h"
#include "hal_spi.h"
#include "hal_gpio.h"

void HAL_SPI_Init(){
    // MOSI, SCK, SS outputs
    HAL_GPIO_ConfigPin(HAL_GPIO_PORT_B, HAL_GPIO_PIN_5, HAL_GPIO_CFG_OUTPUT);
    HAL_GPIO_ConfigPin(HAL_GPIO_PORT_B, HAL_GPIO_PIN_7, HAL_GPIO_CFG_OUTPUT);
    HAL_GPIO_ConfigPin(HAL_GPIO_PORT_B, HAL_GPIO_PIN_4, HAL_GPIO_CFG_OUTPUT);
    // MISO input
    HAL_GPIO_ConfigPin(HAL_GPIO_PORT_B, HAL_GPIO_PIN_6, HAL_GPIO_CFG_INPUT);

    // Enable SPI, Set as Master SPI, Set Prescaler as 1/16 of Frequency of F_CPU
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

    HAL_SPI_SlaveSelectHigh();
}

void HAL_SPI_SlaveSelectLow(){
    HAL_GPIO_WritePin(HAL_GPIO_PORT_B, HAL_GPIO_PIN_4, HAL_GPIO_LOW);
}

void HAL_SPI_SlaveSelectHigh(){
    HAL_GPIO_WritePin(HAL_GPIO_PORT_B, HAL_GPIO_PIN_4, HAL_GPIO_HIGH);
}

uint8_t HAL_SPI_TransmitByte(uint8_t data){
    SPDR = data;
    return HAL_SPI_ReceiveByte();
}

uint8_t HAL_SPI_ReceiveByte(void){
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}