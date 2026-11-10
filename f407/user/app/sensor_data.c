#include "sensor_data.h"
#include "cmsis_os2.h"

static sensor_data_t sensor_data = {0};
static osMutexId_t sensor_mutex = 0;

bool sensor_data_init(void)
{
    sensor_mutex = osMutexNew(NULL);

    return sensor_mutex != NULL;
}

bool sensor_data_set_accel(float x, float y, float z)
{
    if(osMutexAcquire(sensor_mutex, osWaitForever) != osOK) {
        return false;
    }

    sensor_data.x = x;
    sensor_data.y = y;
    sensor_data.z = z;

    osMutexRelease(sensor_mutex);

    return true;
}

bool sensor_data_set_distance(float distance)
{
    if (osMutexAcquire(sensor_mutex, osWaitForever) != osOK) {
        return false;
    }

    sensor_data.distance = distance;

    osMutexRelease(sensor_mutex);

    return true;
}

bool sensor_data_get(sensor_data_t *data)
{
    if (data == NULL) {
        return false;
    }

    if (osMutexAcquire(sensor_mutex, osWaitForever) != osOK) {
        return false;
    }

    *data = sensor_data;

    osMutexRelease(sensor_mutex);

    return true;
}