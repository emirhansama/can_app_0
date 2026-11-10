#ifndef HCSR04_H
#define HCSR04_H

#include "main.h"
#include <stdbool.h>

#define HCSR04_TRIGGER_US           (10)
#define HCSR04_CM_PER_US            (0.01715f)

bool hcsr04_init(void);
void hcsr04_trigger(void);
bool hcsr04_get_distance(float *distance);

#endif