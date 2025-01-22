/**
 * @file    自定义数学函数
 */
#ifndef __MY_MATH_H
#define __MY_MATH_H

#include "math.h"

#define M_PI     3.141592653589793f // PI
#define T_SAMPLE 1e-4               // 采样时间

typedef struct {
    float KP;        // PID参数P
    float KI;        // PID参数I
    float KD;        // PID参数D
    float fdb;       // PID反馈值
    float ref;       // PID目标值
    float cur_error; // 当前误差
    float error[2];  // 前两次误差
    float output;    // 输出值
    float outputMax; // 最大输出值的绝对值
    float outputMin; // 最小输出值的绝对值用于防抖
} PID_t;

extern PID_t Load_id_pi;
extern PID_t Load_iq_pi;
extern PID_t Load_speed_pi;

float normalize(int pole_pairs, float mechine_angle, float offset);

void PID_init(PID_t *pid, float kp, float ki, float kd, float outputMax);
void PID_Calc(PID_t *pid);

#endif