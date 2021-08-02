/*
 * robot.h
 *
 *  Created on: Jul 30, 2021
 *      Author: max
 */

#ifndef INC_ROBOT_H_
#define INC_ROBOT_H_

#include "stm32l4xx_hal.h"

typedef struct {
  uint32_t velLeftWheel; // velocity in mm/s
  uint32_t velRightWheel; // velocity in mm/s
  uint32_t x; // position in millimeters from origin
  uint32_t y; // position in millimeters from origin
} ROB_Robot_State;




#endif /* INC_ROBOT_H_ */
