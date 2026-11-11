#ifndef CAN_COMM_H
#define CAN_COMM_H

#include "can.h"
#include "stm32f7xx_hal_can.h"
#include "stm32f7xx_hal_def.h"
#include <stdbool.h>
#include <stdint.h>

bool can_comm_init(void);
HAL_StatusTypeDef can_comm_send_request(uint16_t id, uint8_t dlc);
HAL_StatusTypeDef can_comm_receive(CAN_RxHeaderTypeDef *header, uint8_t data[8]);

#endif