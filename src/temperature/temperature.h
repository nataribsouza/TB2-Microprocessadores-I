/**
 * @file temperature.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-12-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

/* Includes */
#include <string.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"

/* Defines */
#define C_TIME_READ_TEMPERATURE_MS 50
#define C_REFERENCE_VOLTAGE_V 3.3F
#define C_ADC_RESOLUTION_BITS 4096.0F
#define C_VOLTAGE_25_DEGREE_C 1.43F
#define C_VOLTAGE_PER_DEGREE (1000 / 43)
#define C_BASE_TEMPERATURE_DEGREE 25.0F
#define C_CRITICAL_TEMPERATURE_C 50.0F
#define C_COUNTS_ALERT_OVERHEAT 5
#define C_TIME_CRITICAL_TEMPERATURE 180000

/* Structures */
typedef struct {
  bool alert;
  bool critical;
  float old_temperature;
  float temperature;
} st_temperature;

/* Prototypes */
void init_temperature(st_temperature *temperature_st);
void read_temperature(st_temperature *temperature_st);

#endif /* TEMPERATURE_H_ */
