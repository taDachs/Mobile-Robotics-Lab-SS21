/*
 * line_follower.c
 *
 *  Created on: Aug 3, 2021
 *      Author: max
 */

#include "line_follower.h"

void seekLine(ROB_LineFollower_Driver* driver);
void capVelocity(ROB_LineFollower_Driver* driver);
void graduallyReturnToBaseSpeed(ROB_LineFollower_Driver* driver);


void ROB_LineFollower_InitDriver(ROB_LineFollower_Driver* driver, ROB_Motor_Driver* motors, ROB_Differential_Driver* diff_driver, ROB_Sensors_LineSensor* left, ROB_Sensors_LineSensor* middle, ROB_Sensors_LineSensor* right)
{

  driver->motors = motors;
  driver->diffDriver = diff_driver;
  driver->left = left;
  driver->middle = middle;
  driver->right = right;
  driver->leftVel = 0;
  driver->rightVel = 0;
  driver->baseSpeed = 0.5;
  driver->running = 0;
  driver->factor = 0.0001;
  driver->lastTick = HAL_GetTick();
  driver->lostSinceTick = 0;
  driver->recoveredSinceTick = 0;
  driver->recoveryState = NONE;
  driver->lastCurve = RIGHT;
}

void ROB_LineFollower_Start(ROB_LineFollower_Driver* driver)
{
  driver->leftVel = driver->baseSpeed;
  driver->rightVel = driver->baseSpeed;
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
  ROB_LineFollower_LastCurve last_curve;

  if (ROB_Sensors_LineRead(driver->left))
  {
    if (driver->recoveredSinceTick == 0) driver->recoveredSinceTick = HAL_GetTick();
    last_curve = LEFT;

    driver->rightVel += driver->factor;
    driver->leftVel -= driver->factor;
  }
  else if (ROB_Sensors_LineRead(driver->right))
  {
    if (driver->recoveredSinceTick == 0) driver->recoveredSinceTick = HAL_GetTick();
    last_curve = RIGHT;

    driver->leftVel += driver->factor;
    driver->rightVel -= driver->factor;
  }
  else if (!ROB_Sensors_LineRead(driver->right) && ROB_Sensors_LineRead(driver->middle) && !ROB_Sensors_LineRead(driver->left))
  {
    graduallyReturnToBaseSpeed(driver);
  }
  else if (!ROB_Sensors_LineRead(driver->right) && !ROB_Sensors_LineRead(driver->middle) && !ROB_Sensors_LineRead(driver->left))
  {
    seekLine(driver);
    return;
  }

  capVelocity(driver);

  uint8_t has_recovered = (HAL_GetTick() - driver->recoveredSinceTick) > 50;
  if (!has_recovered) return;

  driver->lastCurve = last_curve;

  ROB_Differential_DriveDistance(driver->diffDriver, 0, 0);
  ROB_Differential_Stop(driver->diffDriver);
  driver->recoveryState = NONE;
  if (driver->lostSinceTick != 0) driver->lostSinceTick = 0;
  ROB_Motor_SetDirVel(driver->motors, driver->leftVel, driver->rightVel);
}

void graduallyReturnToBaseSpeed(ROB_LineFollower_Driver* driver)
{
    if (driver->recoveredSinceTick == 0) driver->recoveredSinceTick = HAL_GetTick();
    if (driver->leftVel > driver->baseSpeed)
    {
      driver->leftVel -= driver->factor;
    }
    if (driver->leftVel < driver->baseSpeed)
    {
      driver->leftVel += driver->factor;
    }
    if (driver->rightVel > driver->baseSpeed)
    {
      driver->rightVel -= driver->factor;
    }
    if (driver->rightVel < driver->baseSpeed)
    {
      driver->rightVel += driver->factor;
    }
}

void capVelocity(ROB_LineFollower_Driver* driver)
{
    if (driver->rightVel > 1) driver->rightVel = 1;
    if (driver->rightVel < 0.2) driver->rightVel = 0.2;
    if (driver->leftVel > 1) driver->leftVel = 1;
    if (driver->leftVel < 0.2) driver->leftVel = 0.2;
}

void seekLine(ROB_LineFollower_Driver* driver)
{
    if (driver->lostSinceTick == 0)
    {
      driver->lostSinceTick = HAL_GetTick();
      driver->recoveryState = NONE;
      return;
    }

    driver->recoveredSinceTick = 0;

    if ((HAL_GetTick() - driver->lostSinceTick) > 300)
    {
      ROB_Differential_Start(driver->diffDriver);
      switch (driver->recoveryState)
      {
      case NONE:
        switch (driver->lastCurve)
        {
        case LEFT:
          driver->recoveryState = ROTATE_LEFT;
          ROB_Differential_Rotate(driver->diffDriver, -90, 100);
          break;
        case RIGHT:
          driver->recoveryState = ROTATE_RIGHT;
          ROB_Differential_Rotate(driver->diffDriver, 90, 100);
          break;
        }
        break;
      case ROTATE_RIGHT:
        if (ROB_Differential_IsDone(driver->diffDriver))
        {
          switch (driver->lastCurve)
          {
          case LEFT:
            ROB_Differential_Rotate(driver->diffDriver, -90, 100);
            driver->recoveryState = ROTATE_BACK;
            break;
          case RIGHT:
            ROB_Differential_Rotate(driver->diffDriver, -180, 100);
            driver->recoveryState = ROTATE_LEFT;
            break;
          }
        }
        break;
      case ROTATE_LEFT:
        if (ROB_Differential_IsDone(driver->diffDriver))
        {
          switch (driver->lastCurve)
          {
          case LEFT:
            ROB_Differential_Rotate(driver->diffDriver, 180, 100);
            driver->recoveryState = ROTATE_RIGHT;
            break;
          case RIGHT:
            ROB_Differential_Rotate(driver->diffDriver, 90, 100);
            driver->recoveryState = ROTATE_BACK;
            break;
          }
        }
        break;
      case ROTATE_BACK:
        if (ROB_Differential_IsDone(driver->diffDriver))
        {
          ROB_Differential_DriveDistance(driver->diffDriver, 150, 100);
          driver->recoveryState = SEEK;
        }
        break;
      case SEEK:
        if (ROB_Differential_IsDone(driver->diffDriver)) driver->recoveryState = NONE;
        break;
      }
    }
}
