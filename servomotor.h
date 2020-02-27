#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <stdint.h>

#ifdef SERVOMOTOR_USE_MAIN_HEADER
    #include <main.h>
#else
    #include <stm32f1xx_hal.h>
#endif

#ifndef KHZ
    #define KHZ 1000
#endif


#ifndef MHZ
    #define MHZ 1000000
#endif

typedef struct
{
    TIM_HandleTypeDef *htim;
    uint32_t channel;
    float offset;
    float min;
    float max;
}servo_t;

typedef enum {SERVO_STATUS_OK, SERVO_STATUS_ERROR } servo_status_t; 

servo_status_t servo_init(servo_t *handler, TIM_HandleTypeDef *htim, uint32_t channel);

void servo_set_limit(servo_t *handler, float min, float max);

void servo_set_offset(servo_t *handler, float angle_deg);

float servo_duty_ms(float angle_deg);

void servo_set_position(servo_t *handler, float angle_deg);

void servo_sweep(servo_t *handler, float start_angle_deg, float end_angle_deg, float step, uint32_t step_delay_ms);

#if 0
servo_status_t servo_psc_arr_calc(unsigned long apb_freq_hz, uint16_t *psc, uint16_t *arr);
servo_status_t servo_config_psc_arr(servo_t *handler, unsigned long apb_freq_hz);
#endif

#endif