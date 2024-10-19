#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "hal_uart.h"
#include "log.h"
#include "gpio.h"
#include "hal_gpio.h"

#include "sram.h"
#include <avr/io.h>

#include "max156.h"
#include <util/delay.h>

#include "joystick.h"
#include "ssd1308.h"

#include <string.h>
#include "display.h"

#include "hal_spi.h"
#include "mcp2515.h"

#include "can.h"
#include "clock.h"

int main(void) {
    SRAM_Init();

    HAL_UART0_Init(9600);
    LOG_Setup();
    GPIO_Init();

    printf("Testing This Function %d\n", 8);
    SRAM_Test();
    printf("Test Finished\n");
    
    CLOCK_Init();

    SSD1306_Init();
    SSD1308_Reset();
    SSD1308_SetCursor(0, 0);

    printf("Initialising SPI/CAN\n");
    HAL_SPI_Init();
    MCP2515_Init();
    MCP2515_SetMode(MODE_LOOPBACK);

    CAN_Data_t sampleFrame;
    sampleFrame.id = 321;  // Example CAN ID
    sampleFrame.length = 8;  // Maximum data size
    sampleFrame.data[0] = 11;
    sampleFrame.data[1] = 22;
    sampleFrame.data[2] = 33;
    sampleFrame.data[3] = 44;
    sampleFrame.data[4] = 55;
    sampleFrame.data[5] = 66;
    sampleFrame.data[6] = 77;
    sampleFrame.data[7] = 88;
    CAN_Transmit(&sampleFrame);
    
    //Wait for 100ms before retrieving frame
    _delay_ms(100);

    CAN_Data_t receiveFrame;
    memset(&receiveFrame, 0, sizeof(receiveFrame));
    CAN_Receive(&receiveFrame);

    printf("Can ID:%d Len:%d\n", receiveFrame.id, receiveFrame.length);
    for(uint8_t i=0; i<8; i++){
        printf("data[%d]:%d\n", i, receiveFrame.data[i]);
    }

    while(1){
        uint8_t adc_reading = MAX156_ReadData(MAX156_CHANNEL_0);
        uint8_t adc_reading1 = MAX156_ReadData(MAX156_CHANNEL_1);
        uint8_t adc_reading2 = MAX156_ReadData(MAX156_CHANNEL_2);
        uint8_t adc_reading3 = MAX156_ReadData(MAX156_CHANNEL_3);
        JOYSTICK_POS_t pos = JOYSTICK_GetAnalogPosition(adc_reading, adc_reading1);
        JOYSTICK_DIR_t dir = JOYSTICK_GetDirection(adc_reading, adc_reading1);

        switch(dir){
            case JOYSTICK_DOWN:
                DISPLAY_nextOption();
                break;
            case JOYSTICK_UP:
                DISPLAY_previousOption();
                break;
            case JOYSTICK_RIGHT:
                DISPLAY_selectOption();
                break;
            default:
                break;
        }

        if(HAL_GPIO_ReadPin(HAL_GPIO_PORT_B, HAL_GPIO_PIN_1) == HAL_GPIO_LOW){
            printf("Joystick press\n");
        }

        DISPLAY_renderMenu();
        printf("MAX156 CH0: %03d CH1: %03d CH2: %03d CH3: %03d JoystickPos: %02d, %02d JoystickDir: %s\n", adc_reading, adc_reading1, adc_reading2, adc_reading3, pos.x, pos.y, JOYSTICK_DIR_toString(dir));
        _delay_ms(500);
    }

    return 0;
}