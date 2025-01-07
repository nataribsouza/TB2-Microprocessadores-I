/* Include */
#include "device.h"

/* Variables */
extern TIM_HandleTypeDef htim3;

/* Functions */

/**
 * @brief Init device struct
 * 
 * @param device_st 
 */
void init_device(st_device *device_st) {
  device_st->state_en = ENUM_STATE_READ_BUTTONS;
  device_st->buzzer_st.alarm = false;
  device_st->buzzer_st.elapsed_time_up = 0;
  device_st->buzzer_st.elapsed_time_down = 0;

  device_st->display_st.screen_en = ENUM_SCREEN_INSTANT_SPEED;
  sprintf(device_st->display_st.row0, LCD_SCREEN_INSTANT_SPEED_ROW0);
  sprintf(device_st->display_st.row1, LCD_SCREEN_INSTANT_SPEED_ROW1, device_st->instant_speed);
  device_st->display_st.update = true;

  device_st->temperature_st.alert = false;
  device_st->temperature_st.critical = false;
  device_st->temperature_st.temperature = C_BASE_TEMPERATURE_DEGREE;
  device_st->instant_speed = 0;
  device_st->average_speed = 0;
  device_st->fuel_range = 0;
}

/**
 * @brief Configure PWM in TIM3 pin PA7 
 * with 50 Hz frequency
 * 
 */
void init_PWM(void) {
  // Enable clock in GPIOA and TIM3
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

  // Set pin PA& as alternative output push-pull (PWM)
  GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
  GPIOA->CRL |= (GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0);
  GPIOA->CRL |= GPIO_CRL_CNF7_1;

  // Set TIM3 as PWM generator
  TIM3->PSC = TIM_PWM_PRESCALER;
  TIM3->ARR = TIM_PWM_AUTORELOAD;
  
  TIM3->CCMR1 &= ~TIM_CCMR1_OC2M; 
  TIM3->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1; 
  TIM3->CCMR1 |= TIM_CCMR1_OC2PE;

  // Enable output in channel 2
  TIM3->CCER |= TIM_CCER_CC2E;
  // Enable timer
  TIM3->CR1 |= TIM_CR1_CEN;

  // Start duty cycle
  TIM3->CCR2 = 0;
}

/**
 * @brief Manage device functioning
 *
 * @param device_st
 */
void state_machine(st_device *device_st) {
  st_buzzer *buzzer_st = &(device_st->buzzer_st);
  st_temperature *temperature_st = &(device_st->temperature_st);
  st_tank *tank_st = &(device_st->tank_st);

  // Switch between states
  switch (device_st->state_en) {
    // Read buttons
    case ENUM_STATE_READ_BUTTONS:
      run_keyboard();
      device_st->state_en = ENUM_STATE_READ_TEMPERATURE;
      break;

    // Read temperature
    case ENUM_STATE_READ_TEMPERATURE:
      read_temperature(temperature_st);
      device_st->state_en = ENUM_STATE_READ_POTENTIOMETER;
      break;

    // Read potentiometer
    case ENUM_STATE_READ_POTENTIOMETER:
      run_tank(tank_st);
      device_st->state_en = ENUM_STATE_MANAGE_SYSTEMS;
      break;

    // Manage internal systems
    case ENUM_STATE_MANAGE_SYSTEMS:
      manage_system(device_st);
      device_st->state_en = ENUM_STATE_MANAGE_LEDS;
      break;

    // Legacy state (don't used)
    case ENUM_STATE_MANAGE_LEDS:
      // handle_leds();
      device_st->state_en = ENUM_STATE_MANAGE_BUZZER;
      break;

    // Manage buzzer
    case ENUM_STATE_MANAGE_BUZZER:
      run_buzzer(buzzer_st);
      device_st->state_en = ENUM_STATE_UPDATE_DISPLAY;
      break;

    // Update display
    case ENUM_STATE_UPDATE_DISPLAY:
      handle_display(device_st);
      display_update(device_st);
      device_st->state_en = ENUM_STATE_READ_BUTTONS;
      break;

    default:
      /* Weak state reached. Rebooting*/
      NVIC_SystemReset();
      break;
  }
}

/**
 * @brief Calculate speed, odometer and
 * other important values for the system functioning
 * 
 * @param device_st 
 */
void manage_system(st_device *device_st) {
  // Handle injection
  manage_injection(device_st);
  // Calculate average speed
  calculate_average_speed(device_st);

  // Handle LED's
  if(device_st->tank_st.critical) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
  }

  // Handle buzzer on/off
  if(device_st->temperature_st.alert) {
    device_st->buzzer_st.alarm = true;
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
  } else {
    device_st->buzzer_st.alarm = false;
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
  }
}

/**
 * @brief Handle PWM injection functioning
 * 
 * @param device_st 
 */
