
#ifndef CAN_APP_CONFIG_H
#define CAN_APP_CONFIG_H

#define CAN_ID_ACCEL_X          (0x100)
#define CAN_ID_ACCEL_Y          (0x101)
#define CAN_ID_ACCEL_Z          (0x102)
#define CAN_ID_DISTANCE         (0x103)
#define CAN_ID_ALL              (0x104)

#define CAN_DLC_FLOAT           (4)
#define CAN_DLC_ALL             (8)

#define ACCEL_TASK_PERIOD_MS    (20)
#define DIST_TASK_PERIOD_MS     (100)
#define LED_TASK_PERIOD_MS      (20)

#define ACCEL_ODR_HZ            (100)
#define ACCEL_FULL_SCALE_G      (2)

#define ACCEL_LPF_ALPHA         (0.2f)

#define LED_MAX_ACCEL_G         (2.0f)

#define HCSR04_TIMEOUT_MS       (40)

#define DIST_NEAR_CM            (10.0f)
#define DIST_FAR_CM             (30.0f)

#define BLINK_FAST_MS           (100)
#define BLINK_MEDIUM_MS         (250)
#define BLINK_SLOW_MS           (500)

#endif
