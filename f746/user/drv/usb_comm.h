#ifndef USB_COMM_H
#define USB_COMM_H

#include <stdint.h>
#include <stdbool.h>

bool usb_comm_init(void);
bool usb_comm_send(const char*message);
void usb_comm_receive(uint8_t *data, uint32_t length);
bool usb_comm_get_char(uint8_t *ch, uint32_t timeout);

#endif