void manage_injection(st_device *device_st) {
  uint8_t new_speed = 0;
  uint32_t timer = HAL_GetTick();
  static uint32_t elapsed_pedals = C_TIME_INJECTION_MS;
  static uint32_t elapsed_idle = C_TIME_IDLE_MS;

  // Verify each 20 ms
  if(timer > elapsed_pedals) {
    elapsed_pedals = timer + C_TIME_INJECTION_MS;

    // Handle injection based on pedals pressing
    if(read_keyboard(ENUM_BUTTON_BREAK) && !device_st->temperature_st.critical) {
      new_speed = device_st->instant_speed - C_BREAK_PWM_SLOP > 0 ? device_st->instant_speed - C_BREAK_PWM_SLOP : 0;
      device_st->instant_speed = new_speed;
      set_PWM(device_st->instant_speed);
    } else if(read_keyboard(ENUM_BUTTON_GAS) && !device_st->temperature_st.critical) {
      new_speed = device_st->instant_speed + C_GAS_PWM_SLOP < TIM_PWM_MAX_DUTY_CYCLE ? device_st->instant_speed + C_GAS_PWM_SLOP : TIM_PWM_MAX_DUTY_CYCLE;
      device_st->instant_speed = new_speed;
      set_PWM(device_st->instant_speed);
    } else if(timer > elapsed_idle) {
      elapsed_idle = timer + C_TIME_IDLE_MS;
      
      // Update instant speed and PWM
      new_speed = device_st->instant_speed - C_GAS_PWM_SLOP > 0 ? device_st->instant_speed - C_GAS_PWM_SLOP : 0;
      device_st->instant_speed = new_speed;
      set_PWM(device_st->instant_speed);
    }
  }
}

/**
 * @brief Calculate average speed
 * 
 * @param device_st 
 */
void calculate_average_speed(st_device *device_st) {
  uint8_t lodometer = 0;
  static uint8_t index = 0;
  static uint8_t speed[2] = {0};
  uint32_t timer = HAL_GetTick();
  static uint32_t elapsed_time_speed = C_TIME_GET_INSTANT_SPEED_MS;
  static uint32_t elapsed_time_average = C_TIME_TO_CALCULATE_AVERAGE_SPEED_MS;

  // Store instant speed each 500 ms
  if(timer > elapsed_time_speed) {
    elapsed_time_speed = timer + C_TIME_GET_INSTANT_SPEED_MS;

    speed[index] = device_st->instant_speed;
    index = index + 1 > 1 ? 0 : 1;
  }

  // Update average speed each 1 s
  if(timer > elapsed_time_average) {
    elapsed_time_average = timer + C_TIME_TO_CALCULATE_AVERAGE_SPEED_MS;

    lodometer = device_st->odometer;

    device_st->average_speed = (speed[0] + speed[1]) / 2;
    device_st->odometer += device_st->average_speed * C_KMH_TO_METER;

    // Update autonomi
    if(device_st->average_speed > 0){
      device_st->autonomi = (uint8_t)(device_st->odometer - lodometer) / (0.2 * (device_st->average_speed / 10));
    }
  }
}

/**
 * @brief Handle display screens
 * 
 * @param device_st 
 */
