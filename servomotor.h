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
 * @file servomotor.h
 * @author Filipe Chagas
 * @date 27 / 02 / 2020
 * @brief Servomotor library for STM32 with CubeMX generated HAL
 *
 * @see https://github.com/FilipeChagasDev/stm32-servomotor
 */

#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <stdint.h>

/* --------------------------------------------------------------- */
/* IMPORTANT: Paste these includes into your project's main.h file */
#if 0
    /* For stm32f0xx */
    #include <stm32f0xx_hal.h>
    #include <stm32f0xx_hal_tim.h>

   
    /* For stm32f1xx */
    #include <stm32f1xx_hal.h>
    #include <stm32f1xx_hal_tim.h>

    
    /* For stm32f2xx */
    #include <stm32f2xx_hal.h>
    #include <stm32f2xx_hal_tim.h>

    
    /* For stm32f3xx */
    #include <stm32f3xx_hal.h>
    #include <stm32f3xx_hal_tim.h>

    
    /* For stm32f4xx */
    #include <stm32f4xx_hal.h>
    #include <stm32f4xx_hal_tim.h>

/* --------------------------------------------------------------- */
#else
    #include <main.h>
#endif

#ifndef KHZ
    #define KHZ 1000
#endif


#ifndef MHZ
    #define MHZ 1000000
#endif


/**
 * @brief Struct typedef for servomotor handler. Each servomotor must have it's own handler struct.
 */
typedef struct
{
    TIM_HandleTypeDef *htim;
    uint32_t channel;
    float offset;
    float min;
    float max;
}servo_t;

typedef enum {SERVO_STATUS_OK, SERVO_STATUS_ERROR } servo_status_t; 

/**
 * @brief Sets the initial values ​​for the servo_t handling struct and starts PWM.
 * @param handler Pointer to the servo_t handling struct.
 * @param htim Pointer to the TIM_HandleTypeDef handling struct of the timer that will be used to generate PWM signals for the servomotors.
 * @param channel Channel of the timer where the servomotor output is connected.
 * @return SERVO_STATUS_OK or SERVO_STATUS_ERROR.
 */
servo_status_t servo_init(servo_t *handler, TIM_HandleTypeDef *htim, uint32_t channel);

/**
 * @brief Sets minimum and maximum angles (without offset) to the servomotor position (default is min=0 max=170).
 * @param handler Pointer to the servo_t handling struct.
 * @param min Minimum angle in degrees.
 * @param max Maximum angle in degrees.
 */ 
void servo_set_limit(servo_t *handler, float min, float max);

/**
 * @brief Sets the offset angle. The offset angle will be used as the relative zero position of the servomotor.
 * @param handler Pointer to the servo_t handling struct.
 * @param angle_deg Offset angle in degrees.
 */
void servo_set_offset(servo_t *handler, float angle_deg);

/**
 * @brief Calculate the duty cicle period of the PWM signal for a angle
 * @param angle_deg Angle in degrees.
 * @return Duty cicle period in ms.
 */
float servo_duty_ms(float angle_deg);

/**
 * @brief Puts the axis of the servomotor in the specified position.
 * @param handler Pointer to the servo_t handling struct.
 * @param angle_deg Angle of the servomotor axis in degrees (with offset as relative zero)
 */
void servo_set_position(servo_t *handler, float angle_deg);

/**
 * @brief Makes a sweeping motion to test the servomotor.
 * @param handler Pointer to the servo_t handling struct.
 * @param start_angle_deg Starting angle for the motion in degrees (must be less than end_angle_deg).
 * @param end_angle_deg Ending angle for the motion in degrees (must be greater than start_angle_deg).
 * @param step Value that will be incremented or decremented of the position angle for each motion step (must be greater thar 0).
 * @param step_delay_ms Delay between the steps (in ms).
 */
void servo_sweep(servo_t *handler, float start_angle_deg, float end_angle_deg, float step, uint32_t step_delay_ms);

#endif
