#include <stdio.h>
#include <stdint.h>
#include "can.h"
#include "mcp2515.h"

#define MCP_RXB0SIDL 0b01100010

#define MCP_TXB0SIDH 0b00110001
#define MCP_TXB0SIDL 0b00110010
#define MCP_TXB0DLC  0b00110101

#define MCP_TXB0D0   0b00110110
#define MCP_RXB0DLC  0b01100101
#define MCP_RXB0D0   0b01100110

void CAN_Transmit(CAN_Data_t *data){
    if(data == NULL) {
        return;
    }

    MCP2515_WriteRegister(MCP_TXB0SIDL, (data->id << 5) & 0b11100000);
    MCP2515_WriteRegister(MCP_TXB0SIDH, (data->id >> 3) & 0b11111111);
    MCP2515_WriteRegister(MCP_TXB0DLC, data->length & 0b00001111);

    for(uint8_t i = 0; i < data->length; i++) {
        MCP2515_WriteRegister(MCP_TXB0D0 + i, data->data[i]);
    }

    MCP2515_RequestToSendBuffer0();
}

void CAN_Receive(CAN_Data_t *data){
    if (data == NULL) {
        return;
    }

    uint8_t id_low = MCP2515_ReadRegister(MCP_RXB0SIDL);
    uint8_t id_high = MCP2515_ReadRegister(MCP_RXB0SIDH);
    data->id = (id_high << 3) | (id_low >> 5);

    data->length = MCP2515_ReadRegister(MCP_RXB0DLC) & 0b00001111;

    for(uint8_t i = 0; i < data->length; i++) {
        data->data[i] = MCP2515_ReadRegister(MCP_RXB0D0 + i);
    }
}

void CAN_print(CAN_Data_t *data){
    if (data == NULL){
        return;
    }
    printf("CANID:0x%03X|Data Len:%d|Data:", data->id, data->length);
    for(uint8_t i = 0; i<data->length; i++){
        printf(" 0x%02X", data->data[i]);
    }
    printf("\n");
}