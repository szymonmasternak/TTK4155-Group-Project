#pragma once
#include <stdint.h>
#include "can.h"
typedef struct{
    uint8_t interruptCAN;
    uint8_t interruptButtonL;
    uint8_t interruptButtonR;
    CAN_Data_t canData;
}STATE_MCU_t;

extern STATE_MCU_t g_mcuState;