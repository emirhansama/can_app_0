
#include "hcsr04.h"
#include "tim.h"

typedef enum {
    HCSR04_IDLE,
    HCSR04_WAIT_RISING,
    HCSR04_WAIT_FALLING,
    HCSR04_READY
} hcsr04_state_t;

static volatile hcsr04_state_t state = HCSR04_IDLE;
static volatile uint16_t start_tick = 0;
static volatile uint16_t pulse_us = 0;

static void hcsr04_set_edge(GPIO_PinState pin_state)
{
    if (pin_state == GPIO_PIN_SET) {
        EXTI->RTSR &= ~HCSR04_ECHO_Pin;
        EXTI->FTSR |= HCSR04_ECHO_Pin;
    } else {
        EXTI->FTSR &= ~HCSR04_ECHO_Pin;
        EXTI->RTSR |= HCSR04_ECHO_Pin;
    }
}

bool hcsr04_init(void)
{
    if (HAL_TIM_Base_Start(&htim3) != HAL_OK) {
        return false;
    }

    HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_RESET);

    hcsr04_set_edge(HAL_GPIO_ReadPin(HCSR04_ECHO_GPIO_Port, HCSR04_ECHO_Pin));

    return true;
}

bool hcsr04_trigger(void)
{
    if (state == HCSR04_WAIT_RISING || state == HCSR04_WAIT_FALLING) {
        return false;
    }

    if (HAL_GPIO_ReadPin(HCSR04_ECHO_GPIO_Port, HCSR04_ECHO_Pin) != GPIO_PIN_RESET) {
        return false;
    }

    state = HCSR04_WAIT_RISING;
    hcsr04_set_edge(GPIO_PIN_RESET);

    HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_SET);

    uint16_t tick = (uint16_t)__HAL_TIM_GET_COUNTER(&htim3);

    while ((uint16_t)(__HAL_TIM_GET_COUNTER(&htim3) - tick) < HCSR04_TRIGGER_US) {
    }

    HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_RESET);

    return true;
}

bool hcsr04_get_distance(float *distance)
{
    if (distance == NULL || state != HCSR04_READY) {
        return false;
    }

    uint16_t duration = pulse_us;

    state = HCSR04_IDLE;

    *distance = duration * HCSR04_CM_PER_US;

    return true;
}

void hcsr04_cancel(void)
{
    state = HCSR04_IDLE;

    HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_RESET);

    hcsr04_set_edge(HAL_GPIO_ReadPin(HCSR04_ECHO_GPIO_Port, HCSR04_ECHO_Pin));
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin != HCSR04_ECHO_Pin) {
        return;
    }

    GPIO_PinState pin_state = HAL_GPIO_ReadPin(HCSR04_ECHO_GPIO_Port, HCSR04_ECHO_Pin);
    uint16_t tick = (uint16_t)__HAL_TIM_GET_COUNTER(&htim3);

    if (pin_state == GPIO_PIN_SET) {

        if (state == HCSR04_WAIT_RISING) {
            start_tick = tick;
            state = HCSR04_WAIT_FALLING;
        }

    } else {

        if (state == HCSR04_WAIT_FALLING) {
            pulse_us = (uint16_t)(tick - start_tick);
            state = HCSR04_READY;
        }
    }

    hcsr04_set_edge(pin_state);
}
