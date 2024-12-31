/* Include */
#include "device.h"

/* Functions */

/**
 * @brief Manage device functioning
 * 
 * @param device_st 
 */
void state_machine(st_device *device_st) {
    switch (device_st->state_en) {
        case ENUM_STATE_READ_BUTTONS:
            run_buttons();
            device_st->state_en = ENUM_STATE_READ_POTENTIOMETER;
            break;

        case ENUM_STATE_READ_POTENTIOMETER:
            run_tank();
            device_st->state_en = ENUM_STATE_MANAGE_LEDS;
            break;

        case ENUM_STATE_MANAGE_LEDS:
            run_leds();
            device_st->state_en = ENUM_STATE_MANAGE_BUZZER;
            break;

        case ENUM_STATE_MANAGE_BUZZER:
            run_buzzer();
            device_st->state_en = ENUM_STATE_UPDATE_DISPLAY;
            break;

        case ENUM_STATE_UPDATE_DISPLAY:
            display_update();
            device_st->state_en = ENUM_STATE_UPDATE_SPEED;
            break;

        case ENUM_STATE_UPDATE_SPEED:
            device_speed();
            device_st->state_en = ENUM_STATE_READ_BUTTONS;

        default:
            /* Weak state reached. Rebooting*/
            NVIC_SystemReset();
    }
}

void init_device(st_device *device_st) {
    device_st->state_en = ENUM_STATE_READ_BUTTONS;
}