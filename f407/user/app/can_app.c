
#include "can_app.h"
#include "accel_task.h"
#include "distance_task.h"
#include "led_task.h"
#include "can_task.h"
#include "sensor_data.h"
#include "cmsis_os2.h"
#include "main.h"

static const osThreadAttr_t accel_task_attr = {
    .name = "accel_task",
    .priority = osPriorityNormal,
    .stack_size = 1024
};

static const osThreadAttr_t distance_task_attr = {
    .name = "distance_task",
    .priority = osPriorityNormal,
    .stack_size = 1024
};

static const osThreadAttr_t led_task_attr = {
    .name = "led_task",
    .priority = osPriorityBelowNormal,
    .stack_size = 1024
};

static const osThreadAttr_t can_task_attr = {
    .name = "can_task",
    .priority = osPriorityAboveNormal,
    .stack_size = 1024
};

void StartDefaultTask(void *argument)
{
    (void)argument;

    if (!sensor_data_init()) {
        Error_Handler();
    }

    if (osThreadNew(accel_task, NULL, &accel_task_attr) == NULL) {
        Error_Handler();
    }

    if (osThreadNew(distance_task, NULL, &distance_task_attr) == NULL) {
        Error_Handler();
    }

    if (osThreadNew(led_task, NULL, &led_task_attr) == NULL) {
        Error_Handler();
    }

    if (osThreadNew(can_task, NULL, &can_task_attr) == NULL) {
        Error_Handler();
    }

    osThreadExit();
}
