/*
 * sensors.c
 *
 *  Created on: May 10, 2026
 *      Author: norma
 */

#include "global_c.h"
#include <stdlib.h>
#include "stm32f2xx_hal.h"

//Sensor reading pins

uint32_t adc_buf[NUM_IR_SENSORS];

//reads sensor data
uint32_t IR_Read(uint8_t sensor_index) {
	return adc_buf[sensor_index];
}

//sees if there is a wall
uint8_t IR_WallDetected(uint8_t sensor_index) {
	if (IR_Read(sensor_index) > IR_THRESHOLD) {
		return 1;
	}
	return 0;
}

uint8_t IR_BotLeft_WallDetected(void) {
	// LOW means wall detected (phototransistor pulls line down when it sees IR)
	return (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_RESET) ? 1 : 0;
}

//emiter code
void IR_EmittersOn(void) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_9,
			GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
}

void IR_EmittersOff(void) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_9,
			GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}
