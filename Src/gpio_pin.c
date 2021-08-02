/*
 * gpio_pin.c
 *
 *  Created on: Jul 30, 2021
 *      Author: max
 */

#include "gpio_pin.h"

void ROB_GPIO_SetPin(ROB_GPIO_Pin* pin)
{
  HAL_GPIO_WritePin(pin->board, pin->pin, GPIO_PIN_SET);
}

void ROB_GPIO_ResetPin(ROB_GPIO_Pin* pin)
{
  HAL_GPIO_WritePin(pin->board, pin->pin, GPIO_PIN_RESET);
}

void ROB_GPIO_WriteValue(ROB_GPIO_Pin* pin, uint8_t value)
{
  GPIO_PinState state = value ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(pin->board, pin->pin, state);
}

uint8_t ROB_GPIO_ReadValue(ROB_GPIO_Pin* pin)
{
  return HAL_GPIO_ReadPin(pin->board, pin->pin);
}
