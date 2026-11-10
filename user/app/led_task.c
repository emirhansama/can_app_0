
#include "led_task.h"
#include "led_pwm.h"
#include "sensor_data.h"
#include "can_app_config.h"
#include "cmsis_os2.h"
#include <math.h>

static void led_blink_callback(void *argument)
{
    (void)argument;

    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
}

void led_task(void *argument)
{
    (void)argument;

    sensor_data_t data;
    uint32_t blink_period = BLINK_SLOW_MS;

    if (!led_pwm_init()) {
        Error_Handler();
    }

    osTimerId_t blink_timer = osTimerNew(led_blink_callback, osTimerPeriodic, NULL, NULL);

    if (blink_timer == NULL) {
        Error_Handler();
    }

    if (osTimerStart(blink_timer, blink_period) != osOK) {
        Error_Handler();
    }

    for (;;) {
        if (sensor_data_get(&data)) {
            float x = fabsf(data.x) / LED_MAX_ACCEL_G;
            float y = fabsf(data.y) / LED_MAX_ACCEL_G;
            float z = fabsf(data.z) / LED_MAX_ACCEL_G;

            led_pwm_set(x, y, z);

            uint32_t new_period = BLINK_SLOW_MS;

            if (data.distance > 0.0f && data.distance < DIST_NEAR_CM) {
                new_period = BLINK_FAST_MS;
            } else if (data.distance > 0.0f && data.distance < DIST_FAR_CM) {
                new_period = BLINK_MEDIUM_MS;
            }

            if (new_period != blink_period) {
                if (osTimerStart(blink_timer, new_period) == osOK) {
                    blink_period = new_period;
                }
            }
        }

        osDelay(LED_TASK_PERIOD_MS);
    }
}
