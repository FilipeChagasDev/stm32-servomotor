#include "servomotor.h"

servo_status_t servo_init(servo_t *handler, TIM_HandleTypeDef *htim, uint32_t channel)
{
    handler->htim = htim;
    handler->channel = channel;
    handler->offset = 0;

    switch (channel)
    {
    case TIM_CHANNEL_1:
        handler->htim->Instance->CCR1 = 0;
        break;
    case TIM_CHANNEL_2:
        handler->htim->Instance->CCR2 = 0;
        break;
    case TIM_CHANNEL_3:
        handler->htim->Instance->CCR3 = 0;
        break;
    case TIM_CHANNEL_4:
        handler->htim->Instance->CCR4 = 0;
        break;
    default:
        handler->htim->Instance->CCR1 = 0;
        handler->htim->Instance->CCR2 = 0;
        handler->htim->Instance->CCR3 = 0;
        handler->htim->Instance->CCR4 = 0;
    }

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

servo_status_t servo_config_psc_arr(servo_t *handler, unsigned long apb_freq_hz)
{
    uint16_t psc, arr;
    if(servo_psc_arr_calc(apb_freq_hz, &psc, &arr) == SERVO_STATUS_ERROR) return SERVO_STATUS_ERROR;
    handler->htim->Instance->PSC = psc;
    handler->htim->Instance->ARR = psc;
    return SERVO_STATUS_OK;
}

void servo_set_offset(servo_t *handler, float angle_deg)
{
    handler->offset = angle_deg;
}

float servo_duty_ms(float angle_deg)
{
    /*  angle_deg per-one of 180deg:
               180/angle_deg = 1/y
            -> angle_deg/180 = y

        duty_ms por_one of 1ms:
               1/duty_ms = 1/z
            -> duty_ms = z

        duty_ms from angle_deg:
               angle_deg/180 = z
            -> angle_deg/180 = duty_ms
    */

   return 1.0 + angle_deg / 180.0;
}

void servo_set_position(servo_t *handler, float angle_deg)
{
    /*  duty_ms per-one of 20ms:
               20/duty_ms = 1/x
            -> x = duty_ms/20

        duty_16 per-one of max_duty_16:
               max_duty_16/duty_16 = 1/y
            -> duty_16/max_duty_16 = y

        calc duty_16 from duty_ms:
               duty_16/max_duty_16 = x
            -> duty_16 = x * max_duty_16
            -> duty_16 = (duty_ms/20) * max_duty_16
            -> duty_16 = (duty_ms * max_duty_16) / 20
    */
    angle_deg += handler->offset;

    float max_duty_16 = handler->htim->Instance->ARR;
    float duty_ms = servo_duty_ms(angle_deg);
    float duty_16 = (duty_ms * max_duty_16)/ 20.0;

    uint16_t ccr = ( (int32_t)duty_16 > 0xFFFF ? 
                    0xFFFF
                    :  ( (int32_t)duty_16 < 0 ?
                        0 
                        :  (uint16_t)duty_16
                       ) 
                   );

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
    default:
        handler->htim->Instance->CCR1 = ccr;
        handler->htim->Instance->CCR2 = ccr;
        handler->htim->Instance->CCR3 = ccr;
        handler->htim->Instance->CCR4 = ccr;
    }
}