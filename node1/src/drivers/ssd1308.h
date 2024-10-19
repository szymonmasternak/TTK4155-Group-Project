#pragma once

#include <stdint.h>

typedef enum {
    SSD1306_FONT_SMALL,
    SSD1306_FONT_MEDIUM,
    SSD1306_FONT_LARGE,    
} SSD1306_FONT_t;

void SSD1306_Init();
void SSD1308_Reset(void);
void SSD1308_SetCursor(unsigned int page, unsigned int column);
void SSD1308_WriteChar(char c, SSD1306_FONT_t font);
int SSD1308_printf(SSD1306_FONT_t font, const char *format, ...);
