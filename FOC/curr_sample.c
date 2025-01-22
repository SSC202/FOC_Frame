/**
 * @file    三相线电流采样相关函数
 * @attention   1. ADC 相关
 *                 H7 ADC 是 16 位的(0-65535映射),经过电压检查验证基本上是严格的线性关系。
 *                 ADC * 3.3 / 65535 = Voltage
 *              2. M 新动力电流采样相关
 *                 该逆变器使用采集相线电流的方式进行电流采集;
 *                 上电时 ADC 平均值进行采集得到 0A 的传感器输出电压值;
 *                 然后使用 9.6 A/V 的线性关系计算相线电流。
 */
#include "curr_sample.h"

CURR_SAMPLE_t Load_M_Curr;

/********************************** 用户函数段 **************************************** */

/**
 * @brief   将 ADC 值转换为电流值
 */
void adc_2_curr(CURR_SAMPLE_t *_curr_sample)
{
    _curr_sample->curr_u = 9.6f * (_curr_sample->adc_val_u - _curr_sample->adc_val_u_offset) * 3.3f / 65535.f;
    _curr_sample->curr_v = 9.6f * (_curr_sample->adc_val_v - _curr_sample->adc_val_v_offset) * 3.3f / 65535.f;
    _curr_sample->curr_w = -_curr_sample->curr_u - _curr_sample->curr_v;
}
