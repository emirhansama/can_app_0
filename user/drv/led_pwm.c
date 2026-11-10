#include "led_pwm.h"
#include "tim.h"

static uint32_t led_pwm_compare(float brightness)
{
    if (brightness < 0.0f) {
        brightness = 0.0f;
    }

    if (brightness > 1.0f) {
        brightness = 1.0f;
    }

    return (uint32_t)(brightness * (__HAL_TIM_GET_AUTORELOAD(&htim4) + 1));
}

bool led_pwm_init(void)
{
    if (HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1) != HAL_OK) {
        return false;
    }

    if (HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2) != HAL_OK) {
        return false;
    }

    if (HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3) != HAL_OK) {
        return false;
    }

    return true;
}

void led_pwm_set(float x, float y, float z)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, led_pwm_compare(x));
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, led_pwm_compare(y));
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, led_pwm_compare(z));
}