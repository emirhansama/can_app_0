#ifndef CAN_TASK_H
#define CAN_TASK_H

#include "can_protocol.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint16_t id;
    uint8_t data[8];
} can_response_t;

void can_task(void *argument);
bool can_task_send_request(uint16_t id);
bool can_task_get_response(can_response_t *result, uint32_t timeout);

#endif