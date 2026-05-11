/*
 * motors.c
 *
 *  Created on: May 10, 2026
 *      Author: norma
 */
#include "global_c.h"
#include <stdlib.h>
#include "stm32f2xx_hal.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;

PID_t pid_left  = {0};
PID_t pid_right = {0};

void Motor_Init(void) {
	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	//start encoder count
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

void Motor_Set(uint8_t motor, int16_t speed) {
	if (motor == MOTOR_LEFT) {
		if (speed > 0) {
			// IN1 (TIM13) = speed, IN2 (TIM14) = 0
			__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, speed);
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);
		} else if (speed < 0) {
			// IN1 (TIM13) = 0, IN2 (TIM14) = abs(speed)
			__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, (uint32_t )(-speed));
		} else {
			// both 0
			__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);
		}
	} else {
		if (speed > 0) {
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed);
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
		} else if (speed < 0) {

			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, (uint32_t )(-speed));
		} else {
			// both 0
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
		}
	}
}

void Motor_Stop(uint8_t motor) {
	Motor_Set(motor, 0);
}

void Motor_StopAll(void) {
	Motor_Set(MOTOR_LEFT, 0);
	Motor_Set(MOTOR_RIGHT, 0);
}

//encoder fucntions
int32_t Encoder_Read(uint8_t motor) {
	if (motor == MOTOR_LEFT) {
		return (int32_t) __HAL_TIM_GET_COUNTER(&htim3);
	} else {
		return (int32_t) __HAL_TIM_GET_COUNTER(&htim1);
	}
}

float Encoder_GetSpeed(uint8_t motor) {
	int32_t currEnc = 0;
	if (motor == MOTOR_LEFT) {
		currEnc = Encoder_Read(MOTOR_LEFT);
	} else {
		currEnc = Encoder_Read(MOTOR_RIGHT);
	}

	uint32_t currTick = HAL_GetTick();
	uint32_t deltaT = currTick - prev_tick;

	int32_t deltaEnc = 0;
	if (motor == MOTOR_LEFT) {
		deltaEnc = currEnc - prev_count_left;
		prev_count_left = currEnc;
	} else {
		deltaEnc = currEnc - prev_count_right;
		prev_count_right = currEnc;
	}

	prev_tick = currTick;

	if (deltaT == 0)
		return 0.0f;  // avoid divide by zero

	return (float) deltaEnc / (float) deltaT;
}

//PID CONTROL
float PID_Compute(PID_t *pid, float measured, float dt) {
    // 1. calculate error = setpoint - measured
	float error = pid->setpoint - measured;
    // 2. accumulate integral += error * dt
	pid->integral+=error*dt;
    // 3. calculate derivative = (error - prev_error) / dt
	float deritative = (error - pid->prev_error)/dt;
    // 4. update prev_error = error
	pid->prev_error = error;
    // 5. return kp*error + ki*integral + kd*derivative
	return pid->kp*error+pid+pid->ki*pid->integral+pid->kd+deritative;
}

void PID_Reset(PID_t *pid) {
    // clear integral and prev_error
	pid->integral = 0;
	pid->prev_error = 0;
}

void PID_SetGains(PID_t *pid, float kp, float ki, float kd) {
    // assign kp, ki, kd to struct
	pid->kp = kp;
	pid->kd = kd;
	pid->ki = ki;
}
