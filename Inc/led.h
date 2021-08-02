/*
 * led.h
 *
 *  Created on: Jul 31, 2021
 *      Author: max
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "gpio_pin.h"

typedef struct {
  ROB_GPIO_Pin* back;
  ROB_GPIO_Pin* left;
  ROB_GPIO_Pin* right;
} ROB_LED_Driver;

void ROB_LED_InitDriver(ROB_LED_Driver* driver, ROB_GPIO_Pin* back, ROB_GPIO_Pin* left, ROB_GPIO_Pin* right);


#endif /* INC_LED_H_ */
