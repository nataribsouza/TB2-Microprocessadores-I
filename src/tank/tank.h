/**
 * @file tank.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-12-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef TANK_H_
#define TANK_H_

/* Includes */
#include <math.h>
#include <stdbool.h>

#include "stm32f1xx_hal.h"

/* Defines */
#define TIME_READ_TANK_MS 50
#define C_REFERENCE_VOLTAGE_V 3.3F
#define C_ADC_RESOLUTION_BITS 4096.0F
#define C_VOLT_TO_DEGREE (360 / C_REFERENCE_VOLTAGE_V)
#define MIM_VALID_ANGLE_DEGREE 98.66F
#define MAX_VALID_ANGLE_DEGREE 201.34F
#define ANGLE_MIDDLE_SCALE_DEGREES 150.0F
#define VOLUME_MIDDLE_SCALE_L 25.0F
#define C_MILIMITER_TO_VOLUME_L 20.0F
#define C_MIN_FUEL_VOLUME_L 4.0F

/* Structs */
typedef struct {
  bool critical;
  float volume;
} st_tank;

/* Prototypes */
void init_tank(st_tank *tank_st);
void run_tank(st_tank *tank_st);

#endif /* TANK_H_ */
