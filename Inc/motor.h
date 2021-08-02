#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32l4xx_hal.h"
#include "gpio_pin.h"
#include "encoder.h"

typedef struct {
  ROB_Encoder_Driver* leftEncoder;
  ROB_Encoder_Driver* rightEncoder;
  volatile uint32_t* leftCCR;
  volatile uint32_t* rightCCR;
  ROB_GPIO_Pin* leftDirectionPin;
  ROB_GPIO_Pin* rightDirectionPin;
} ROB_Motor_Driver;

void ROB_Motor_InitDriver(ROB_Motor_Driver* driver, ROB_Encoder_Driver* left_encoder, ROB_Encoder_Driver* right_encoder, TIM_TypeDef* tim, ROB_GPIO_Pin* leftDirectionPin, ROB_GPIO_Pin* rightDirectionPin);

void ROB_Motor_SetDirVel(ROB_Motor_Driver* driver, float left, float right);

void ROB_Motor_ResetEncoders(ROB_Motor_Driver* driver);

#endif /* __MOTOR_H__*/
