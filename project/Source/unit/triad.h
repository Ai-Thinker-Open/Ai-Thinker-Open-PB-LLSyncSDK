#ifndef TRIAD_H
#define TRIAD_H

#include "OSAL.h"

#include "user_config.h"

// ProductId DeviceName DeviceSecret
// user flash space addr:0x11005000
#define TRIAD_FLASH_ADDR    USER_QIOT_TRIAD_FLASH_ADDR
#define TRIAD_FLASH_SIZE    128
#define PRODUCTID_SIZE      32
#define DEVICENAME_SIZE     32
#define DEVICESECRET_SIZE   64

char *triad_get_productId(void);

char *triad_get_devicename(void);

char *triad_get_devicesecret(void);

int triad_set(char *product_id, char *device_name, char *device_secret);

int triad_init(void);


#endif /* TRIAD_H */
