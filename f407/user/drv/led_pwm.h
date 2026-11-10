#ifndef LED_PWM_H
#define LED_PWM_H

#include "main.h"
#include <stdbool.h>

bool led_pwm_init(void);
void led_pwm_set(float x, float y, float z);

#endif