#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    uint16_t id;
    float x;
    float y;
    float z;
    float distance;
} can_protocol_data_t;

bool can_protocol_unpack(uint16_t id, const uint8_t data[8], can_protocol_data_t *result);

#endif