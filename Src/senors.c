/*
 * senors.c
 *
 *  Created on: Jul 29, 2021
 *      Author: max
 */

#include "sensors.h"
#include <stdio.h>

void ROB_Sensors_initDriver(ROB_Sensors_Driver* driver, volatile uint32_t *buffer, uint16_t num_sensors)
{
  driver->buffer = buffer;
  driver->numSensors = num_sensors;
}

void ROB_Sensors_logSensors(ROB_Sensors_Driver* driver, UART_HandleTypeDef* huart)
{
  char* labels[6] = {"lineSensor_middle", "encoder_left", "lineSensor_right", "battery", "encoder_right", "lineSensor_left"};
  char string_buf[100];
  uint16_t i = 0;
  for (uint32_t value = driver->buffer[i]; i < driver->numSensors; value = driver->buffer[++i])
  {
    uint8_t len = sprintf((char*)string_buf, "%s=%ld ; ", labels[i], value);
//    uint8_t len = sprintf((char*)string_buf, "%ld ; ", value);
    HAL_UART_Transmit(huart, (uint8_t*) string_buf, len, 1000000);
  }
    HAL_UART_Transmit(huart, (uint8_t*) "\n", 1, 1000000);
}
