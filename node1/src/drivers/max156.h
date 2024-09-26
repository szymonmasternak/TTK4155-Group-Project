#pragma once

#include <stdint.h>
typedef enum MAX156_SNGL_CHNNL{    
    MAX156_CHANNEL_0 = 0b00,
    MAX156_CHANNEL_1 = 0b01,
    MAX156_CHANNEL_2 = 0b10,
    MAX156_CHANNEL_3 = 0b11,
}MAX156_SNGL_CHNNL_t;

uint8_t MAX156_ReadData(MAX156_SNGL_CHNNL_t channel);
uint8_t MAX156_ReadData2(MAX156_SNGL_CHNNL_t channel);
