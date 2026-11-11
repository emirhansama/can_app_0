
#include "can_task.h"
#include "can_comm.h"
#include "can_app_config.h"
#include "cmsis_os2.h"
#include "main.h"

static osMessageQueueId_t can_request_queue;
static osMessageQueueId_t can_response_queue;

bool can_task_send_request(uint16_t id)
{
    return osMessageQueuePut(can_request_queue, &id, 0, 0) == osOK;
}

bool can_task_get_response(can_response_t *response, uint32_t timeout)
{
    return osMessageQueueGet(can_response_queue, response, NULL, timeout) == osOK;
}

void can_task(void *argument)
{
    (void)argument;

    uint16_t request_id;
    uint8_t dlc;

    can_request_queue = osMessageQueueNew(CAN_TX_QUEUE_LENGTH, sizeof(uint16_t), NULL);
    can_response_queue = osMessageQueueNew(CAN_RX_QUEUE_LENGTH, sizeof(can_response_t), NULL);

    if (!can_comm_init()) {
        Error_Handler();
    }

    for (;;) {
        if (osMessageQueueGet(can_request_queue, &request_id, NULL, osWaitForever) == osOK) {
            if (request_id == CAN_ID_ALL) {
                dlc = CAN_DLC_ALL;
            } else {
                dlc = CAN_DLC_FLOAT;
            }

            can_comm_send_request(request_id, dlc);
        }
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    (void)hcan;

    CAN_RxHeaderTypeDef header;
    can_response_t response;

    can_comm_receive(&header, response.data);

    response.id = (uint16_t)header.StdId;

    osMessageQueuePut(can_response_queue, &response, 0, 0);
}
