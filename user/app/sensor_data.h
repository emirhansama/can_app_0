#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdbool.h>

typedef struct {
    float x;
    float y;
    float z;
    float distance;
} sensor_data_t;

bool sensor_data_init(void);

bool sensor_data_set_accel(float x, float y, float z);
bool sensor_data_set_distance(float distance);

bool sensor_data_get(sensor_data_t *data);

 #endif