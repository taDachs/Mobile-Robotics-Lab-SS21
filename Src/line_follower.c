/*
 * line_follower.c
 *
 *  Created on: Aug 3, 2021
 *      Author: max
 */

#include "line_follower.h"


void ROB_LineFollower_InitDriver(ROB_LineFollower_Driver* driver, ROB_Motor_Driver* motors, ROB_Differential_Driver* diff_driver, ROB_Sensors_LineSensor* left, ROB_Sensors_LineSensor* middle, ROB_Sensors_LineSensor* right)
{

  driver->motors = motors;
  driver->diffDriver = diff_driver;
  driver->left = left;
  driver->middle = middle;
  driver->right = right;
  driver->leftVel = 0;
  driver->rightVel = 0;
  driver->running = 0;
  driver->factor = 0.01;
  driver->lastTick = HAL_GetTick();
  driver->lostSinceTick = 0;
  driver->recoveredSinceTick = 0;
  driver->recoveryState = NONE;
}

void ROB_LineFollower_Start(ROB_LineFollower_Driver* driver)
{
  driver->leftVel = 0.5;
  driver->rightVel = 0.5;
  driver->running = 1;
  driver->lastTick = HAL_GetTick();
  ROB_Motor_SetDirVel(driver->motors, driver->leftVel, driver->rightVel);
}

void ROB_LineFollower_Stop(ROB_LineFollower_Driver* driver)
{
  driver->leftVel = 0;
  driver->rightVel = 0;
  driver->running = 0;
  ROB_Motor_SetDirVel(driver->motors, driver->leftVel, driver->rightVel);
}

void ROB_LineFollower_Update(ROB_LineFollower_Driver* driver)
{
  if (!driver->running) return;

  if (ROB_Sensors_LineRead(driver->left))
  {
    if (driver->recoveredSinceTick == 0) driver->recoveredSinceTick = HAL_GetTick();
    driver->rightVel += driver->factor;
    driver->leftVel -= driver->factor;

    if (driver->rightVel > 1) driver->rightVel = 1;
    if (driver->leftVel < 0.1) driver->leftVel = 0.1;
  }
  else if (ROB_Sensors_LineRead(driver->right))
  {
    if (driver->recoveredSinceTick == 0) driver->recoveredSinceTick = HAL_GetTick();
    driver->leftVel += driver->factor;
    driver->rightVel -= driver->factor;

    if (driver->leftVel > 1) driver->leftVel = 1;
    if (driver->rightVel < 0.1) driver->rightVel = 0.1;
  }
  else if (!ROB_Sensors_LineRead(driver->right) && ROB_Sensors_LineRead(driver->middle) && !ROB_Sensors_LineRead(driver->left))
  {
    if (driver->recoveredSinceTick == 0) driver->recoveredSinceTick = HAL_GetTick();
    if (driver->leftVel > 0.5)
    {
      driver->leftVel -= driver->factor;
    }
    if (driver->leftVel < 0.5)
    {
      driver->leftVel += driver->factor;
    }
    if (driver->rightVel > 0.5)
    {
      driver->rightVel -= driver->factor;
    }
    if (driver->rightVel < 0.5)
    {
      driver->rightVel += driver->factor;
    }
  }
  else if (!ROB_Sensors_LineRead(driver->right) && !ROB_Sensors_LineRead(driver->middle) && !ROB_Sensors_LineRead(driver->left))
  {
    if (driver->lostSinceTick == 0)
    {
      driver->lostSinceTick = HAL_GetTick();
      driver->recoveryState = NONE;
      return;
    }

    driver->recoveredSinceTick = 0;

    if ((HAL_GetTick() - driver->lostSinceTick) > 100)
    {
      ROB_Differential_Start(driver->diffDriver);
      switch (driver->recoveryState)
      {
      case NONE:
        driver->recoveryState = ROTATE_LEFT;
        ROB_Differential_Rotate(driver->diffDriver, -90, 50);
        break;
      case ROTATE_LEFT:
        if (ROB_Differential_IsDone(driver->diffDriver))
        {
          ROB_Differential_Rotate(driver->diffDriver, 180, 50);
          driver->recoveryState = ROTATE_RIGHT;
        }
        break;
      case ROTATE_RIGHT:
        if (ROB_Differential_IsDone(driver->diffDriver))
        {
          ROB_Differential_Rotate(driver->diffDriver, -90, 50);
          driver->recoveryState = ROTATE_BACK;
        }
        break;
      case ROTATE_BACK:
        if (ROB_Differential_IsDone(driver->diffDriver))
        {
          ROB_Differential_DriveDistance(driver->diffDriver, 100, 100);
          driver->recoveryState = SEEK;
        }
        break;
      case SEEK:
        if (ROB_Differential_IsDone(driver->diffDriver)) driver->recoveryState = NONE;
        break;
      }
    }
    return;
  }

  uint8_t has_recovered = (HAL_GetTick() - driver->recoveredSinceTick) > 50;
  if (!has_recovered) return;


  ROB_Differential_DriveDistance(driver->diffDriver, 0, 0);
  ROB_Differential_Stop(driver->diffDriver);
  driver->recoveryState = NONE;
  if (driver->lostSinceTick != 0) driver->lostSinceTick = 0;
  ROB_Motor_SetDirVel(driver->motors, driver->leftVel, driver->rightVel);
}
