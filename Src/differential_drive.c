/*
 * differential_drive.c
 *
 *  Created on: Aug 2, 2021
 *      Author: max
 */

#include "differential_drive.h"

#include "usart.h"
#include <stdio.h>

float controlVelocity(float vel, float reference, float measured, float integration_factor);

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
  driver->lastTick = HAL_GetTick();
  driver->running = 0;
}

void ROB_Differential_DriveDistance(ROB_Differential_Driver* driver, int32_t distance, float reference_vel)
{
  ROB_Motor_ResetEncoders(driver->motors);

  float left_vel = 0.2;
  float right_vel = 0.2;

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
  if (!driver->running) return;
  if ((HAL_GetTick() - driver->lastTick) < 50) return;
  driver->lastTick = HAL_GetTick();

  ROB_Encoder_Driver* encoder_left = driver->motors->leftEncoder;
  ROB_Encoder_Driver* encoder_right =  driver->motors->rightEncoder;
  int32_t left_measured_vel = encoder_left->wheelCircumference/ encoder_left->numWheelSegments * encoder_left->velocity;
  int32_t right_measured_vel = encoder_right->wheelCircumference/ encoder_right->numWheelSegments * encoder_right->velocity;

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
    driver->referenceVelLeft = 0;
  }

  if (((driver->targetDistanceRight < 0) && (right_measured_distance > driver->targetDistanceRight))
      || ((driver->targetDistanceRight >= 0) && (right_measured_distance < driver->targetDistanceRight)))
  {
    driver->currentVelRight = controlVelocity(driver->currentVelRight, driver->referenceVelRight, right_measured_vel, driver->integrationFactor);
  }
  else
  {
    driver->currentVelRight = 0;
    driver->referenceVelRight = 0;
  }

//  char string_buf[100];
//  uint8_t len = sprintf((char*) string_buf, "left_measured: %f, left_set: %f, right_measured: %f, right_set: %f ", left_measured_vel, driver->currentVelLeft, right_measured_vel, driver->currentVelRight);
//  HAL_UART_Transmit(&huart2, (uint8_t*) string_buf, len, 1000000);
//  len = sprintf((char*) string_buf, "left_reference: %f, right_reference: %f \n", driver->referenceVelLeft, driver->referenceVelRight);
//  HAL_UART_Transmit(&huart2, (uint8_t*) string_buf, len, 1000000);

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

float controlVelocity(float vel, float reference, float measured, float integration_factor)
{
    float correction = (reference - measured) * integration_factor;
    return vel + correction;
}

void ROB_Differential_Rotate(ROB_Differential_Driver* driver, int16_t angle, float reference_vel)
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

void ROB_Differential_Start(ROB_Differential_Driver* driver)
{
  driver->running = 1;
}

void ROB_Differential_Stop(ROB_Differential_Driver* driver)
{
  driver->running = 0;
}
