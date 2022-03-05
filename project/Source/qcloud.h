/*
 * @Author: xuhongv
 * @Date: 2022-01-23 10:19:46
 * @LastEditTime: 2022-02-04 20:17:44
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \phyplus_qcloud_at\project\Source\qcloud.h
 */
#ifndef QCLOUD_H
#define QCLOUD_H

#include "types.h"
#include "qcloud_cmd.h"


#define IS_SOC_OR_MODULE (1) // 1:SOC  0:Module

#if (IS_SOC_OR_MODULE == 1)
#define PRODUCT_ID "Z57EKQBG56"
#define DEVICE_NAME "AiThinker03"
#define PRODUCT_SECRET "oLKC/uxWqwl2Ng0hAR1IQA=="
#endif 

// QCLOUD Task Events
#define START_DEVICE_EVT 0x0001
#define START_TEST_EVT 0x0002
#define STOP_TEST_EVT 0x0004
// QCLOUD test Events
#define TEST_REPORT_PROPERTY_EVT 0x0008
#define TEST_GET_STATUS_EVT 0x0010
#define TEST_EVENT_POST_EVT 0x0020


extern uint8_t ble_name[];

/*
 * Task Initialization for the BLE Application
 */
extern void Qcloud_Init(uint8 task_id);

/*
 * Task Event Processor for the BLE Application
 */
extern uint16 Qcloud_ProcessEvent(uint8 task_id, uint16 events);

int qcloud_start(void);

int qcloud_bind_flush(void);

void qcloud_config_exec(qcloud_ble_t *qcloud_cfg);
void qcloud_at_config_read(qcloud_cmd_t *cfg);
int qcloud_config_save(qcloud_cmd_t *cfg);
int qcloud_ble_name_set(const char *name);
int qcloud_set_ota_enable(int enable);
int qcloud_get_ota_is_enable(void);
int qcloud_adv_en_set(uint8_t enable);
int qcloud_adv_en_get(void);
int qcloud_ble_disconn(void);
int qcloud_ble_connect_state_get(void);

int qcloud_qiot_property_get(void);
int qcloud_qiot_property_post(const char *data, uint32_t len);
int qcloud_qiot_event_post(uint8_t event_id, const char *data, uint32_t len);
int qcloud_qiot_action_post(uint8_t action_id, uint8_t result, const char *data, uint32_t len);

#endif /* QCLOUD_H */
