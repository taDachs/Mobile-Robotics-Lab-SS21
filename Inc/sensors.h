/*
 * sensors.h
 *
 *  Created on: Jul 29, 2021
 *      Author: max
 */
#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

#include "stm32l4xx_hal.h"
#include "gpio_pin.h"

typedef struct {
  volatile uint32_t *adcBuffer;
  uint16_t numSensors;
  ROB_GPIO_Pin* leftTouchSensor;
  ROB_GPIO_Pin* middleTouchSensor;
  ROB_GPIO_Pin* rightTouchSensor;
} ROB_Sensors_Driver;

typedef struct {
  ROB_Sensors_Driver* driver;
  uint16_t index;
} ROB_Sensors_AnalogSensor;

typedef struct {
  ROB_Sensors_AnalogSensor* sensor;
  uint32_t thresh;
} ROB_Sensors_LineSensor;

void ROB_Sensors_InitAnalogSensor(ROB_Sensors_AnalogSensor* sensor, ROB_Sensors_Driver* driver, uint16_t index);
void ROB_Sensors_InitLineSensor(ROB_Sensors_LineSensor* sensor, ROB_Sensors_AnalogSensor* analog_sensor, uint32_t thresh);

uint32_t ROB_Sensors_AnalogRead(ROB_Sensors_AnalogSensor* sensor);
uint8_t ROB_Sensors_LineRead(ROB_Sensors_LineSensor* sensor);

void ROB_Sensors_InitDriver(ROB_Sensors_Driver* driver, volatile uint32_t *adc_buffer, uint16_t num_sensors, ROB_GPIO_Pin* left_touch_sensor, ROB_GPIO_Pin* middle_touch_sensor, ROB_GPIO_Pin* right_touch_sensor);
void ROB_Sensors_LogSensors(ROB_Sensors_Driver* driver, UART_HandleTypeDef* huart);

uint8_t ROB_Sensors_LeftPressed(ROB_Sensors_Driver* driver);
uint8_t ROB_Sensors_MiddlePressed(ROB_Sensors_Driver* driver);
uint8_t ROB_Sensors_RightPressed(ROB_Sensors_Driver* driver);

#endif /* INC_SENSORS_H_ */
