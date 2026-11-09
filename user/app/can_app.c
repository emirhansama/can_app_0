
#include "can_app.h"
#include "lis3dsh.h"
#include "hcsr04.h"
#include "cmsis_os2.h"
#include "main.h"

volatile float debug_x = 0;
volatile float debug_y = 0;
volatile float debug_z = 0;
volatile float debug_distance = 0;

volatile bool debug_accel_ok = false;
volatile bool debug_hcsr04_ok = false;
volatile bool debug_distance_ok = false;

void StartDefaultTask(void *argument)
{
    (void)argument;

    debug_accel_ok = lis3dsh_check_id();

    if (debug_accel_ok) {
        debug_accel_ok = lis3dsh_init();
    }

    debug_hcsr04_ok = hcsr04_init();

    for (;;) {

        if (debug_accel_ok) {
            float x, y, z;

            if (lis3dsh_read_xyz(&x, &y, &z) == HAL_OK) {
                debug_x = x;
                debug_y = y;
                debug_z = z;
            }
        }

        if (debug_hcsr04_ok && hcsr04_trigger()) {

            uint32_t start = HAL_GetTick();
            debug_distance_ok = false;

            while (HAL_GetTick() - start < 40) {
                float distance;

                if (hcsr04_get_distance(&distance)) {
                    debug_distance = distance;
                    debug_distance_ok = true;
                    break;
                }

                osDelay(1);
            }

            if (!debug_distance_ok) {
                hcsr04_cancel();
            }
        }

        osDelay(100);
    }
}
