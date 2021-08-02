/*
 * differential_drive.c
 *
 *  Created on: Aug 2, 2021
 *      Author: max
 */

#include "differential_drive.h"

#include "usart.h"
#include <stdio.h>

float controlVelocity(float vel, int16_t reference, int16_t measured, float integrationFactor);
void turnLeft(ROB_Differential_Driver* driver, int32_t distance, int16_t reference_vel);
void turnRight(ROB_Differential_Driver* driver, int32_t distance, int16_t reference_vel);

void ROB_Differential_InitDriver(ROB_Differential_Driver* driver, ROB_Motor_Driver* motors, float integration_factor, uint32_t axis_width)
{
  driver->motors = motors;
  driver->integrationFactor = integration_factor;
  driver->axisWidth = axis_width;
}

void ROB_Differential_DriveDistance(ROB_Differential_Driver* driver, int32_t distance, int16_t reference_vel)
{
  ROB_Motor_ResetEncoders(driver->motors);

  float left_vel = 0.2;
  float right_vel = 0.2;

  if (distance < 0) {
    left_vel *= -1;
    right_vel *= -1;
  }

  while (((ROB_Encoder_GetDistance(driver->motors->leftEncoder) + ROB_Encoder_GetDistance(driver->motors->rightEncoder)) / 2) < distance)
  {
    ROB_Motor_SetDirVel(driver->motors, left_vel, right_vel);

    int16_t left_measured_vel = ROB_Encoder_GetVelocity(driver->motors->leftEncoder);
    int16_t right_measured_vel = ROB_Encoder_GetVelocity(driver->motors->rightEncoder);

    left_vel = controlVelocity(left_vel, reference_vel, left_measured_vel, driver->integrationFactor);
    right_vel = controlVelocity(right_vel, reference_vel, right_measured_vel, driver->integrationFactor);
  }
  ROB_Motor_SetDirVel(driver->motors, 0, 0);
}

float controlVelocity(float vel, int16_t reference, int16_t measured, float integrationFactor)
{
    float correction = (reference - measured) * integrationFactor;
    return vel + correction;
}

void ROB_Differential_Rotate(ROB_Differential_Driver* driver, int16_t angle, uint16_t reference_vel)
{
  ROB_Motor_ResetEncoders(driver->motors);

  float pi = 3.14159265359f;
  int32_t turning_circle_circumference = pi * driver->axisWidth;
  int32_t turning_distance = turning_circle_circumference * (angle / 360.0);

  if (angle < 0) {
    turnLeft(driver, -1 * turning_distance, reference_vel);
  }
  else
  {
    turnRight(driver, turning_distance, reference_vel);
  }
}

void turnLeft(ROB_Differential_Driver* driver, int32_t distance, int16_t reference_vel)
{
  float left_vel = -0.5;
  float right_vel = 0.5;

  while ((ROB_Encoder_GetDistance(driver->motors->leftEncoder) > -distance) && (ROB_Encoder_GetDistance(driver->motors->rightEncoder) < distance))
  {
    ROB_Motor_SetDirVel(driver->motors, left_vel, right_vel);

//    int16_t left_measured_vel = ROB_Encoder_GetVelocity(driver->motors->leftEncoder);
//    int16_t right_measured_vel = ROB_Encoder_GetVelocity(driver->motors->rightEncoder);
//
//    left_vel = controlVelocity(left_vel, -reference_vel, left_measured_vel, driver->integrationFactor);
//    right_vel = controlVelocity(right_vel, reference_vel, right_measured_vel, driver->integrationFactor);
  }
  ROB_Motor_SetDirVel(driver->motors, 0, 0);
}

void turnRight(ROB_Differential_Driver* driver, int32_t distance, int16_t reference_vel)
{
  float left_vel = 0.5;
  float right_vel = -0.5;

  while ((ROB_Encoder_GetDistance(driver->motors->leftEncoder) < distance) && (ROB_Encoder_GetDistance(driver->motors->rightEncoder) > -distance))
  {
    ROB_Motor_SetDirVel(driver->motors, left_vel, right_vel);

//    int16_t left_measured_vel = ROB_Encoder_GetVelocity(driver->motors->leftEncoder);
//    int16_t right_measured_vel = ROB_Encoder_GetVelocity(driver->motors->rightEncoder);
//
//    left_vel = controlVelocity(left_vel, reference_vel, left_measured_vel, driver->integrationFactor);
//    right_vel = controlVelocity(right_vel, -reference_vel, right_measured_vel, driver->integrationFactor);
  }
  ROB_Motor_SetDirVel(driver->motors, 0, 0);
}
