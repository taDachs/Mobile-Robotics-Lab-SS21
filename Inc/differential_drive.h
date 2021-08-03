/*
 * differential_driver.h
 *
 *  Created on: Aug 2, 2021
 *      Author: max
 */

#ifndef INC_DIFFERENTIAL_DRIVE_H_
#define INC_DIFFERENTIAL_DRIVE_H_

#include "stm32l4xx_hal.h"
#include "motor.h"

typedef struct {
  ROB_Motor_Driver* motors;
  float integrationFactor;
  uint32_t axisWidth;
  float currentVelLeft;
  float currentVelRight;
  float referenceVelLeft;
  float referenceVelRight;
  int32_t targetDistanceLeft;
  int32_t targetDistanceRight;
  int32_t lastTick;
} ROB_Differential_Driver;

void ROB_Differential_InitDriver(ROB_Differential_Driver* driver, ROB_Motor_Driver* motors, float integration_factor, uint32_t axis_width);

void ROB_Differential_DriveDistance(ROB_Differential_Driver* driver, int32_t distance, float reference_vel);

void ROB_Differential_Rotate(ROB_Differential_Driver* driver, int16_t angle, float reference_vel);

void ROB_Differential_Update(ROB_Differential_Driver* driver);

uint8_t ROB_Differential_IsDone(ROB_Differential_Driver* driver);


#endif /* INC_DIFFERENTIAL_DRIVE_H_ */
