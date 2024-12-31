/* Includes */
#include "temperature.h"

/* Variables */
extern ADC_HandleTypeDef hadc1;

/* Functions */

void init_temperature(st_temperature *temperature_st) {
  temperature_st->alert = false;
  temperature_st->critical = false;
  temperature_st->old_temperature = C_BASE_TEMPERATURE_DEGREE;
  temperature_st->temperature = C_BASE_TEMPERATURE_DEGREE;
}

void read_temperature(st_temperature *temperature_st) {
  uint32_t adc = 0;
  float v_readed = 0;
  uint32_t timer = HAL_GetTick();
  static uint8_t overheat_counter = 0;
  static uint32_t elapsed_timer = C_TIME_READ_TEMPERATURE_MS;
  static uint32_t critical_timer = 0;

  if (timer > elapsed_timer) {
    elapsed_timer = timer + C_TIME_READ_TEMPERATURE_MS;

    // Wait until convertion
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    // Read ADC value
    adc = HAL_ADC_GetValue(&hadc1);

    // Convert to voltage
    v_readed = (adc * C_REFERENCE_VOLTAGE_V) / C_ADC_RESOLUTION_BITS;

    // Convert to temperature
    temperature_st->temperature =
        ((v_readed - C_VOLTAGE_25_DEGREE_C) / C_VOLTAGE_PER_DEGREE) +
        C_BASE_TEMPERATURE_DEGREE;

    // Check if temperature is above max
    if (temperature_st->temperature >= C_CRITICAL_TEMPERATURE_C) {
      overheat_counter = overheat_counter >= C_COUNTS_ALERT_OVERHEAT
                             ? overheat_counter
                             : overheat_counter + 1;

      // Check if above max temp for more than 5 readings
      if (overheat_counter >= C_COUNTS_ALERT_OVERHEAT &&
          temperature_st->alert == false) {
        critical_timer = timer + C_TIME_CRITICAL_TEMPERATURE;
        temperature_st->alert = true;
      }

      // Check if above max temp for more than 30 s
      if (timer > critical_timer) {
        temperature_st->critical = true;
      }

      // Reset overheat alert
    } else {
      overheat_counter = 0;
      temperature_st->alert = false;
      temperature_st->critical = false;
    }
  }
}
