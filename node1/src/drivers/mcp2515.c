#include <stdio.h>
#include "mcp2515.h"
#include "hal_spi.h"
#include "gpio.h"

void MCP2515_Init(){
    MCP2515_SoftwareReset();

    uint8_t value = MCP2515_ReadRegister(MCP_CANSTAT);
    if((value & MODE_MASK ) != MODE_CONFIG){
        printf("MCP2515 not in config!\n");
        return;
    }

    //Enable CAN Interrupt on Message RX
    MCP2515_WriteRegister(MCP_CANINTE, 0x01);
    GPIO_EnableCANInterrupt();
}

uint8_t MCP2515_ReadRegister(uint8_t reg){
    HAL_SPI_SlaveSelectLow();
    
    HAL_SPI_TransmitByte(MCP_READ);
    HAL_SPI_TransmitByte(reg);
    uint8_t ret = HAL_SPI_TransmitByte(0x00); //Dummy Data
    
    HAL_SPI_SlaveSelectHigh();
    
    return ret;
}

void MCP2515_WriteRegister(uint8_t address, uint8_t data){
    HAL_SPI_SlaveSelectLow();
    
    HAL_SPI_TransmitByte(MCP_WRITE);
    HAL_SPI_TransmitByte(address);
    HAL_SPI_TransmitByte(data);
    
    HAL_SPI_SlaveSelectHigh();
}

void MCP2515_SoftwareReset(){
    HAL_SPI_SlaveSelectLow();

    HAL_SPI_TransmitByte(MCP_RESET);

    HAL_SPI_SlaveSelectHigh();
}

void MCP2515_BitModify(uint8_t reg, uint8_t mask, uint8_t data){
    HAL_SPI_SlaveSelectLow();

    HAL_SPI_TransmitByte(MCP_BITMOD);
    HAL_SPI_TransmitByte(reg);
    HAL_SPI_TransmitByte(mask);
    HAL_SPI_TransmitByte(data);

    HAL_SPI_SlaveSelectHigh();
}

void MCP2515_SetMode(uint8_t mode){
    MCP2515_BitModify(MCP_CANCTRL, MODE_MASK, mode);

    uint8_t data = MCP2515_ReadRegister(MCP_CANSTAT);
    if((data & MODE_MASK) != mode){
        printf("MCP2515 Failed to set mask\n");
    }
}

void MCP2515_RequestToSendBuffer0(){
    HAL_SPI_SlaveSelectLow();

    HAL_SPI_TransmitByte(MCP_RTS_TX0);

    HAL_SPI_SlaveSelectHigh();
}