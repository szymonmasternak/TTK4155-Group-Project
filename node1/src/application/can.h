#pragma once
#include <stdint.h>

typedef struct{
    uint16_t id;
    uint8_t length;
    uint8_t data[8];
}CAN_Data_t;

void CAN_Transmit(CAN_Data_t *data);
void CAN_Receive(CAN_Data_t *data);
void CAN_print(CAN_Data_t *data);