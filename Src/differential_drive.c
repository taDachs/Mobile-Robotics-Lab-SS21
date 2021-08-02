/*
 * differential_drive.c
 *
 *  Created on: Aug 2, 2021
 *      Author: max
 */

#include "differential_drive.h"

#include "usart.h"
#include <stdio.h>

float controlVelocity(float vel, int16_t reference, int16_t measured, float integration_factor);

void ROB_Differential_InitDriver(ROB_Differential_Driver* driver, ROB_Motor_Driver* motors, float integration_factor, uint32_t axis_width)
{
  driver->motors = motors;
  driver->integrationFactor = integration_factor;
  driver->axisWidth = axis_width;
  driver->currentVelLeft = 0;
  driver->currentVelRight = 0;
  driver->referenceVelLeft = 0;
  driver->referenceVelRight = 0;
  driver->targetDistanceLeft = 0;
  driver->targetDistanceRight = 0;
}

void ROB_Differential_DriveDistance(ROB_Differential_Driver* driver, int32_t distance, int16_t reference_vel)
{
  ROB_Motor_ResetEncoders(driver->motors);

  float left_vel = 0.5;
  float right_vel = 0.5;

  if (distance < 0)
  {
    left_vel *= -1;
    right_vel *= -1;
    driver->referenceVelLeft = -reference_vel;
    driver->referenceVelRight = -reference_vel;
  }
  else
  {
    driver->referenceVelLeft = reference_vel;
    driver->referenceVelRight = reference_vel;
  }

  driver->targetDistanceLeft = distance;
  driver->currentVelLeft = left_vel;

  driver->targetDistanceRight = distance;
  driver->currentVelRight = right_vel;

  ROB_Motor_SetDirVel(driver->motors, driver->currentVelLeft, driver->currentVelRight);
}

void ROB_Differential_Update(ROB_Differential_Driver* driver)
{
  int16_t left_measured_vel = ROB_Encoder_GetVelocity(driver->motors->leftEncoder);
  int16_t right_measured_vel = ROB_Encoder_GetVelocity(driver->motors->rightEncoder);

  int16_t left_measured_distance = ROB_Encoder_GetDistance(driver->motors->leftEncoder);
  int16_t right_measured_distance = ROB_Encoder_GetDistance(driver->motors->rightEncoder);

  if (((driver->targetDistanceLeft < 0) && (left_measured_distance > driver->targetDistanceLeft))
      || ((driver->targetDistanceLeft >= 0) && (left_measured_distance < driver->targetDistanceLeft)))
  {
    driver->currentVelLeft = controlVelocity(driver->currentVelLeft, driver->referenceVelLeft, left_measured_vel, driver->integrationFactor);
  }
  else
  {
    driver->currentVelLeft = 0;
  }

  if (((driver->targetDistanceRight < 0) && (right_measured_distance > driver->targetDistanceRight))
      || ((driver->targetDistanceRight >= 0) && (right_measured_distance < driver->targetDistanceRight)))
  {
    driver->currentVelRight = controlVelocity(driver->currentVelRight, driver->referenceVelRight, right_measured_vel, driver->integrationFactor);
  }
  else
  {
    driver->currentVelRight = 0;
  }

  ROB_Motor_SetDirVel(driver->motors, driver->currentVelLeft, driver->currentVelRight);
}

uint8_t ROB_Differential_IsDone(ROB_Differential_Driver* driver)
{
  int16_t left_measured_distance = ROB_Encoder_GetDistance(driver->motors->leftEncoder);
  int16_t right_measured_distance = ROB_Encoder_GetDistance(driver->motors->rightEncoder);
  return !((((driver->targetDistanceLeft < 0) && (left_measured_distance > driver->targetDistanceLeft))
      || ((driver->targetDistanceLeft >= 0) && (left_measured_distance < driver->targetDistanceLeft)))
      && (((driver->targetDistanceRight < 0) && (right_measured_distance > driver->targetDistanceRight))
      || ((driver->targetDistanceRight >= 0) && (right_measured_distance < driver->targetDistanceRight))));
}

float controlVelocity(float vel, int16_t reference, int16_t measured, float integration_factor)
{
    float correction = (reference - measured) * integration_factor;
    return vel + correction;
}

void ROB_Differential_Rotate(ROB_Differential_Driver* driver, int16_t angle, uint16_t reference_vel)
{
  ROB_Motor_ResetEncoders(driver->motors);

  float pi = 3.14159265359f;
  int32_t turning_circle_circumference = pi * driver->axisWidth;
  int32_t turning_distance = turning_circle_circumference * (angle / 360.0);

  float left_vel = 0.5;
  float right_vel = 0.5;

  if (angle < 0) {
    turning_distance *= -1;
    driver->targetDistanceLeft = -turning_distance;
    driver->currentVelLeft = -left_vel;
    driver->referenceVelLeft = -reference_vel;

    driver->targetDistanceRight = turning_distance;
    driver->currentVelRight = right_vel;
    driver->referenceVelRight = reference_vel;
  }
  else
  {
    driver->targetDistanceLeft = turning_distance;
    driver->currentVelLeft = left_vel;
    driver->referenceVelLeft = reference_vel;

    driver->targetDistanceRight = -turning_distance;
    driver->currentVelRight = -right_vel;
    driver->referenceVelRight = -reference_vel;
  }

  ROB_Motor_SetDirVel(driver->motors, driver->currentVelLeft, driver->currentVelRight);
}
