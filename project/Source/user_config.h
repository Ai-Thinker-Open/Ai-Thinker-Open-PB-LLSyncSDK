#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#define USER_DEV_NAME                   "AXK"

#define USER_VERSION                    "0.0.1"
#define USER_DEV_MFR                    "Aithinker"
#define USER_DEV_MODEL                  "PB62XX_MODULE"

// user flash space
#define USER_QIOT_TRIAD_FLASH_ADDR      0x0000
#define USER_QIOT_AT_FLASH_ADDR         0x1000
#define USER_QIOT_RECORD_FLASH_ADDR     0x2000
#define USER_QIOT_OTA_INFO_FLASH_ADDR   0x3000
#define USER_CFG_MTU_MAX                128
#define USER_CFG_DATA_ALIGN             4

#endif  // USER_CONFIG_H
