/* Includes */
#include "keyboard.h"

/* Variables */
st_button keyboard[KEYBOARD_NUM_BUTTONS];

/* Functions */

/**
 * @brief init buttons structure
 * 
 * @param keyboard 
 */
void init_keyboard(void) {
    // Configure button break
    keyboard[ENUM_BUTTON_BREAK].executed = false;
    keyboard[ENUM_BUTTON_BREAK].pressed = false;
    keyboard[ENUM_BUTTON_BREAK].state = false;
    keyboard[ENUM_BUTTON_BREAK].old_state = false;
    keyboard[ENUM_BUTTON_BREAK].timer = 0;
    keyboard[ENUM_BUTTON_BREAK].pin = PIN_BUTTON_BREAK;

    // Configure button gas
    keyboard[ENUM_BUTTON_GAS].executed = false;
    keyboard[ENUM_BUTTON_GAS].pressed = false;
    keyboard[ENUM_BUTTON_GAS].state = false;
    keyboard[ENUM_BUTTON_GAS].old_state = false;
    keyboard[ENUM_BUTTON_GAS].timer = 0;
    keyboard[ENUM_BUTTON_GAS].pin = PIN_BUTTON_GAS;

    // Configure button previous
    keyboard[ENUM_BUTTON_PREVIOUS].executed = false;
    keyboard[ENUM_BUTTON_PREVIOUS].pressed = false;
    keyboard[ENUM_BUTTON_PREVIOUS].state = false;
    keyboard[ENUM_BUTTON_PREVIOUS].old_state = false;
    keyboard[ENUM_BUTTON_PREVIOUS].timer = 0;
    keyboard[ENUM_BUTTON_PREVIOUS].pin = PIN_BUTTON_PREVIOUS;

    // Configure button next
    keyboard[ENUM_BUTTON_NEXT].executed = false;
    keyboard[ENUM_BUTTON_NEXT].pressed = false;
    keyboard[ENUM_BUTTON_NEXT].state = false;
    keyboard[ENUM_BUTTON_NEXT].old_state = false;
    keyboard[ENUM_BUTTON_NEXT].timer = 0;
    keyboard[ENUM_BUTTON_NEXT].pin = PIN_BUTTON_NEXT;
}

/**
 * @brief Handle keyboard, reading 
 * all buttons with pooling
 * 
 */
void run_keyboard(void) {
    for(int i = 0; i < KEYBOARD_NUM_BUTTONS; i++) {
        st_button *button = &keyboard[i];
        uint32_t timer = HAL_GetTick();

        button->old_state = button->state;
        button->state = HAL_GPIO_ReadPin(GPIOB, button->pin);

        // If button unpressed
        if(button->state == true) {
            button->pressed = false;
            button->executed = false;
        }

        // Set timestamp when pressed
        if(button->state == false && button->old_state == true) {
            button->timer = timer + KEYBOARD_PRESSING_TIME_MS;
        }

        // If button was continues pressed for a determinated time
        if(button->state == false && timer > button->timer) {
            button->pressed = true;
        }
    }
}

/**
 * @brief Return true one single
 * time if the button was pressed
 * 
 * @param button 
 * @return true 
 * @return false 
 */
bool read_keyboard(uint8_t button) {
    bool pressed = keyboard[button].pressed;
    bool executed = keyboard[button].executed;

    // Buttons gas and break return the actual state every time 
    // that the function is called
    if(button == ENUM_BUTTON_BREAK || button == ENUM_BUTTON_GAS) {
        return pressed;

    // Buttons previous and next return pressed state only once
    // when called
    } else {
        if(executed || !pressed) {
            return false;
        } else {
            keyboard[button].executed = true;
            return true;
        }
    }
}