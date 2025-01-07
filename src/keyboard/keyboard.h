/**
 * @file keyboard.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

/* Includes */
#include <stdbool.h>
#include "stm32f1xx_hal.h"

/* Defines */
#define KEYBOARD_NUM_BUTTONS 4
#define PIN_BUTTON_BREAK GPIO_PIN_15
#define PIN_BUTTON_GAS GPIO_PIN_14
#define PIN_BUTTON_PREVIOUS GPIO_PIN_13
#define PIN_BUTTON_NEXT GPIO_PIN_12
#define KEYBOARD_PRESSING_TIME_MS 150

/* Enums */
typedef enum {
    ENUM_BUTTON_BREAK,
    ENUM_BUTTON_GAS,
    ENUM_BUTTON_PREVIOUS,
    ENUM_BUTTON_NEXT
} en_button;

/* Structures */

/**
 * @brief Hold all needed variables
 * for button control
 * 
 */
typedef struct {
    bool state;
    bool old_state;
    bool pressed;
    bool executed;
    uint32_t timer;
    uint16_t pin;
} st_button;

/* Prototypes */
void init_keyboard(void);
void run_keyboard(void);
bool read_keyboard(uint8_t button);

#endif /* Keyboard */