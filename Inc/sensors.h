/*
 * sensors.h
 *
 *  Created on: Jul 29, 2021
 *      Author: max
 */

#include "stm32l4xx_hal.h"

typedef struct {
  volatile uint32_t *buffer;
  uint16_t numSensors;
} ROB_Sensors_Driver;

#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

void ROB_Sensors_initDriver(ROB_Sensors_Driver* driver, volatile uint32_t *buffer, uint16_t num_sensors);

void ROB_Sensors_logSensors(ROB_Sensors_Driver* driver, UART_HandleTypeDef* huart);

#endif /* INC_SENSORS_H_ */
