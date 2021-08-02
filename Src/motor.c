#include "motor.h"

void ROB_Motor_InitDriver(ROB_Motor_Driver* driver, ROB_Encoder_Driver* left_encoder, ROB_Encoder_Driver* right_encoder, TIM_TypeDef* tim, ROB_GPIO_Pin* leftDirectionPin, ROB_GPIO_Pin* rightDirectionPin)
{
  driver->leftEncoder = left_encoder;
  driver->rightEncoder = right_encoder;
  driver->leftCCR = &tim->CCR2;
  driver->rightCCR = &tim->CCR3;
  driver->leftDirectionPin = leftDirectionPin;
  driver->rightDirectionPin = rightDirectionPin;
}

void ROB_Motor_SetDirVel(ROB_Motor_Driver* driver, float left, float right)
{
  // max value is 1.0, min is -1.0
  left = (left > 1.0) ? 1.0 : ((left < -1.0) ? -1.0 : left);
  right = (right > 1.0) ? 1.0 : ((right < -1.0) ? -1.0 : right);

  uint32_t max_duty_cycle = (1l << 16) - 1;

  float left_factor = (left >= 0) ? left : 1 + left;
  *driver->leftCCR = max_duty_cycle * left_factor;
  uint8_t left_set = (left >= 0) ? 0 : 1;
  ROB_GPIO_WriteValue(driver->leftDirectionPin, left_set);

  float right_factor = (right > 0) ? 1 - right : -right;
  *driver->rightCCR = max_duty_cycle * right_factor;
  uint8_t right_set = (right > 0) ? 1 : 0;
  ROB_GPIO_WriteValue(driver->rightDirectionPin, right_set);

  driver->leftEncoder->direction = (left >= 0) ? ROB_ENCODER_FORWARD : ROB_ENCODER_BACKWARD;
  driver->rightEncoder->direction = (right >= 0) ? ROB_ENCODER_FORWARD : ROB_ENCODER_BACKWARD;
}

void ROB_Motor_ResetEncoders(ROB_Motor_Driver* driver)
{
  ROB_Encoder_Reset(driver->leftEncoder);
  ROB_Encoder_Reset(driver->rightEncoder);
}
