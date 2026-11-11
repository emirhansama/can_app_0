
#include "can_app.h"
#include "can_task.h"
#include "usb_task.h"
#include "led_task.h"
#include "cmsis_os2.h"

static osThreadId_t can_task_handle;
static osThreadId_t usb_task_handle;
static osThreadId_t led_task_handle;

static const osThreadAttr_t can_task_attr = {
    .name = "can_task",
    .priority = osPriorityAboveNormal,
    .stack_size = 1024
};

static const osThreadAttr_t usb_task_attr = {
    .name = "usb_task",
    .priority = osPriorityNormal,
    .stack_size = 2048
};

static const osThreadAttr_t led_task_attr = {
    .name = "led_task",
    .priority = osPriorityBelowNormal,
    .stack_size = 1024
};

void StartDefaultTask(void *argument)
{
    can_task_handle = osThreadNew(can_task, NULL, &can_task_attr);
    usb_task_handle = osThreadNew(usb_task, NULL, &usb_task_attr);
    led_task_handle = osThreadNew(led_task, NULL, &led_task_attr);

    osThreadExit();
}