void handle_display(st_device *device_st) {
  st_display *display_st = &(device_st->display_st);
  uint32_t timer = HAL_GetTick();
  static uint32_t time_elapsed_display = TIME_UPDATE_SCREEN_INSTANT_SPEED_MS;

  // Menus of display
  switch (display_st->screen_en) {
    // Instant speed screen
    case ENUM_SCREEN_INSTANT_SPEED:
      // Print screen 
      if(timer > time_elapsed_display) {
        time_elapsed_display = timer + TIME_UPDATE_SCREEN_INSTANT_SPEED_MS;
        sprintf(device_st->display_st.row0, LCD_SCREEN_INSTANT_SPEED_ROW0);
        sprintf(device_st->display_st.row1, LCD_SCREEN_INSTANT_SPEED_ROW1, device_st->instant_speed);
        device_st->display_st.update = true;
      }

      // Manage menu navigation
      if(read_keyboard(ENUM_BUTTON_PREVIOUS)) {
        display_st->screen_en = ENUM_SCREEN_AUTONOMI;
      } else if(read_keyboard(ENUM_BUTTON_NEXT)) {
        display_st->screen_en = ENUM_SCREEN_AVERAGE_SPEED;
      }
      break;

    // Average speed screen
    case ENUM_SCREEN_AVERAGE_SPEED:
      // Update screen
      if(timer > time_elapsed_display) {
        time_elapsed_display = timer + TIM_UPDATE_SCREEN_AVERAGE_SPEED_MS;
        sprintf(device_st->display_st.row0, LCD_SCREEN_AVERAGE_SPEED_ROW0);
        sprintf(device_st->display_st.row1, LCD_SCREEN_AVERAGE_SPEED_ROW1, device_st->average_speed);
        device_st->display_st.update = true;
      }

      // Manage menu navigation
      if(read_keyboard(ENUM_BUTTON_PREVIOUS)) {
        display_st->screen_en = ENUM_SCREEN_INSTANT_SPEED;
      } else if(read_keyboard(ENUM_BUTTON_NEXT)) {
        display_st->screen_en = ENUM_SCREEN_ODOMETER;
      }
      break;

    // Odometer screen
    case ENUM_SCREEN_ODOMETER:
      // Print screen
      if(timer > time_elapsed_display) {
        time_elapsed_display = timer + TIME_UPDATE_SCREEN_ODOMETER_MS;
        sprintf(device_st->display_st.row0, LCD_SCREEN_ODOMETER_ROW0);
        sprintf(device_st->display_st.row1, LCD_SCREEN_ODOMETER_ROW1, device_st->odometer);
        device_st->display_st.update = true;
      }

      // Manage menu navigatoin
      if(read_keyboard(ENUM_BUTTON_PREVIOUS)) {
        display_st->screen_en = ENUM_SCREEN_AVERAGE_SPEED;
      } else if(read_keyboard(ENUM_BUTTON_NEXT)) {
        display_st->screen_en = ENUM_SCREEN_TEMPERATURE;
      }
      break;

    // Temperature screen
    case ENUM_SCREEN_TEMPERATURE:
      // Print screen
      if(timer > time_elapsed_display) {
        time_elapsed_display = timer + TIME_UPDATE_SCREEN_TEMPERATURE_MS;
        sprintf(device_st->display_st.row0, LCD_SCREEN_TEMPERATURE_ROW0);
        sprintf(device_st->display_st.row1, LCD_SCREEN_TEMPERATURE_ROW1, device_st->temperature_st.temperature, device_st->temperature_st.msg);
        device_st->display_st.update = true;
      }

      // Manage menu navigation
      if(read_keyboard(ENUM_BUTTON_PREVIOUS)) {
        display_st->screen_en = ENUM_SCREEN_ODOMETER;
      } else if(read_keyboard(ENUM_BUTTON_NEXT)) {
        display_st->screen_en = ENUM_SCREEN_GASOLINE_LEVEL;
      }
      break;

    // Gasoline screen
    case ENUM_SCREEN_GASOLINE_LEVEL:
      // Print screen
      if(timer > time_elapsed_display) {
        time_elapsed_display = timer + TIME_UPDATE_SCREEN_GASOLINE_LEVEL_MS;
        sprintf(device_st->display_st.row0, LCD_SCREEN_GASOLINE_LEVEL_ROW0);
        sprintf(device_st->display_st.row1, LCD_SCREEN_GASOLINE_LEVEL_ROW1, device_st->tank_st.volume);
        device_st->display_st.update = true;
      }

      // Manage menu navigation
      if(read_keyboard(ENUM_BUTTON_PREVIOUS)) {
        display_st->screen_en = ENUM_SCREEN_TEMPERATURE;
      } else if(read_keyboard(ENUM_BUTTON_NEXT)) {
        display_st->screen_en = ENUM_SCREEN_AUTONOMI;
      }
      break;

    // Autonomi screen
    case ENUM_SCREEN_AUTONOMI:
      // Print screen
      if(timer > time_elapsed_display) {
        time_elapsed_display = timer + TIME_UPDATE_SCREEN_AUTONOMI_MS;
        sprintf(device_st->display_st.row0, LCD_SCREEN_AUTONOMI_ROW0);
        sprintf(device_st->display_st.row1, LCD_SCREEN_AUTONOMI_ROW1, device_st->autonomi);
        device_st->display_st.update = true;
      }

      // Manage menu navigation
      if(read_keyboard(ENUM_BUTTON_PREVIOUS)) {
        display_st->screen_en = ENUM_SCREEN_GASOLINE_LEVEL;
      } else if(read_keyboard(ENUM_BUTTON_NEXT)) {
        display_st->screen_en = ENUM_SCREEN_INSTANT_SPEED;
      }
      break;
  }
}

/**
 * @brief Update display 
 * 
 * @param device_st 
 */
void display_update(st_device *device_st) {
    st_display *display_st = &(device_st->display_st);

    if(display_st->update) {
        display_st->update = false;

        display_clear();
        display_setCursor(0, 0);
        display_print(display_st->row0);
        display_setCursor(1, 0);
        display_print(display_st->row1);

        display_temp_alert(device_st->temperature_st.alert);
        display_gasoline_alert(device_st->tank_st.critical);
    }
}

/**
 * @brief Set PWM duty cycle
 * 
 * @param duty_cycle 
 */
void set_PWM(uint8_t duty_cycle) {
  TIM3->CCR2 = (int)(TIM_PWM_AUTORELOAD / 100) * duty_cycle / 100;
}