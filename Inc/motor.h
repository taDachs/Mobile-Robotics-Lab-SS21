#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32l4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  volatile uint32_t* leftCCR;
  volatile uint32_t* rightCCR;
  GPIO_TypeDef* leftGpioBoard;
  GPIO_TypeDef* rightGpioBoard;
  uint16_t leftDirectionPin;
  uint16_t rightDirectionPin;
} ROB_Motor_Driver;

void ROB_Motor_initDriver(ROB_Motor_Driver* driver, TIM_TypeDef* tim, GPIO_TypeDef* leftGpioBoard, GPIO_TypeDef* rightGpioBoard, uint16_t leftDirectionPin, uint16_t rightDirectionPin);

void ROB_Motor_SetDirVel(ROB_Motor_Driver* driver, float left, float right);

#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_H__*/
