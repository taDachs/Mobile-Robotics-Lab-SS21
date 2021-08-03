/*
 * encoder.c
 *
 *  Created on: Aug 1, 2021
 *      Author: max
 */
#include "encoder.h"
#include "usart.h"

void ROB_Encoder_InitDriver(ROB_Encoder_Driver* driver, ROB_Sensors_AnalogSensor* sensor, uint32_t upper_thresh, uint32_t lower_thresh, int16_t num_wheel_segments, int16_t wheel_circumference)
{
  driver->sensor = sensor;
  driver->upperThresh = upper_thresh;
  driver->lowerThresh = lower_thresh;
  driver->numWheelSegments = num_wheel_segments;
  driver->wheelCircumference = wheel_circumference;
  driver->velocity = 0;
  driver->ticks = 0;
  driver->last_tick_time = HAL_GetTick();
  driver->last_tick_amount = 0;
  driver->state = ROB_ENCODER_UP;
  driver->direction = ROB_ENCODER_FORWARD;
}

void ROB_Encoder_Update(ROB_Encoder_Driver* driver)
{
  uint32_t value = ROB_Sensors_AnalogRead(driver->sensor);

  int8_t tick_direction;
  switch (driver->direction)
  {
  case ROB_ENCODER_FORWARD:
    tick_direction = 1;
    break;
  case ROB_ENCODER_BACKWARD:
    tick_direction = -1;
    break;
  }

  switch (driver->state)
  {
  case ROB_ENCODER_UP:
    if (value >= driver->upperThresh)
    {
      driver->ticks += tick_direction;
      driver->state = ROB_ENCODER_DOWN;
    }
    break;
  case ROB_ENCODER_DOWN:
    if (value <= driver->lowerThresh)
    {
      driver->ticks += tick_direction;
      driver->state = ROB_ENCODER_UP;
    }
    break;
  }

  if ((HAL_GetTick() - driver->last_tick_time) > 50)
  {

    uint32_t interval = HAL_GetTick() - driver->last_tick_time;
    int32_t num_ticks = driver->ticks - driver->last_tick_amount;

    driver->velocity = (num_ticks / ((float) interval)) * 1000;



    driver->last_tick_amount = driver->ticks;
    driver->last_tick_time = HAL_GetTick();
  }
}

int32_t ROB_Encoder_GetVelocity(ROB_Encoder_Driver* driver)
{
  float factor = driver->wheelCircumference / (float) driver->numWheelSegments;

  return driver->velocity * factor;
}

int32_t ROB_Encoder_GetDistance(ROB_Encoder_Driver* driver)
{
  float factor = driver->wheelCircumference / driver->numWheelSegments;
  return driver->ticks * factor;
}

void ROB_Encoder_Reset(ROB_Encoder_Driver* driver)
{
  driver->ticks = 0;
  driver->velocity = 0;
  driver->last_tick_amount = 0;
  driver->last_tick_time = HAL_GetTick();
}
