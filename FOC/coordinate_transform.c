#include "coordinate_transform.h"

/**
 * @brief   Clarke 变换
 * @param[out]   _alpha_beta alpha-beta坐标系物理量
 * @param[in]   _a_b_c      ABC坐标系物理量
 */
void abc_2_alphabeta(alpha_beta_t *_alpha_beta, abc_t *_a_b_c)
{
    _alpha_beta->alpha = sqrtf(2.f / 3.f) * ((1.f) * _a_b_c->a - (0.5f) * _a_b_c->b - (0.5f) * _a_b_c->c);
    _alpha_beta->beta  = sqrtf(2.f / 3.f) * ((0.f) * _a_b_c->a + (sqrtf(3.f) / 2.f) * _a_b_c->b - (sqrtf(3.f) / 2.f) * _a_b_c->c);
}

/**
 * @brief   Clarke 变换 + Park 变换
 * @param[in]    _a_b_c    ABC坐标系物理量
 * @param[out]   _d_q      dq坐标系物理量
 * @param[in]   theta      变换角度
 */
void abc_2_dq(dq_t *_d_q, abc_t *_a_b_c, float theta)
{
    _d_q->d = (2.f / 3.f) * ((cosf(theta))*_a_b_c->a + (cosf(theta - 2.f * M_PI / 3.f)) * _a_b_c->b + (cosf(theta + 2.f * M_PI / 3.f)) * _a_b_c->c);
    _d_q->q = -(2.f / 3.f) * ((sinf(theta))*_a_b_c->a + (sinf(theta - 2.f * M_PI / 3.f)) * _a_b_c->b - (sinf(theta + 2.f * M_PI / 3.f)) * _a_b_c->c);
}

/**
 * @brief   Clarke 反变换
 * @param[in]   _alpha_beta alpha-beta坐标系物理量
 * @param[out]   _a_b_c      ABC坐标系物理量
 */
void alphabeta_2_abc(abc_t *_a_b_c, alpha_beta_t *_alpha_beta)
{
    _a_b_c->a = sqrtf(2.f / 3.f) * ((1.f) * _alpha_beta->alpha - (0.f) * _alpha_beta->beta);
    _a_b_c->b = sqrtf(2.f / 3.f) * (-(0.5f) * _alpha_beta->alpha + (sqrtf(3.f) / 2.f) * _alpha_beta->beta);
    _a_b_c->c = sqrtf(2.f / 3.f) * (-(0.5f) * _alpha_beta->alpha - (sqrtf(3.f) / 2.f) * _alpha_beta->beta);
}

/**
 * @brief   Park 变换
 * @param[in]   _alpha_beta alpha-beta坐标系物理量
 * @param[out]  _d_q      dq坐标系物理量
 * @param[in]   theta      变换角度
 */
void alphabeta_2_dq(dq_t *_d_q, alpha_beta_t *_alpha_beta, float theta)
{
    _d_q->d = (cosf(theta))*_alpha_beta->alpha + (sinf(theta))*_alpha_beta->beta;
    _d_q->q = -(sinf(theta))*_alpha_beta->alpha + (cosf(theta))*_alpha_beta->beta;
}

/**
 * @brief   Clarke 反变换 + Park 反变换
 * @param[out]  _a_b_c    ABC坐标系物理量
 * @param[in]   _d_q      dq坐标系物理量
 * @param[in]   theta      变换角度
 */
void dq_2_abc(abc_t *_a_b_c, dq_t *_d_q, float theta)
{
    _a_b_c->a = cosf(theta) * _d_q->d - sinf(theta) * _d_q->q;
    _a_b_c->b = cosf(theta - 2.f * M_PI / 3.f) * _d_q->d - sinf(theta - 2.f * M_PI / 3.f) * _d_q->q;
    _a_b_c->c = cosf(theta + 2.f * M_PI / 3.f) * _d_q->d - sinf(theta + 2.f * M_PI / 3.f) * _d_q->q;
}

/**
 * @brief   Park 反变换
 * @param[out]  _alpha_beta alpha-beta坐标系物理量
 * @param[in]   _d_q      dq坐标系物理量
 * @param[in]   theta      变换角度
 */
void dq_2_alphabeta(alpha_beta_t *_alpha_beta, dq_t *_d_q, float theta)
{
    _alpha_beta->alpha = (cosf(theta))*_d_q->d - (sinf(theta))*_d_q->q;
    _alpha_beta->beta  = (sinf(theta))*_d_q->d + (cosf(theta))*_d_q->q;
}