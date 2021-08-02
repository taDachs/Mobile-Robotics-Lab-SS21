/*
 * senors.c
 *
 *  Created on: Jul 29, 2021
 *      Author: max
 */

#include "sensors.h"
#include <stdio.h>

void ROB_Sensors_InitSensor(ROB_Sensors_AnalogSensor* sensor, ROB_Sensors_Driver* driver, uint16_t index)
{
  sensor->driver = driver;
  sensor->index = index;
}

uint32_t ROB_Sensors_Read(ROB_Sensors_AnalogSensor* sensor)
{
  return sensor->driver->adcBuffer[sensor->index];
}

void ROB_Sensors_InitDriver(ROB_Sensors_Driver* driver, volatile uint32_t *adc_buffer, uint16_t num_sensors, ROB_GPIO_Pin* left_touch_sensor, ROB_GPIO_Pin* middle_touch_sensor, ROB_GPIO_Pin* right_touch_sensor)
{
  driver->adcBuffer = adc_buffer;
  driver->numSensors = num_sensors;
  driver->leftTouchSensor = left_touch_sensor;
  driver->middleTouchSensor = middle_touch_sensor;
  driver->rightTouchSensor = right_touch_sensor;
}

void ROB_Sensors_LogSensors(ROB_Sensors_Driver* driver, UART_HandleTypeDef* huart)
{
//  char* labels[6] = {"lineSensor_middle", "encoder_left", "lineSensor_right", "battery", "encoder_right", "lineSensor_left"};
  char string_buf[100];
  uint16_t i = 0;
  for (uint32_t value = driver->adcBuffer[i]; i < driver->numSensors; value = driver->adcBuffer[++i])
  {
    uint8_t len = sprintf((char*)string_buf, "%ld ; ", value);
    HAL_UART_Transmit(huart, (uint8_t*) string_buf, len, 1000000);
  }
    HAL_UART_Transmit(huart, (uint8_t*) "\n", 1, 1000000);
}

uint8_t ROB_Sensors_LeftPressed(ROB_Sensors_Driver* driver)
{
  return !ROB_GPIO_ReadValue(driver->leftTouchSensor);
}

uint8_t ROB_Sensors_MiddlePressed(ROB_Sensors_Driver* driver)
{
  return !ROB_GPIO_ReadValue(driver->middleTouchSensor);
}

uint8_t ROB_Sensors_RightPressed(ROB_Sensors_Driver* driver)
{
  return !ROB_GPIO_ReadValue(driver->rightTouchSensor);
}

