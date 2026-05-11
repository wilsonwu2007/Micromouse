/*
 * global_c.h
 *
 *  Created on: May 10, 2026
 *      Author: norma
 */

#ifndef INC_GLOBAL_C_H_
#include <stdlib.h>
#include "stm32f2xx_hal.h"

#define INC_GLOBAL_C_H_

#define NUM_IR_SENSORS 3
#define IR_THRESHOLD 2000

#define IR_TOP_LEFT 0
#define IR_TOP_RIGHT 1
#define IR_BOTTOM_RIGHT 2

//motor
#define MOTOR_LEFT   0
#define MOTOR_RIGHT  1
#define PWM_MAX      1000

//encoders
int32_t prev_count_left = 0;
int32_t prev_count_right = 0;
uint32_t prev_tick = 0;

// PID tuning - start with these, tune on hardware
#define KP  1.0f
#define KI  0.0f
#define KD  0.0f

#endif /* INC_GLOBAL_C_H_ */
