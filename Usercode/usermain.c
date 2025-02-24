#include "usermain.h"

dq_t u_dq;
dq_t Load_idq;
abc_t Load_iabc;
abc_t u_abc;
duty_abc_t duty_abc;

float theta;
float speed;
float load_speed_ref;

uint8_t system_enable = 0; // 系统使能参数

/**
 * @brief   UART print to screen
 */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}

/**
 * @brief   Init Program
 */
static void init(void)
{
    // Wait
    HAL_Delay(50);
    // Init AD2S1210
    AD2S1210_Init();
    // Init parameters
    system_enable                = 0;
    u_dq.d                       = 0;
    u_dq.q                       = 0;
    load_speed_ref               = 0;
    Load_M_Curr.adc_val_u_offset = 0;
    Load_M_Curr.adc_val_v_offset = 0;
    Load_M_Curr.sample_flag      = CURR_SAMPLE_GET_OFFSET; // Read ADC Offset
    PID_init(&Load_id_pi, 0.02, 50, 0, 50);
    PID_init(&Load_iq_pi, 0.02, 50, 0, 50);
    PID_init(&Load_speed_pi, 0.1, 0.1, 0, 1);
    // Current Sample
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    __HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_JEOC);
    __HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_EOC);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
    HAL_ADCEx_InjectedStart_IT(&hadc1);
    // Wait for Read ADC Offset
    while (Load_M_Curr.sample_flag == CURR_SAMPLE_GET_OFFSET) {
        ;
    }
    // Open Inverter
    HAL_TIM_Base_Start_IT(&htim8);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_3);
}

/**
 * @brief   主函数
 */
void usermain(void)
{
    init();
    while (1) {

        // TODO: 主程序里准备一个停止运行的代码段

        // TODO: 同时准备一个恢复运行的代码段，准备将初始化代码移植到此处
        // printf("U:%d,%d,%d\n", cnt[0], cnt[1], cnt[2]);
        printf("U:%f,%f\n", load_speed_ref, Load_speed_pi.fdb);
    }
}

// 中断函数段

/**
 * @brief   定时器中断
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM8) {
        // // 角度和速度采样
        // AD2S1210_Angle_Get();
        // AD2S1210_Speed_Get();

        // // 采样电流变换为 dq 轴电流
        // abc_2_dq(&Load_idq, &Load_iabc, AD2S.Electrical_Angle);

        // // 速度 PI 控制器计算
        // Load_speed_pi.ref = load_speed_ref;
        // Load_speed_pi.fdb = AD2S.Speed;
        // PID_Calc(&Load_speed_pi);

        // // 电流 PI 控制器计算(id = 0)
        // Load_id_pi.ref = 0;
        // Load_id_pi.fdb = Load_idq.d;
        // PID_Calc(&Load_id_pi);
        // u_dq.d = Load_id_pi.output;

        // Load_iq_pi.ref = Load_speed_pi.output;
        // Load_iq_pi.fdb = Load_idq.q;
        // PID_Calc(&Load_iq_pi);
        // u_dq.q = Load_iq_pi.output;

        // // SVPWM
        // dq_2_abc(&u_abc, &u_dq, AD2S.Electrical_Angle);
        // e_svpwm(&u_abc, 101, &duty_abc);

        // Inverter
        if (system_enable == 0) {
            TIM8->CCR1 = 0;
            TIM8->CCR2 = 0;
            TIM8->CCR3 = 0;
        } else {
            TIM8->CCR1 = duty_abc.dutya * TIM8->ARR;
            TIM8->CCR2 = duty_abc.dutyb * TIM8->ARR;
            TIM8->CCR3 = duty_abc.dutyc * TIM8->ARR;
        }
    }
}

/**
 * @brief   ADC 注入组转换中断函数
 */
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    static int adc_cnt               = 0;
    static uint32_t adc_u_offset_sum = 0;
    static uint32_t adc_v_offset_sum = 0;
    UNUSED(hadc);
    if (hadc == &hadc1) {
        // Read Offset
        if (Load_M_Curr.sample_flag == CURR_SAMPLE_GET_OFFSET) {
            adc_u_offset_sum += hadc1.Instance->JDR1;
            adc_v_offset_sum += hadc1.Instance->JDR2;
            adc_cnt++;
            if (adc_cnt == 20) {
                adc_cnt                      = 0;
                Load_M_Curr.adc_val_u_offset = adc_u_offset_sum / 20.0f;
                Load_M_Curr.adc_val_v_offset = adc_v_offset_sum / 20.0f;
                Load_M_Curr.sample_flag      = CURR_SAMPLE_RUNNING;
                adc_u_offset_sum             = 0;
                adc_v_offset_sum             = 0;
            }
        }
        // Read Current
        else {
            Load_M_Curr.adc_val_u = hadc1.Instance->JDR1;
            Load_M_Curr.adc_val_v = hadc1.Instance->JDR2;
            adc_2_curr(&Load_M_Curr);
            Load_iabc.a = Load_M_Curr.curr_u;
            Load_iabc.b = Load_M_Curr.curr_v;
            Load_iabc.c = Load_M_Curr.curr_w;
            // software protection
            if ((Load_iabc.a * Load_iabc.a > 1 * 1) || (Load_iabc.b * Load_iabc.b > 1 * 1) || (Load_iabc.c * Load_iabc.c > 1 * 1)) {
                system_enable = 0;
            }
        }
    }
    // 角度和速度采样
    AD2S1210_Angle_Get();
    AD2S1210_Speed_Get();

    // 采样电流变换为 dq 轴电流
    abc_2_dq(&Load_idq, &Load_iabc, AD2S.Electrical_Angle);

    // 速度 PI 控制器计算
    Load_speed_pi.ref = load_speed_ref;
    Load_speed_pi.fdb = AD2S.Speed;
    PID_Calc(&Load_speed_pi);

    // 电流 PI 控制器计算(id = 0)
    Load_id_pi.ref = 0;
    Load_id_pi.fdb = Load_idq.d;
    PID_Calc(&Load_id_pi);
    u_dq.d = Load_id_pi.output;

    Load_iq_pi.ref = Load_speed_pi.output;
    Load_iq_pi.fdb = Load_idq.q;
    PID_Calc(&Load_iq_pi);
    u_dq.q = Load_iq_pi.output;

    // SVPWM
    dq_2_abc(&u_abc, &u_dq, AD2S.Electrical_Angle);
    e_svpwm(&u_abc, 101, &duty_abc);
}