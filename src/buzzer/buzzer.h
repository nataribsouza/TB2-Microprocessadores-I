/**
 * @file buzzer.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-12-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/* Include */
#include <stdbool.h>
#include "stm32f1xx_hal.h"

/* Defines */
#define PORT_BUZZER GPIOB
#define PIN_BUZZER GPIO_PIN_1
#define TIME_BEEP_UP_MS 2000
#define TIME_BEEP_DOWN_MS 30000

/* Structs */
typedef struct {
  bool alarm;
  uint32_t interval_beeps;
} st_buzzer;

/* Prototypes */
void run_buzzer(st_buzzer *buzzer_st);
void buzzer_turnOn(st_buzzer *buzzer_st);
void buzzer_turnOff(st_buzzer *buzzer_st);

#endif /* BUZZER_H_ */
