# STM32 Servomotor library
⚙️ Servomotor control library for STM32 microcontrollers with CubeMX generated HAL

![Version](https://img.shields.io/badge/Version-v1.0.0-green)
![License](https://img.shields.io/badge/License-MIT-blue)

# Documentation
The functions are declared and documented in the **servomotor.h** file.

# Timer configuration
The timer that will be used for PWM generation needs to be previously configured. This library will not do this for you.

It is recommended to use **STM32CubeMX** to configure the timers settings.

When doing this, you must define in the timers a PWM generation channel for each servomotor.

You should check the frequency of the APB clock used by the timer. To find out which APB clock is used by the timer you have chosen, see your microcontroller datasheet.

Use the **timercalc.py** python script to calculate the timer's Prescaler (PSC) and Counter Period (ARR) values.

Wrong settings will cause servomotors to not work.

# HAL headers

if necessary, change *#include<stm32f1xx_hal.h>* in the **servomotor.h** file to the HAL header of your microcontroller.