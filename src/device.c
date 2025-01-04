/* Include */
#include "device.h"

/* Functions */

/**
 * @brief Init device struct
 * 
 * @param device_st 
 */
void init_device(st_device *device_st) {
  device_st->state_en = ENUM_STATE_READ_BUTTONS;
}

/**
 * @brief Manage device functioning
 *
 * @param device_st
 */
void state_machine(st_device *device_st) {
  st_buzzer *buzzer_st = &(device_st->buzzer_st);
  st_display *display_st = &(device_st->display_st);
  st_temperature *temperature_st = &(device_st->temperature_st);

  switch (device_st->state_en) {
    case ENUM_STATE_READ_BUTTONS:
      // run_buttons();
      device_st->state_en = ENUM_STATE_READ_TEMPERATURE;
      break;

    case ENUM_STATE_READ_TEMPERATURE:
      read_temperature(temperature_st);
      device_st->state_en = ENUM_STATE_READ_POTENTIOMETER;
      break;

    case ENUM_STATE_READ_POTENTIOMETER:
      // run_tank();
      device_st->state_en = ENUM_STATE_MANAGE_SYSTEMS;
      break;

    case ENUM_STATE_MANAGE_SYSTEMS:
      manage_system(device_st);
      device_st->state_en = ENUM_STATE_MANAGE_LEDS;
      break;

    case ENUM_STATE_MANAGE_LEDS:
      // run_leds();
      device_st->state_en = ENUM_STATE_MANAGE_BUZZER;
      break;

    case ENUM_STATE_MANAGE_BUZZER:
      // run_buzzer();
      device_st->state_en = ENUM_STATE_UPDATE_DISPLAY;
      break;

    case ENUM_STATE_UPDATE_DISPLAY:
      display_update(display_st);
      device_st->state_en = ENUM_STATE_READ_BUTTONS;
      break;

    default:
      /* Weak state reached. Rebooting*/
      NVIC_SystemReset();
  }
}

void manage_system(st_device *device_st) {
  manage_injection(device_st);

  float temperature = device_st->temperature_st.temperature;

  sprintf(device_st->display_st.row1, "Temp: %d", (int)temperature);
  device_st->display_st.update = true;
}

void manage_injection(st_device *device_st) {
  // uint32_t timer = HAL_GetTick();
  // static uint32_t elased_time_injection = C_TIME_INJECTION_MS;
  // static uint32_t elased_time_idle = C_TIME_IDLE_MS;

  // if(timer > elased_time_injection) {
  //   elased_time_injection = timer + C_TIME_INJECTION_MS;

  //   if(/* Check if breaks are on */) {
  //     elased_time_idle = timer + C_TIME_IDLE_MS;

  //   } else if (/* Check if gas are on */) {
  //     elased_time_idle = timer + C_TIME_IDLE_MS;

  //   } else if (/* In idle case, check if time has passed */) {
      
  //   }
  // }
}