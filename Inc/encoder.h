/*
 * encoder.h
 *
 *  Created on: Aug 1, 2021
 *      Author: max
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "sensors.h"
#include "stm32l4xx_hal.h"

typedef enum { ROB_ENCODER_UP, ROB_ENCODER_DOWN, } ROB_Encoder_State;

typedef enum { ROB_ENCODER_FORWARD, ROB_ENCODER_BACKWARD, } ROB_Encoder_Direction;

typedef struct {
  ROB_Sensors_AnalogSensor* sensor;
  uint32_t upperThresh;
  uint32_t lowerThresh;
  uint16_t numWheelSegments;
  uint16_t wheelCircumference;
  float velocity;
  int32_t ticks;
  int32_t last_tick_time;
  int32_t last_tick_amount;
  ROB_Encoder_State state;
  ROB_Encoder_Direction direction;
} ROB_Encoder_Driver;

void ROB_Encoder_InitDriver(ROB_Encoder_Driver* driver, ROB_Sensors_AnalogSensor* sensor, uint32_t upper_thresh, uint32_t lower_thresh, uint16_t num_wheel_segments, uint16_t wheel_circumference);

void ROB_Encoder_Update(ROB_Encoder_Driver* driver);

// returns vel in mm/s
int32_t ROB_Encoder_GetVelocity(ROB_Encoder_Driver* driver);

// returns distance in mm/s
int32_t ROB_Encoder_GetDistance(ROB_Encoder_Driver* driver);

void ROB_Encoder_Reset(ROB_Encoder_Driver* driver);

#endif /* INC_ENCODER_H_ */
