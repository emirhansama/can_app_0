
#include "accel_task.h"
#include "lis3dsh.h"
#include "sensor_data.h"
#include "can_app_config.h"
#include "cmsis_os2.h"

void accel_task(void *argument)
{
    (void)argument;

    float x, y, z;
    float fx = 0, fy = 0, fz = 0;
    bool initialized = false;

    if (!lis3dsh_check_id() || !lis3dsh_init()) {
        Error_Handler();
    }

    for (;;) {
        if (lis3dsh_read_xyz(&x, &y, &z) == HAL_OK) {
            if (!initialized) {
                fx = x;
                fy = y;
                fz = z;
                initialized = true;
            } else {
                fx += ACCEL_LPF_ALPHA * (x - fx);
                fy += ACCEL_LPF_ALPHA * (y - fy);
                fz += ACCEL_LPF_ALPHA * (z - fz);
            }

            sensor_data_set_accel(fx, fy, fz);
        }

        osDelay(ACCEL_TASK_PERIOD_MS);
    }
}
