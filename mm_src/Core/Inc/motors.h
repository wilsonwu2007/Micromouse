/*
 * motors.h
 *
 *  Created on: May 10, 2026
 *      Author: norma
 */

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include "stm32f2xx_hal.h"
// PID struct
typedef struct {
    float kp, ki, kd;
    float integral;
    float prev_error;
    float setpoint;
} PID_t;

// PID instances - one per motor
extern PID_t pid_left;
extern PID_t pid_right;

// PID function declarations
float PID_Compute(PID_t *pid, float measured, float dt);
void PID_Reset(PID_t *pid);
void PID_SetGains(PID_t *pid, float kp, float ki, float kd);

void Motor_Init(void);
void Motor_Set(uint8_t motor, int16_t speed);
void Motor_Stop(uint8_t motor);
void Motor_StopAll(void);

#endif /* INC_MOTORS_H_ */
