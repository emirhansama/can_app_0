
#ifndef CAN_APP_CONFIG_H
#define CAN_APP_CONFIG_H

#define CAN_ID_ACCEL_X              (0x100)
#define CAN_ID_ACCEL_Y              (0x101)
#define CAN_ID_ACCEL_Z              (0x102)
#define CAN_ID_DISTANCE             (0x103)
#define CAN_ID_ALL                  (0x104)

#define CAN_DLC_FLOAT               (4)
#define CAN_DLC_ALL                 (8)

#define USB_CMD_HELP                (0)
#define USB_CMD_ACCEL_X             (1)
#define USB_CMD_ACCEL_Y             (2)
#define USB_CMD_ACCEL_Z             (3)
#define USB_CMD_DISTANCE            (4)
#define USB_CMD_ALL                 (5)
#define USB_CMD_CONTINUOUS          (6)
#define USB_TX_BUFFER_SIZE          (128)
#define USB_RX_QUEUE_LENGTH         (9)

#define CAN_CONTINUOUS_PERIOD_MS    (500)
#define LED_BLINK_PERIOD_MS         (500)

#define CAN_TX_QUEUE_LENGTH         (8)
#define CAN_RX_QUEUE_LENGTH         (8)

#endif
