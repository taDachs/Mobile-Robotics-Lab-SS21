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
} ROB_Differential_Driver;

void ROB_Differential_InitDriver(ROB_Differential_Driver* driver, ROB_Motor_Driver* motors, float integration_factor, uint32_t axis_width);

void ROB_Differential_DriveDistance(ROB_Differential_Driver* driver, int32_t distance, int16_t reference_vel);

void ROB_Differential_Rotate(ROB_Differential_Driver* driver, int16_t angle, uint16_t reference_vel);

#endif /* INC_DIFFERENTIAL_DRIVE_H_ */
