#include "can_protocol.h"
#include "can_app_config.h"
#include <stdint.h>
#include <string.h>

static uint16_t can_protocol_read_u16(const uint8_t *data)
{
    return (uint16_t)data[0] | ((uint16_t)data[1] << 8);
}

bool can_protocol_unpack(uint16_t id, const uint8_t *data, can_protocol_data_t *result)
{
    can_protocol_data_t decoded = {0};
    decoded.id = id;

    switch(id){
        case CAN_ID_ACCEL_X:
        {
            memcpy(&decoded.x, data, sizeof(float));
        }
        break;

        case CAN_ID_ACCEL_Y:
        {
            memcpy(&decoded.y, data, sizeof(float));
        }
        break;

        case CAN_ID_ACCEL_Z:
        {
            memcpy(&decoded.z, data, sizeof(float));
        }
        break;

        case CAN_ID_DISTANCE:
        {
            memcpy(&decoded.distance, data, sizeof(float));
        }
        break;

        case CAN_ID_ALL:
        {
            decoded.x = (int16_t)can_protocol_read_u16(&data[0]) /1000.f;
            decoded.y = (int16_t)can_protocol_read_u16(&data[2]) /1000.f;
            decoded.z = (int16_t)can_protocol_read_u16(&data[4]) /10.f;
            decoded.distance = (int16_t)can_protocol_read_u16(&data[6]) /1000.f;
        }
        break;

        default:
        {
            return false;
        }
    }

    *result = decoded;

    return true;
    }