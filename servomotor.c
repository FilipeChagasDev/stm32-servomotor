/*
 * MIT License
 * 
 * Copyright (c) 2020 Filipe Chagas
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file servomotor.c
 * @author Filipe Chagas
 * @date 27 / 02 / 2020
 * @brief Servomotor library for STM32 with CubeMX generated HAL
 *
 * @see https://github.com/FilipeChagasDev/stm32-servomotor
 */

#include "servomotor.h"

servo_status_t servo_init(servo_t *handler, TIM_HandleTypeDef *htim, uint32_t channel)
{
    if(handler == NULL) return SERVO_STATUS_ERROR;

    handler->htim = htim;
    handler->channel = channel;
    handler->offset = 0;
    handler->min = 0;
    handler->max = 170;

    if(HAL_TIM_PWM_Start(htim, channel) != HAL_OK) return SERVO_STATUS_ERROR;
    servo_set_position(handler, 0);

    return SERVO_STATUS_OK;
}

void servo_set_limit(servo_t *handler, float min, float max)
{
    if(handler == NULL) return;

    handler->min = min;
    handler->max = max;
}

void servo_set_offset(servo_t *handler, float angle_deg)
{
    if(handler == NULL) return;

    handler->offset = angle_deg;
}

float servo_duty_ms(float angle_deg)
{
    /*  
       180            2ms
    --------   =   ----------
    angle_deg       duty_ms

                angle_deg
    duty_ms =   --------- 
                   90

    0.5ms -> 0deg
    2.5ms -> 180deg
    */

    return 0.5 + angle_deg / 90.0;
}

void servo_set_position(servo_t *handler, float angle_deg)
{
    /*
    duty_ms      duty_16
    ------- =  ----------- 
      20ms     max_duty_16

                             duty_ms
    duty_16 = max_duty_16 * ----------
                              20ms 
    */

    if(handler == NULL) return;

    angle_deg += handler->offset;
    angle_deg = ( angle_deg > handler->max ? handler->max : 
                    ( angle_deg < handler->min ? handler->min : angle_deg )
                );

    float max_duty_16 = handler->htim->Instance->ARR;
    float duty_ms = servo_duty_ms(angle_deg);
    float duty_16 = (duty_ms * max_duty_16)/ 20.0;

    uint16_t ccr = ( (int32_t)duty_16 > 0xFFFF ? 0xFFFF :  
                     ( (int32_t)duty_16 < 0 ? 0 : (uint16_t)duty_16 ) 
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

void servo_sweep(servo_t *handler, float start_angle_deg, float end_angle_deg, float step, uint32_t step_delay_ms)
{
    if(handler == NULL || start_angle_deg >= end_angle_deg || step < 1) return;

    float i = start_angle_deg;

    while (i < end_angle_deg)
    {
        servo_set_position(handler, i);
        i += step;
        i = (i > end_angle_deg ? end_angle_deg : i);
        if(step_delay_ms > 0) HAL_Delay(step_delay_ms);
    }

    while (i > start_angle_deg)
    {
        servo_set_position(handler, i);
        i -= step;
        i = (i < start_angle_deg ? start_angle_deg : i);
        if(step_delay_ms > 0) HAL_Delay(step_delay_ms);
    }
    
}