// #pragma once

typedef struct{
    char optionName[15 + 1];
}DISPLAY_Option_t;

typedef struct{
    DISPLAY_Option_t *optionsArray;
    unsigned int optionsArrayLen;
    unsigned int currentPage;
    unsigned int selectedOption;
    DISPLAY_Option_t *currentOptionState;
} DISPLAY_MenuState_t;

void DISPLAY_renderMenu();
void DISPLAY_nextOption();
void DISPLAY_previousOption();
void DISPLAY_selectOption();
