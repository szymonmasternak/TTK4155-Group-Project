#include "joystick.h"

JOYSTICK_DIR_t JOYSTICK_GetDirection(uint8_t joystickX, uint8_t joystickY){
    #define JOYSTICK_MAX_VAL 255
    #define JOYSTICK_MID_VAL 160
    #define JOYSTICK_THRESHOLD 20

    const uint8_t neutral_max = JOYSTICK_MID_VAL + JOYSTICK_THRESHOLD;
    const uint8_t neutral_min = JOYSTICK_MID_VAL - JOYSTICK_THRESHOLD;
    
    if((joystickY > neutral_max) && (neutral_min <= joystickX <= neutral_max))
        return JOYSTICK_UP;
    if((joystickY < neutral_min) && (neutral_min <= joystickX <= neutral_max))
        return JOYSTICK_DOWN;
    if((joystickX < neutral_min) && (neutral_min <= joystickY <= neutral_max))
        return JOYSTICK_LEFT;
    if((joystickX > neutral_max) && (neutral_min <= joystickY <= neutral_max))
        return JOYSTICK_RIGHT;
    return JOYSTICK_NEUTRAL;
    
    #undef JOYSTICK_MAX_VAL
    #undef JOYSTICK_MID_VAL
    #undef JOYSTICK_THRESHOLD
}

const char* JOYSTICK_DIR_toString(JOYSTICK_DIR_t dir) {
    #define JOYSTICK_STRINGIFY(x) #x
    #define JOYSTICK_CASE(s) case s: return JOYSTICK_STRINGIFY(s)

    switch(dir){
        JOYSTICK_CASE(JOYSTICK_LEFT);
        JOYSTICK_CASE(JOYSTICK_RIGHT);
        JOYSTICK_CASE(JOYSTICK_UP);
        JOYSTICK_CASE(JOYSTICK_DOWN);
        JOYSTICK_CASE(JOYSTICK_NEUTRAL);
        default:
            return "JOYSTICK_UNKNOWN";
    }
    
    #undef JOYSTICK_CASE
}

JOYSTICK_POS_t JOYSTICK_GetAnalogPosition(uint8_t joystickX, uint8_t joystickY){
    #define JOYSTICK_MIDPOINT_X 163
    #define JOYSTICK_MIDPOINT_Y 161
    #define JOYSTICK_MAX_VALUE 255

    JOYSTICK_POS_t res;

    res.x = (int8_t)(((int32_t)(joystickX - JOYSTICK_MIDPOINT_X) * 100) / (JOYSTICK_MAX_VALUE - JOYSTICK_MIDPOINT_X));
    res.y = (int8_t)(((int32_t)(joystickY - JOYSTICK_MIDPOINT_Y) * 100) / (JOYSTICK_MAX_VALUE - JOYSTICK_MIDPOINT_Y));

    return res;
}

