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
}servo_t;

typedef enum {SERVO_STATUS_OK, SERVO_STATUS_ERROR } servo_status_t; 

servo_status_t servo_init(servo_t *handler, TIM_HandleTypeDef *htim, uint32_t channel);

servo_status_t servo_psc_arr_calc(unsigned long apb_freq_hz, uint16_t *psc, uint16_t *arr);

void servo_set_offset(servo_t *handler, float angle_deg);

void servo_set_position(servo_t *handler, float angle_deg);

#endif