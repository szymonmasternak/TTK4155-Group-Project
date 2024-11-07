#include <stddef.h>
#include <stdio.h>
#include "display.h"
#include "ssd1308.h"

#define LEN(x) (sizeof(x) / sizeof((x)[0]))
#define DISPLAY_MAX_VISIBLE_OPTIONS 8

static void DISPLAY_optionHandler(DISPLAY_Option_t* option, unsigned int index);

DISPLAY_Option_t displayOptions[] = {
    {.optionName = "Option 1"},
    {.optionName = "Option 2"},
    {.optionName = "Option 3"},
    {.optionName = "Option 4"},
    {.optionName = "Option 5"},
    {.optionName = "Option 6"},
    {.optionName = "Option 7"},
    {.optionName = "Option 8"},
    {.optionName = "Option 9"},
};

DISPLAY_MenuState_t displayMenuState = {
    .optionsArray = displayOptions,
    .optionsArrayLen = LEN(displayOptions),
    .currentPage = 0,
    .selectedOption = 0,
    .currentOptionState = NULL, //We use pointer to the DISPLAY_Option_t as state
};

void DISPLAY_renderMenu(){
    SSD1308_Reset();

    if(displayMenuState.currentOptionState != NULL){
        DISPLAY_optionHandler(&displayMenuState.optionsArray[displayMenuState.selectedOption], displayMenuState.selectedOption);
        return;
    }
    
    unsigned int startIndex = displayMenuState.currentPage;
    unsigned int endIndex = startIndex + DISPLAY_MAX_VISIBLE_OPTIONS;
    if(endIndex > displayMenuState.optionsArrayLen){
        //if index goes out of bounds then clip
        endIndex = displayMenuState.optionsArrayLen;
    }

    for(unsigned int i = startIndex; i < endIndex; i++){
        SSD1308_SetCursor(i - startIndex, 0);  // Set cursor to current page (line)
        if(i == displayMenuState.selectedOption){
            SSD1308_printf(SSD1306_FONT_LARGE, ">%s", displayMenuState.optionsArray[i].optionName);
        }else{
            SSD1308_printf(SSD1306_FONT_LARGE, " %s", displayMenuState.optionsArray[i].optionName);
        }
    }
}

void DISPLAY_nextOption() {
    if(displayMenuState.currentOptionState != NULL){
        //if inside a particular option, dont increment
        return;
    }

    // check if out of index
    if(displayMenuState.selectedOption < displayMenuState.optionsArrayLen - 1){
        displayMenuState.selectedOption++;
        
        // check if we need to increase the page
        if(displayMenuState.selectedOption >= displayMenuState.currentPage + DISPLAY_MAX_VISIBLE_OPTIONS){
            displayMenuState.currentPage++;
        }
    }
}

void DISPLAY_previousOption() {
    if(displayMenuState.currentOptionState != NULL){
        //if inside a particular option, dont decrement
        return;
    }
    
    // check if not out of index
    if(displayMenuState.selectedOption > 0){
        displayMenuState.selectedOption--;
        //move the menu options if selection goes out of screen
        if(displayMenuState.selectedOption < displayMenuState.currentPage){
            displayMenuState.currentPage--;
        }
    }
}

void DISPLAY_selectOption() {
    if (displayMenuState.currentOptionState != NULL){
        // go back to the menu
        displayMenuState.currentOptionState = NULL;
        return;
    }
    if (displayMenuState.selectedOption < displayMenuState.optionsArrayLen) {
        displayMenuState.currentOptionState = &displayMenuState.optionsArray[displayMenuState.selectedOption];
    }
}

static void DISPLAY_optionHandler(DISPLAY_Option_t* option, unsigned int index){
    SSD1308_SetCursor(0,0);    
    char *text = NULL;
    switch (index){
        case 0:
            text = "Option 1!";
            break;
        case 1:
            text = "Option 2!";
            break;
        case 2:
            text = "Option 3!";
            break;
        case 3:
            text = "Option 4!";
            break;
        case 4:
            text = "Option 5!";
            break;
        case 5:
            text = "Option 6!";
            break;
        case 6:
            text = "Option 7!";
            break;
        case 7:
            text = "Option 8";
            break;
        case 8:
            text = "Option 9!";
            break;
        default:
            text = "Unknown Option";
            break;
    }
    SSD1308_printf(SSD1306_FONT_LARGE, text);
}