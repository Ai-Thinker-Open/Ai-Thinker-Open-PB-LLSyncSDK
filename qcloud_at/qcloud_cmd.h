#ifndef __QCLOUD_CMD_H__
#define __QCLOUD_CMD_H__

#include <stdint.h>

#define QCLOUD_AT_BUFFER_SIZE  512
#define QCLOUD_QIOT_TRIAD_SIZE 128
#define QCLOUD_QIOT_PROP_SIZE  512
#define QCLOUD_QIOT_EVENT_SIZE 256
#define QCLOUD_QIOT_ACT_SIZE   256

#define QCLOUD_AT_BLE_NAME_DEFAULT "AXK IoT"
#define QCLOUD_AT_BLE_NAME_LIMIT   29

#define QCLOUD_AT_MAGIC_NUM 0x5A

typedef struct {
    char        ble_name[QCLOUD_AT_BLE_NAME_LIMIT + 1];
    uint8_t     auto_light_sleep_enable; // if set 1, automatically enter light sleep after power-on 
    uint8_t     ota_enable;
    int32_t     tx_power;
    uint16_t    adv_int_max;
    uint16_t    adv_int_min;
    uint16_t    conn_int_max;
    uint16_t    conn_int_min;
    uint16_t    conn_latency;
    uint16_t    conn_timeout;
} qcloud_ble_t;

typedef struct {
    uint8_t      magic_num;
    uint8_t      echo;
    qcloud_ble_t ble;
} qcloud_cmd_t;

void qcloud_at_init(void);

///////////////////////callback function///////////////////////
void qcloud_property_down(const char *data, int len);
void qcloud_action_down(int action_id, const char *data, int len);
void qcloud_qiot_connected(void);
void qcloud_qiot_disconnected(void);
void qcloud_qiot_bind(void);
void qcloud_qiot_unbind(void);
void qcloud_qiot_ota_start(void);
void qcloud_qiot_ota_success(void);
void qcloud_qiot_ota_failure(void);

#endif
