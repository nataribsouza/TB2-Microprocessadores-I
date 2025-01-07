/* Includes */
#include "buzzer.h"

/* Functions */
/**
 * @brief Manage buzzer functioning
 *
 * @param buzzer_st
 */
void run_buzzer(st_buzzer *buzzer_st) {
  uint8_t state = HAL_GPIO_ReadPin(PORT_BUZZER, PIN_BUZZER);
  uint32_t timer = HAL_GetTick();
  uint32_t *elapsed_time_up = &(buzzer_st->elapsed_time_up);
  uint32_t *elapsed_time_down = &(buzzer_st->elapsed_time_down);

  // Check if alert is on
  if (buzzer_st->alarm) {
    // Check if buzzer on and timer-on has passed
    if (state && timer > *elapsed_time_up) {
      *elapsed_time_down = timer + TIME_BEEP_DOWN_MS;
      HAL_GPIO_WritePin(PORT_BUZZER, PIN_BUZZER, GPIO_PIN_RESET);

      // Check if buzzer off and timer-off has passed
    } else if (!state && timer > *elapsed_time_down) {
      *elapsed_time_up = timer + TIME_BEEP_UP_MS;
      HAL_GPIO_WritePin(PORT_BUZZER, PIN_BUZZER, GPIO_PIN_SET);
    }
  } else {
    HAL_GPIO_WritePin(PORT_BUZZER, PIN_BUZZER, GPIO_PIN_RESET);
  }
}

/**
 * @brief Turn buzzer alarm on
 *
 * @param buzzer_st
 */
void buzzer_turnOn(st_buzzer *buzzer_st) {
  buzzer_st->elapsed_time_up = HAL_GetTick() + TIME_BEEP_UP_MS;
  buzzer_st->alarm = true;
  HAL_GPIO_WritePin(PORT_BUZZER, PIN_BUZZER, GPIO_PIN_SET);
}

/**
 * @brief Turn buzzer alarm off
 *
 * @param buzzer_st
 */
void buzzer_turnOff(st_buzzer *buzzer_st) {
  HAL_GPIO_WritePin(PORT_BUZZER, PIN_BUZZER, GPIO_PIN_RESET);
  buzzer_st->alarm = false;
}
