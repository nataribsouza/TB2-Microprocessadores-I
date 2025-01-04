/**
 * @file device.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-12-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef DEVICE_H_
#define DEVICE_H_

/* Includes */
#include <string.h>
#include "buzzer/buzzer.h"
#include "display/display.h"
#include "temperature/temperature.h"


/* Defines */
#define C_TIME_INJECTION_MS 500
#define C_TIME_IDLE_MS 1000

/* Enums */
typedef enum {
  ENUM_STATE_READ_BUTTONS,
  ENUM_STATE_READ_TEMPERATURE,
  ENUM_STATE_READ_POTENTIOMETER,
  ENUM_STATE_MANAGE_SYSTEMS,
  ENUM_STATE_MANAGE_LEDS,
  ENUM_STATE_MANAGE_BUZZER,
  ENUM_STATE_UPDATE_DISPLAY
} en_state;

/* Structs */
typedef struct {
  en_state state_en;
  st_buzzer buzzer_st;
  st_display display_st;
  st_temperature temperature_st;
  uint8_t instant_speed;
  uint8_t average_speed;
  uint8_t fuel_range;
} st_device;

/* Prototypes */
void init_device(st_device *device_st);
void state_machine(st_device *device_st);
void manage_system(st_device *device_st);
void manage_injection(st_device *device_st);

#endif /* DEVICE_H_ */
