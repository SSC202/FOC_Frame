#include "my_math.h"
#include "usermain.h"

/**
 * @brief   机械角度转换为电角度函数并归一化
 */
float normalize(int pole_pairs, float mechine_angle, float offset)
{
    float electric_angle = pole_pairs * mechine_angle - offset;
    float out            = electric_angle;
    while (out < -M_PI) {
        out = out + 2 * M_PI;
    }
    while (out > M_PI) {
        out = out - 2 * M_PI;
    }
    return out;
}

/****************************** PID 控制器 ********************************* */

// Curr PID
PID_t Load_id_pi;
PID_t Load_iq_pi;
// Speed PID 
PID_t Load_speed_pi;

/**
 * @brief   通用 PID 控制器初始化
 * @param   pid         PID 控制器句柄
 * @param   kp          Kp
 * @param   ki          Ki
 * @param   kd          Kd
 * @param   outputMax   最大输出值
 * @attention   PID 为增量式
 */
void PID_init(PID_t *pid, float kp, float ki, float kd, float outputMax)
{
    pid->KP        = kp;
    pid->KI        = ki;
    pid->KD        = kd;
    pid->outputMax = outputMax;
}

/**
 * @brief   增量式PID计算
 */
void PID_Calc(PID_t *pid)
{
    pid->ref       = pid->ref * system_enable;
    pid->fdb       = pid->fdb * system_enable;
    pid->cur_error = pid->ref - pid->fdb;
    pid->output += pid->KP * (pid->cur_error - pid->error[1]) + pid->KI * T_SAMPLE * pid->cur_error + pid->KD * (pid->cur_error - 2 * pid->error[1] + pid->error[0]);
    pid->output   = pid->output * system_enable;
    pid->error[0] = pid->error[1];
    pid->error[1] = pid->ref - pid->fdb;
    /*设定输出上限*/
    if (pid->output > pid->outputMax) pid->output = pid->outputMax;
    if (pid->output < -pid->outputMax) pid->output = -pid->outputMax;
}