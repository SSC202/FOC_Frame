#ifndef __CURR_SAMPLE_H
#define __CURR_SAMPLE_H

#include "stm32h7xx.h"

/**************** 结构体定义 **************** */

typedef enum {
    CURR_SAMPLE_GET_OFFSET, // 采集偏移值
    CURR_SAMPLE_RUNNING     // 采集运行值
} CURR_SAMPLE_FLAG;

typedef struct {
    int adc_val_u;                // u相ADC值
    int adc_val_v;                // v相ADC值
    float adc_val_u_offset;         // u相ADC偏移
    float adc_val_v_offset;         // v相ADC偏移
    float curr_u;                 // u相电流
    float curr_v;                 // v相电流
    float curr_w;                 // w相电流
    CURR_SAMPLE_FLAG sample_flag; // ADC采样状态
} CURR_SAMPLE_t;

/**************** 接口定义 **************** */

extern CURR_SAMPLE_t Load_M_Curr;

void adc_2_curr(CURR_SAMPLE_t *_curr_sample);

#endif