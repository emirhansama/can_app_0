
#include "usb_task.h"
#include "usb_comm.h"
#include "can_task.h"
#include "can_protocol.h"
#include "can_app_config.h"
#include "cmsis_os2.h"
#include "main.h"
#include <stdio.h>

void usb_task(void *argument)
{
    uint8_t ch;
    bool continuous = false;
    uint32_t next_tick = 0;

    can_response_t response;
    can_protocol_data_t result;
    char message[USB_TX_BUFFER_SIZE];

    if (!usb_comm_init()) {
        Error_Handler();
    }

    for (;;) {
        if (usb_comm_get_char(&ch, 10)) {
            switch (ch) {
                case '0':
                    usb_comm_send("0:Help 1:X 2:Y 3:Z 4:Distance 5:All 6:Continuous\r\n");
                    break;

                case '1':
                    can_task_send_request(CAN_ID_ACCEL_X);
                    break;

                case '2':
                    can_task_send_request(CAN_ID_ACCEL_Y);
                    break;

                case '3':
                    can_task_send_request(CAN_ID_ACCEL_Z);
                    break;

                case '4':
                    can_task_send_request(CAN_ID_DISTANCE);
                    break;

                case '5':
                    can_task_send_request(CAN_ID_ALL);
                    break;

                case '6':
                    continuous = !continuous;
                    next_tick = osKernelGetTickCount();
                    usb_comm_send(continuous ? "Continuous ON\r\n" : "Continuous OFF\r\n");
                    break;

                default:
                    break;
            }
        } else if (can_task_get_response(&response, 0)) {
            if (can_protocol_unpack(response.id, response.data, &result)) {
                switch (result.id) {
                    case CAN_ID_ACCEL_X:
                        snprintf(message, sizeof(message), "X: %.3f g\r\n", result.x);
                        break;

                    case CAN_ID_ACCEL_Y:
                        snprintf(message, sizeof(message), "Y: %.3f g\r\n", result.y);
                        break;

                    case CAN_ID_ACCEL_Z:
                        snprintf(message, sizeof(message), "Z: %.3f g\r\n", result.z);
                        break;

                    case CAN_ID_DISTANCE:
                        snprintf(message, sizeof(message), "Distance: %.2f cm\r\n", result.distance);
                        break;

                    case CAN_ID_ALL:
                        snprintf(message, sizeof(message), "X: %.3f g | Y: %.3f g | Z: %.3f g | D: %.2f cm\r\n",
                                 result.x, result.y, result.z, result.distance);
                        break;

                    default:
                        break;
                }

                usb_comm_send(message);
            }
        }

        if (continuous && (int32_t)(osKernelGetTickCount() - next_tick) >= 0) {
            can_task_send_request(CAN_ID_ALL);
            next_tick = osKernelGetTickCount() + CAN_CONTINUOUS_PERIOD_MS;
        }
    }
}
