
#include "distance_task.h"
#include "hcsr04.h"
#include "sensor_data.h"
#include "can_app_config.h"
#include "cmsis_os2.h"

void distance_task(void *argument)
{
    float distance;
    uint32_t next_tick = osKernelGetTickCount();

    if (!hcsr04_init()) {
        Error_Handler();
    }

    for (;;) {
        hcsr04_trigger();

        uint32_t start_tick = osKernelGetTickCount();

        while (osKernelGetTickCount() - start_tick < HCSR04_TIMEOUT_MS) {
            if (hcsr04_get_distance(&distance)) {
                sensor_data_set_distance(distance);
                break;
            }

            osDelay(1);
        }

        next_tick += DIST_TASK_PERIOD_MS;
        osDelayUntil(next_tick);
    }
}
