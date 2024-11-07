#include "ssd1308.h"

#include "config.h"
#include "util/delay.h"

#include <avr/pgmspace.h>
#include "fonts.h"

#include "log.h"
#include <stdio.h>

#define SSD1306_COMMAND_LOCATION        0x1000
#define SSD1306_DATA_LOCATION           0x1200

#define SSD1306_NO_OF_PAGES             8
#define SSD1306_NO_OF_COLUMNS           128

static SSD1306_FONT_t printfFont = SSD1306_FONT_SMALL; //Holds the font type for printf, do not modify
static volatile uint8_t *SSD1308_Command = (uint8_t *) SSD1306_COMMAND_LOCATION;
static volatile uint8_t *SSD1308_Data = (uint8_t *) SSD1306_DATA_LOCATION;

static void SSD1308_WriteCommand(uint8_t cmd);
static void SSD1308_WriteData(uint8_t data);
static int SSD1308_PutChar(char c, FILE *stream);

FILE SSD1308out = FDEV_SETUP_STREAM(SSD1308_PutChar, NULL, _FDEV_SETUP_WRITE);

static void SSD1308_WriteCommand(uint8_t cmd){
    SSD1308_Command[0] = cmd;
}

static void SSD1308_WriteData(uint8_t data){
    SSD1308_Data[0] = data;
}

static int SSD1308_PutChar(char c, FILE *stream){
    SSD1308_WriteChar(c, printfFont);
    return 0;
}

void SSD1306_Init(){
    SSD1308_WriteCommand(0xae); // display off
    SSD1308_WriteCommand(0xa1); //segment remap
    SSD1308_WriteCommand(0xda); //common pads hardware: alternative
    SSD1308_WriteCommand(0x12);
    SSD1308_WriteCommand(0xc8); //common output scan direction:com63~com0
    SSD1308_WriteCommand(0xa8); //multiplex ration mode:63
    SSD1308_WriteCommand(0x3f);
    SSD1308_WriteCommand(0xd5); //display divide ratio/osc. freq. mode
    SSD1308_WriteCommand(0x80);
    SSD1308_WriteCommand(0x81); //contrast control
    SSD1308_WriteCommand(0x50);
    SSD1308_WriteCommand(0xd9); //set pre-charge period
    SSD1308_WriteCommand(0x21);
    SSD1308_WriteCommand(0x20); //Set Memory Addressing Mode
    SSD1308_WriteCommand(0x02);
    SSD1308_WriteCommand(0xdb); //VCOM deselect level mode
    SSD1308_WriteCommand(0x30);
    SSD1308_WriteCommand(0xad); //master configuration
    SSD1308_WriteCommand(0x00);
    SSD1308_WriteCommand(0xa4); //out follows RAM content
    SSD1308_WriteCommand(0xa6); //set normal display
    SSD1308_WriteCommand(0xaf); // display on
}

void SSD1308_Reset(void){
    for (unsigned int page = 0; page < SSD1306_NO_OF_PAGES; page++){
        SSD1308_SetCursor(page, 0);
        for (unsigned int col = 0; col < SSD1306_NO_OF_COLUMNS; col++){
            SSD1308_WriteData(0x00);
        }
    }
}

void SSD1308_SetCursor(unsigned int page, unsigned int column){
    if(page >= SSD1306_NO_OF_PAGES){
        return;
    }
    if(column >= SSD1306_NO_OF_COLUMNS){
        return;
    }
    
    //Set Page
    SSD1308_WriteCommand(0xB0 | (page & 0x07));
    
    //Set Column
    SSD1308_WriteCommand(0x00 | (column & 0x0F));
    SSD1308_WriteCommand(0x10 | (column >> 4));
}

void SSD1308_WriteChar(char c, SSD1306_FONT_t font){
    const unsigned char* fontPtr = NULL;
    uint8_t numCols = 0;

    switch(font){
        case SSD1306_FONT_SMALL:
            fontPtr = font4[c-32];
            numCols = sizeof(font4[0])/sizeof(font4[0][0]);
            break;
        case SSD1306_FONT_MEDIUM:
            fontPtr = font5[c-32];
            numCols = sizeof(font5[0])/sizeof(font5[0][0]);
            break;
        case SSD1306_FONT_LARGE:
            fontPtr = font8[c-32];
            numCols = sizeof(font8[0])/sizeof(font8[0][0]);
            break;
        default:
            break;
    }
    
    for (uint8_t i = 0; i < numCols; i++) {
        uint8_t data = pgm_read_byte(&fontPtr[i]);
        SSD1308_WriteData(data);
    }
}

int SSD1308_printf(SSD1306_FONT_t font, const char *format, ...) {
    va_list args;
    va_start(args, format);

    printfFont = font;

    int ret = vfprintf(&SSD1308out, format, args);

    va_end(args);
    return ret;
}