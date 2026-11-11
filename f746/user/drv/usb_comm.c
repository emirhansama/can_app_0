#include "usb_comm.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "can_app_config.h"
#include "cmsis_os2.h"
#include "usbd_def.h"
#include <stdbool.h>
#include <string.h>

extern USBD_HandleTypeDef hUsbDeviceFS;

static osMessageQueueId_t usb_rx_queue;
static uint8_t usb_tx_buffer[USB_TX_BUFFER_SIZE];

bool usb_comm_init(void)
{
    usb_rx_queue = osMessageQueueNew(USB_RX_QUEUE_LENGTH, sizeof(uint8_t), NULL);
    
    MX_USB_DEVICE_Init();
    return true;
}

void usb_comm_receive(uint8_t *data, uint32_t length)
{
    for(uint32_t i = 0; i < length; i++){
        osMessageQueuePut(usb_rx_queue, &data[i], 0, 0);
    }
}

bool usb_comm_send(const char *message)
{
    uint16_t length = (uint16_t)strlen(message);

    memcpy(usb_tx_buffer, message, length);

    return CDC_Transmit_FS(usb_tx_buffer, length) == USBD_OK;
}

bool usb_comm_get_char(uint8_t *ch, uint32_t timeout)
{
    return osMessageQueueGet(usb_rx_queue, ch, NULL, timeout) == osOK;
}
