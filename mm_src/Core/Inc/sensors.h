/*
 * sensors.h
 *
 *  Created on: May 10, 2026
 *      Author: norma
 */

#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

#include "stm32f2xx_hal.h"

void IR_EmittersOn(void);
void IR_EmittersOff(void);
uint32_t IR_Read(uint8_t sensor_index);
uint8_t IR_WallDetected(uint8_t sensor_index);
uint8_t IR_BotLeft_WallDetected(void);

#endif /* INC_SENSORS_H_ */
