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
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "tank/tank.h"
#include "buzzer/buzzer.h"
#include "display/display.h"
#include "keyboard/keyboard.h"
#include "temperature/temperature.h"
#include "stm32f1xx_hal.h"

/* Defines */
#define TIM_PWM_PRESCALER 71
#define TIM_PWM_AUTORELOAD 20000-1
#define TIM_PWM_MAX_DUTY_CYCLE 100
#define C_BREAK_PWM_SLOP 50
#define C_GAS_PWM_SLOP 10
#define C_IDLE_PWM_SLOP 10
#define C_TIME_LED_MS 500
#define C_TIME_INJECTION_MS 500
#define C_TIME_IDLE_MS 1000
#define C_KMH_TO_METER 0.36F
#define C_METER_TO_KM (1 / 1000)
#define C_TIME_GET_INSTANT_SPEED_MS 500
#define C_TIME_TO_CALCULATE_AVERAGE_SPEED_MS 1000

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
  bool alarm;
  en_state state_en;
  st_tank tank_st;
  st_buzzer buzzer_st;
  st_display display_st;
  st_temperature temperature_st;
  uint8_t instant_speed;
  uint8_t average_speed;
  uint8_t autonomi;
  uint32_t odometer;
  float fuel_range;
} st_device;

/* Prototypes */
void init_PWM(void);
void set_PWM(uint8_t duty_cycle);
void handle_leds(void);
void handle_display(st_device *device_st);
void display_update(st_device *device_st);
void init_device(st_device *device_st);
void state_machine(st_device *device_st);
void manage_system(st_device *device_st);
void manage_injection(st_device *device_st);
void calculate_average_speed(st_device *device_st);

#endif /* DEVICE_H_ */
