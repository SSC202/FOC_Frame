#ifndef __SVPWM_H
#define __SVPWM_H

#include "math.h"
#include "my_math.h"

#include "usermain.h"

typedef struct
{
    float dutya;
    float dutyb;
    float dutyc;
} duty_abc_t;

void e_svpwm(abc_t *_u_ref, float Udc, duty_abc_t *_duty_abc);

#endif // !_SVPWM_H__
