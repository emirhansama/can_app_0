#include "can_protocol.h"
#include "can_app_config.h"
#include <string.h>

static void can_protocol_write_u16(uint8_t *data, uint16_t value)
{
    data[0] = (uint8_t)(value & 0xFF);
    data[1] = (uint8_t)(value >> 8);
}

bool can_protocol_pack(uint16_t id, const sensor_data_t *sensor, uint8_t data[8], uint8_t *dlc)
{
    float value;

    switch(id) {
        case CAN_ID_ACCEL_X:
        {
            value = sensor-> x;
        }
        break;

        case CAN_ID_ACCEL_Y:
        {
            value = sensor-> y;
        }
        break;

        case CAN_ID_ACCEL_Z:
        {
            value = sensor-> z;
        }
        break;

        case CAN_ID_DISTANCE:
        {
            value = sensor-> distance;
        }
        break;

        case CAN_ID_ALL:
        {
            int16_t x = (int16_t)(sensor->x * 1000.0f);
            int16_t y = (int16_t)(sensor->y * 1000.0f);
            int16_t z = (int16_t)(sensor->z * 1000.0f);
            uint16_t distance = (uint16_t)(sensor->distance * 10.0f);

            can_protocol_write_u16(&data[0], (uint16_t)x);
            can_protocol_write_u16(&data[2], (uint16_t)y);
            can_protocol_write_u16(&data[4], (uint16_t)z);
            can_protocol_write_u16(&data[6], distance);

            *dlc = CAN_DLC_ALL;
            return true;
        }
        default: 
        {
            return false;
        }
    }

    memcpy(data, &value, sizeof(float));
    *dlc = CAN_DLC_FLOAT;
    return true;
}