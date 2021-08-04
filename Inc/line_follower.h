/*
 * line_follower.h
 *
 *  Created on: Aug 3, 2021
 *      Author: max
 */

#ifndef INC_LINE_FOLLOWER_H_
#define INC_LINE_FOLLOWER_H_

#include "stm32l4xx_hal.h"
#include "motor.h"
#include "sensors.h"
#include "differential_drive.h"

typedef enum { ROTATE_LEFT, ROTATE_RIGHT, ROTATE_BACK, SEEK, NONE, } ROB_LineFollower_Recovery;
typedef enum { LEFT, RIGHT, } ROB_LineFollower_LastCurve;

typedef struct {
  ROB_Motor_Driver* motors;
  ROB_Differential_Driver* diffDriver;
  ROB_Sensors_LineSensor* left;
  ROB_Sensors_LineSensor* middle;
  ROB_Sensors_LineSensor* right;
  float leftVel;
  float rightVel;
  float baseSpeed;
  uint8_t running;
  float factor;
  uint32_t lastTick;
  uint32_t lostSinceTick;
  uint32_t recoveredSinceTick;
  ROB_LineFollower_Recovery recoveryState;
  ROB_LineFollower_LastCurve lastCurve;
} ROB_LineFollower_Driver;

void ROB_LineFollower_InitDriver(ROB_LineFollower_Driver* driver, ROB_Motor_Driver* motors, ROB_Differential_Driver* diff_driver, ROB_Sensors_LineSensor* left, ROB_Sensors_LineSensor* middle, ROB_Sensors_LineSensor* right);

void ROB_LineFollower_Start(ROB_LineFollower_Driver* driver);

void ROB_LineFollower_Stop(ROB_LineFollower_Driver* driver);

void ROB_LineFollower_Update(ROB_LineFollower_Driver* driver);

#endif /* INC_LINE_FOLLOWER_H_ */
