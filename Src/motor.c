#include "motor.h"

void ROB_Motor_initDriver(ROB_Motor_Driver* driver, TIM_TypeDef* tim, GPIO_TypeDef* leftGpioBoard, GPIO_TypeDef* rightGpioBoard, uint16_t leftDirectionPin, uint16_t rightDirectionPin)
{
  driver->leftCCR = &tim->CCR2;
  driver->rightCCR = &tim->CCR3;
  driver->leftGpioBoard = leftGpioBoard;
  driver->rightGpioBoard = rightGpioBoard;
  driver->leftDirectionPin = leftDirectionPin;
  driver->rightDirectionPin = rightDirectionPin;
}

void ROB_Motor_SetDirVel(ROB_Motor_Driver* driver, float left, float right)
{
  uint32_t max_duty_cycle = (1l << 16) - 1;

  float left_factor = (left >= 0) ? left : 1 + left;
  *driver->leftCCR = max_duty_cycle * left_factor;
  GPIO_PinState left_set = (left >= 0) ? GPIO_PIN_RESET : GPIO_PIN_SET;
  HAL_GPIO_WritePin(driver->leftGpioBoard, driver->leftDirectionPin, left_set);

  float right_factor = (right > 0) ? 1 - right : -right;
  *driver->rightCCR = max_duty_cycle * right_factor;
  GPIO_PinState right_set = (right > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(driver->rightGpioBoard, driver->rightDirectionPin, right_set);
}
