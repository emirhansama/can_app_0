#include "can_app_config.h"
#include "cmsis_os2.h"
#include "led_task.h"
#include "main.h"

void led_task(void *argument)
{
    for(;;){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
        osDelay(LED_BLINK_PERIOD_MS);
    }
}