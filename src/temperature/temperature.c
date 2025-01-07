/* Includes */
#include "temperature.h"

/* Variables */
extern ADC_HandleTypeDef hadc1;

/* Functions */

/**
 * @brief Init temperature structure
 * 
 * @param temperature_st 
 */
void init_temperature(st_temperature *temperature_st) {
  temperature_st->alert = false;
  temperature_st->critical = false;
  temperature_st->temperature = C_BASE_TEMPERATURE_DEGREE;
  sprintf(temperature_st->msg, " ");
}

/**
 * @brief Read temperature with LM35
 * 
 * @param temperature_st 
 */
void read_temperature(st_temperature *temperature_st) {
  uint32_t raw = 0;
  float volts = 0;
  uint32_t timer = HAL_GetTick();
  static uint8_t counter = 0;
  static uint32_t elapsed_timer = C_TIME_READ_TEMPERATURE_MS;
  static uint32_t critical_timer = 0;

  // Read each 50 ms
  if (timer > elapsed_timer) {
    elapsed_timer = timer + C_TIME_READ_TEMPERATURE_MS;

    // Read ADC value
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    raw = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    // Converto to degree Celsius
    volts = (float)raw * C_REFERENCE_VOLTAGE_V / C_ADC_RESOLUTION_BITS;
    temperature_st->temperature = (uint8_t)(volts * C_LM35_MILIVOLTS_TO_DEGREES);

    // Set message on display
    if(temperature_st->temperature >= 50) {
      sprintf(temperature_st->msg, "M. CRIT");
    } else if(temperature_st->temperature >= 35) {
      sprintf(temperature_st->msg, "M. AQUE");
    } else {
      sprintf(temperature_st->msg, " ");
    }

    // Check if temperature is above max
    if (temperature_st->temperature >= C_CRITICAL_TEMPERATURE_C) {
      counter = counter >= C_COUNTS_ALERT_OVERHEAT ? counter : counter + 1;

      // Check if above max temp for more than 5 readings
      if (counter >= C_COUNTS_ALERT_OVERHEAT && temperature_st->alert == false) {
        // Enable temperature alert
        temperature_st->alert = true;
        // Get timestamp of the begin of the alert
        critical_timer = timer + C_TIME_CRITICAL_TEMPERATURE;
      }
      
      // If temperature alert for more than C_TIME_CRITICAL_TEMPERATURE
      if (temperature_st->alert && timer > critical_timer) {
        temperature_st->critical = true;
      }

    // Reset temperature alerts
    } else {
      counter = 0;
      temperature_st->alert = false;
      temperature_st->critical = false;
    }
  }
}
