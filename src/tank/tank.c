/* Includes */
#include "tank.h"

/* Variables */
extern ADC_HandleTypeDef hadc2;

/* Functions */
void init_tank(st_tank *tank_st) {
  tank_st->volume = 0;
  tank_st->critical = false;
}

void run_tank(st_tank *tank_st) {
  uint32_t adc = 0;
  float v_readed = 0;
  float angle = 0;
  uint32_t timer = HAL_GetTick();
  static uint32_t elapsed_timer = TIME_READ_TANK_MS;

  if (timer > elapsed_timer) {
    elapsed_timer = timer + TIME_READ_TANK_MS;

    // Wait until convertion
    HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
    // Read ADC value
    adc = HAL_ADC_GetValue(&hadc2);

    v_readed = (adc * C_REFERENCE_VOLTAGE_V) / C_ADC_RESOLUTION_BITS;
    angle *= C_VOLT_TO_DEGREE;

    // Check if angle is between limits
    if (angle > MIM_VALID_ANGLE_DEGREE && angle < MAX_VALID_ANGLE_DEGREE) {
      // Convert angle to volume
      if (angle > ANGLE_MIDDLE_SCALE_DEGREES) {
        tank_st->volume =
            tan(angle - ANGLE_MIDDLE_SCALE_DEGREES) * C_MILIMITER_TO_VOLUME_L;
        tank_st->volume += VOLUME_MIDDLE_SCALE_L;
      } else {
        tank_st->volume =
            tan(ANGLE_MIDDLE_SCALE_DEGREES - angle) * C_MILIMITER_TO_VOLUME_L;
        tank_st->volume = VOLUME_MIDDLE_SCALE_L - tank_st->volume;
      }
    }

    // Check if fuel volume is critical
    if (tank_st->volume < C_MIN_FUEL_VOLUME_L) {
      tank_st->critical = true;
    } else {
      tank_st->critical = false;
    }
  }
}
