#include "svpwm.h"

/**
 * @brief   取最大值函数
 */
static float get_max(float a, float b, float c)
{
    float ret = 0;
    if (a >= b && a >= c)
        ret = a;
    else if (b >= a && b >= c)
        ret = b;
    else
        ret = c;
    return ret;
}

/**
 * @brief   取最小值函数
 */
static float get_min(float a, float b, float c)
{
    float ret = 0;
    if (a <= b && a <= c)
        ret = a;
    else if (b <= a && b <= c)
        ret = b;
    else
        ret = c;
    return ret;
}

/**
 * @brief   取中间值函数
 */
static float get_middle(float a, float b, float c)
{
    float ret = 0;
    ret       = a + b + c - get_max(a, b, c) - get_min(a, b, c);
    return ret;
}

/**
 * @brief       SVPWM 占空比生成函数
 * @attention   使用三次谐波注入法
 * @param       _u_ref       参考电压
 * @param       Udc         母线电压
 * @param       _duty_abc   输出占空比
 */
void e_svpwm(abc_t *_u_ref, float Udc, duty_abc_t *_duty_abc)
{
    float ua_ref_eq  = _u_ref->a + 0.5f * get_middle(_u_ref->a, _u_ref->b, _u_ref->c);
    _duty_abc->dutya = 0.5f + ua_ref_eq / Udc;
    float ub_ref_eq  = _u_ref->b + 0.5f * get_middle(_u_ref->a, _u_ref->b, _u_ref->c);
    _duty_abc->dutyb = 0.5f + ub_ref_eq / Udc;
    float uc_ref_eq  = _u_ref->c + 0.5f * get_middle(_u_ref->a, _u_ref->b, _u_ref->c);
    _duty_abc->dutyc = 0.5f + uc_ref_eq / Udc;
}