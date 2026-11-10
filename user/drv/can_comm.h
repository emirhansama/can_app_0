
#ifndef CAN_COMM_H
#define CAN_COMM_H

#include "can.h"
#include <stdbool.h>

bool can_comm_init(void);
HAL_StatusTypeDef can_comm_send(uint16_t id, const uint8_t data[8], uint8_t dlc);
HAL_StatusTypeDef can_comm_receive(CAN_RxHeaderTypeDef *header, uint8_t data[8]);

#endif
