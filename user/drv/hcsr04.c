
#include "hcsr04.h"
#include "tim.h"

typedef enum {
    HCSR04_IDLE,
    HCSR04_WAIT_RISING,
    HCSR04_WAIT_FALLING,
    HCSR04_READY
} hcsr04_state_t;

static volatile hcsr04_state_t state = HCSR04_IDLE;
static uint16_t first = 0;
static volatile uint16_t pulse_us = 0;

bool hcsr04_init(void)
{
    EXTI->FTSR &= ~EXTI_FTSR_TR1;
    EXTI->RTSR |= EXTI_RTSR_TR1;

    return HAL_TIM_Base_Start(&htim3) == HAL_OK;
}

void hcsr04_trigger(void)
{
    state = HCSR04_WAIT_RISING;

    HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_SET);

    uint16_t tick = (uint16_t)__HAL_TIM_GET_COUNTER(&htim3);

    while ((uint16_t)(__HAL_TIM_GET_COUNTER(&htim3) - tick) < HCSR04_TRIGGER_US) {
    }

    HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_RESET);
}

bool hcsr04_get_distance(float *distance)
{
    if (distance == NULL || state != HCSR04_READY) {
        return false;
    }

    *distance = pulse_us * HCSR04_CM_PER_US;

    state = HCSR04_IDLE;

    return true;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin != HCSR04_ECHO_Pin) {
        return;
    }

    if (state == HCSR04_WAIT_RISING) {
        first = (uint16_t)__HAL_TIM_GET_COUNTER(&htim3);
        state = HCSR04_WAIT_FALLING;

        EXTI->RTSR &= ~EXTI_RTSR_TR1;
        EXTI->FTSR |= EXTI_FTSR_TR1;
    }

    else if (state == HCSR04_WAIT_FALLING) {
        pulse_us = (uint16_t)(__HAL_TIM_GET_COUNTER(&htim3) - first);
        state = HCSR04_READY;

        EXTI->FTSR &= ~EXTI_FTSR_TR1;
        EXTI->RTSR |= EXTI_RTSR_TR1;
    }
}
