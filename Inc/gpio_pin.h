/*
 * gpio.h
 *
 *  Created on: Jul 30, 2021
 *      Author: max
 */

#ifndef INC_GPIO_PIN_H_
#define INC_GPIO_PIN_H_

#include "gpio.h"
#include "gpio_pin.h"

typedef struct {
  GPIO_TypeDef* board;
  uint16_t pin;
} ROB_GPIO_Pin;

void ROB_GPIO_SetPin(ROB_GPIO_Pin* pin);
void ROB_GPIO_ResetPin(ROB_GPIO_Pin* pin);
void ROB_GPIO_WriteValue(ROB_GPIO_Pin* pin, uint8_t value);
uint8_t ROB_GPIO_ReadValue(ROB_GPIO_Pin* pin);

#endif /* INC_GPIO_PIN_H_ */
