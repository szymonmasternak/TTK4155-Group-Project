#pragma once
#include <stdint.h>

typedef enum JOYSTICK_DIR{
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT,
    JOYSTICK_UP,
    JOYSTICK_DOWN,
    JOYSTICK_NEUTRAL,
} JOYSTICK_DIR_t;

typedef struct JOYSTICK_POS{
    int8_t x;
    int8_t y;
} JOYSTICK_POS_t;

JOYSTICK_DIR_t JOYSTICK_GetDirection(uint8_t joystickX, uint8_t joystickY);
JOYSTICK_POS_t JOYSTICK_GetAnalogPosition(uint8_t joystickX, uint8_t joystickY);
const char* JOYSTICK_DIR_toString(JOYSTICK_DIR_t dir);