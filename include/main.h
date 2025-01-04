#ifndef MAIN_H_
#define MAIN_H_

/* Includes */
#include "stm32f1xx_hal.h"
#include "device.h"
#include "display/display.h"

/* Prototypes */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Error_Handler(void);

#endif /* MAIN_H_ */