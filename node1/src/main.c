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

#include "state.h"

void processInterrupts(){
    if(g_mcuState.interruptButtonL){
        g_mcuState.interruptButtonL = 0;
        printf("L Button\n");
    }
    if(g_mcuState.interruptButtonR){
        g_mcuState.interruptButtonR = 0;
        printf("R Button\n");
    }
    if(g_mcuState.interruptCAN){
        g_mcuState.interruptCAN = 0;
        printf("CAN Int\n");
        uint8_t interruptFlags = MCP2515_ReadRegister(MCP_CANINTF);
        if (interruptFlags & 0b00000001) {
            MCP2515_WriteRegister(MCP_CANINTF, 0x00); //Clear The Interrupt
        }
        CAN_Receive(&g_mcuState.canData);
        printf("CAN Data Received\n");
        CAN_print(&g_mcuState.canData);
    }
}


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

    CAN_Data_t sampleFrame;
    sampleFrame.id = 0x24;  // Example CAN ID
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

        processInterrupts();

        if(HAL_GPIO_ReadPin(HAL_GPIO_PORT_B, HAL_GPIO_PIN_1) == HAL_GPIO_LOW){
            printf("Joystick press\n");
        }

        memset(&sampleFrame, 0, sizeof(sampleFrame));
        sampleFrame.length = 2;
        sampleFrame.id = 0x24;
        sampleFrame.data[0] = pos.x;
        sampleFrame.data[1] = pos.y;
        CAN_Transmit(&sampleFrame);

        DISPLAY_renderMenu();
        // printf("MAX156 CH0: %03d CH1: %03d CH2: %03d CH3: %03d JoystickPos: %02d, %02d JoystickDir: %s\n", adc_reading, adc_reading1, adc_reading2, adc_reading3, pos.x, pos.y, JOYSTICK_DIR_toString(dir));
        _delay_ms(500);
    }

    return 0;
}