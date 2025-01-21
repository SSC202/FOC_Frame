/**
 * @file    坐标变换相关代码
 */
#ifndef __TF_H
#define __TF_H

#include "math.h"
#include "my_math.h"

/**
 * 结构体相关定义
 */

typedef struct
{
    float a;
    float b;
    float c;
} abc_t; // ABC 三相静止坐标系结构体

typedef struct
{
    float alpha;
    float beta;
} alpha_beta_t; // alpha-beta 两相静止坐标系结构体

typedef struct
{
    float d;
    float q;
} dq_t; // dq 旋转坐标系结构体

/**
 * 接口函数相关定义
 */
void abc_2_alphabeta(alpha_beta_t *_alpha_beta, abc_t *_a_b_c);
void abc_2_dq(dq_t *_d_q, abc_t *_a_b_c, float theta);
void alphabeta_2_abc(abc_t *_a_b_c, alpha_beta_t *_alpha_beta);
void alphabeta_2_dq(dq_t *_d_q, alpha_beta_t *_alpha_beta, float theta);
void dq_2_abc(abc_t *_a_b_c, dq_t *_d_q, float theta);
void dq_2_alphabeta(alpha_beta_t *_alpha_beta, dq_t *_d_q, float theta);

#endif // !_TF_H__