#include "usermain.h"

dq_t u_dq;
abc_t u_abc;
duty_abc_t duty_abc;
int i;
int cnt[3];

float theta;
float speed;

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}

/**
 * @brief   外设及配置初始化
 */
static void init(void)
{
    // 等待系统稳定
    HAL_Delay(50);
    // 旋变初始化
    AD2S1210_Init();
    //
    u_dq.d = 0;
    u_dq.q = 0;
    speed  = 0;
    theta  = 0;
    // 开启逆变器输出
    HAL_TIM_Base_Start_IT(&htim8);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_3);
    //
}

/**
 * @brief   主函数
 */
void usermain(void)
{
    init();
    while (1) {
        // if (i == 20000) {
        //     HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_7);
        //     i = 0;
        // }
        AD2S1210_Angle_Get();
        printf("U:%d,%d,%d\n", cnt[0], cnt[1], cnt[2]);
    }
}

// 中断函数段

/**
 * @brief   定时器中断
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM8) {
        dq_2_abc(&u_abc, &u_dq, theta);
        e_svpwm(&u_abc, 101, &duty_abc);
        TIM8->CCR1 = duty_abc.dutya * TIM8->ARR;
        TIM8->CCR2 = duty_abc.dutyb * TIM8->ARR;
        TIM8->CCR3 = duty_abc.dutyc * TIM8->ARR;
        cnt[0]     = TIM8->CCR1;
        cnt[1]     = TIM8->CCR2;
        cnt[2]     = TIM8->CCR3;
        theta      = theta + speed * 0.0001f;
        i++;
    }
}