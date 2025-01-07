/**
 * @file main.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef MAIN_H_
#define MAIN_H_

/* Includes */
#include "stm32f1xx_hal.h"
#include "device.h"
#include "display/display.h"
#include "keyboard/keyboard.h"

/* Prototypes */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Error_Handler(void);

#endif /* MAIN_H_ */