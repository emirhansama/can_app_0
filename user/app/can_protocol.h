#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include "sensor_data.h"
#include <stdint.h>
#include <stdbool.h>

bool can_protocol_pack(uint16_t id, const sensor_data_t *sensor, uint8_t data[8], uint8_t *dlc);
#endif