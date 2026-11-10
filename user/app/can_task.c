
#include "can_task.h"
#include "can_comm.h"
#include "can_protocol.h"
#include "sensor_data.h"
#include "cmsis_os2.h"
#include "main.h"

#define CAN_RX_QUEUE_LENGTH     (8)

static osMessageQueueId_t can_rx_queue;

void can_task(void *argument)
{
    uint16_t request_id;
    sensor_data_t sensor;
    uint8_t data[8];
    uint8_t dlc;

    can_rx_queue = osMessageQueueNew(CAN_RX_QUEUE_LENGTH, sizeof(uint16_t), NULL);

    if (!can_comm_init()) {
        Error_Handler();
    }

    for (;;) {
        if (osMessageQueueGet(can_rx_queue, &request_id, NULL, osWaitForever) == osOK) {
            if (sensor_data_get(&sensor) && can_protocol_pack(request_id, &sensor, data, &dlc)) {
                can_comm_send(request_id, data, dlc);
            }
        }
    }
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    (void)hcan;

    CAN_RxHeaderTypeDef header;
    uint8_t data[8];

    can_comm_receive(&header, data);

    uint16_t request_id = (uint16_t)header.StdId;
    osMessageQueuePut(can_rx_queue, &request_id, 0, 0);
}

