#include "servomotor.h"

servo_status_t servo_init(servo_t *handler, TIM_HandleTypeDef *htim, uint32_t channel)
{
    handler->htim = htim;
    handler->channel = channel;
    if(HAL_TIM_PWM_Start(htim, channel) == HAL_OK) return SERVO_STATUS_OK;
    else return SERVO_STATUS_ERROR;
}

servo_status_t servo_psc_arr_calc(unsigned long apb_freq_hz, uint16_t *psc, uint16_t *arr)
{
    /*  Rule 1: (apb_freq_hz / 50hz) most be equal to ( PSC * ARR ) 
                to generate the 20ms pwm period.

        Rule 2: ARR value most be higher as possible and PSC value most be lower as possible.
    */

    unsigned long psc_arr = apb_freq_hz / 50;

    uint16_t psc_tmp;
    uint16_t arr_tmp;
    
    for(arr_tmp = 0xFFFF; arr_tmp > 0; arr_tmp--)
    {
        for(psc_tmp = 1; psc_tmp <=  0xFFFF; psc_tmp++)
        {
            if(psc_tmp * arr_tmp == psc_arr)
            {
                *psc = psc_tmp;
                *arr = arr_tmp;
                return SERVO_STATUS_OK;
            }
        }
    }

    return SERVO_STATUS_ERROR;
}

void servo_set_offset(servo_t *handler, float angle_deg)
{
    handler->offset = angle_deg;
}

void servo_set_position(servo_t *handler, float angle_deg)
{
    /*  angle_duty per-one of max_duty:
               max_duty/andle_duty = 1/x
            -> andle_duty/max_duty = x

        angle_deg per-one of 180deg:
               180/angle_deg = 1/y
            -> angle_deg/180 = y

        calculating angle_duty where y=x:
               andle_duty/max_duty = y
            -> angle_duty = y * max_duty
            -> angle_duty = (angle_deg/180)*max_duty
            -> angle_duty = (angle_deg*max_duty)/180
    */
    angle_deg += handler->offset;

    float max_duty = handler->htim->Instance->ARR;
    float angle_duty = (angle_deg * max_duty) / 180.0;
    uint16_t ccr = ((unsigned long)angle_duty) < 0xFFFF ? (uint16_t)angle_duty : 0xFFFF;

    switch (handler->channel)
    {
    case TIM_CHANNEL_1:
        handler->htim->Instance->CCR1 = ccr;
        break;
    case TIM_CHANNEL_2:
        handler->htim->Instance->CCR2 = ccr;
        break;
    case TIM_CHANNEL_3:
        handler->htim->Instance->CCR3 = ccr;
        break;
    case TIM_CHANNEL_4:
        handler->htim->Instance->CCR4 = ccr;
        break;
    }
    
}