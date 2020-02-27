# STM32 servomotor library
Servomotor control library for STM32 microcontrollers with CubeMX generated HAL

![Version](https://img.shields.io/badge/Version-v1.1.0-green)
![License](https://img.shields.io/badge/License-MIT-blue)

## IMPORTANT

The library's dependency includes need to be included in your project's **main.h** file. Are they:

* The ...**_hal.h** header
* The ...**_hal_tim.h** header (for timers)

```C
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

```

## Documentation
The functions are declared and documented in the **servomotor.h** file.

## Timer configuration
The timer that will be used for PWM generation needs to be previously configured. This library will not do this for you.

It is recommended to use **STM32CubeMX** to configure the timers settings.

### Timer channels
When doing this, you must define in the timers a PWM generation channel for each servomotor.

### APB timer clocks
You should check the frequency of the **APB** clock used by the timer. To find out which **APB** clock is used by the timer you have chosen, see your microcontroller datasheet.

### Prescaler (PSC) & Counter Period (ARR)
Use the **servocalc.py** python script to calculate the timer's **Prescaler (PSC)** and **Counter Period (ARR)** values.

### Other setting
Keep other settings as default

**Wrong settings will cause servomotors to not work.**

