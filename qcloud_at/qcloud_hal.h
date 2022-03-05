#ifndef __QCLOUD_HAL_H__
#define __QCLOUD_HAL_H__

#include "qcloud_cmd.h"
#include "sdk_version.h"

#include <stdint.h>

///////////////////////hal interface///////////////////////
/**
 * @brief   生效Qcloud配置
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_config_exec(qcloud_ble_t *qcloud_cfg);
/**
 * @brief   读取AT配置
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_at_config_read(qcloud_cmd_t *cfg);
/**
 * @brief   保存AT配置
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_at_config_save(qcloud_cmd_t *cfg);
/**
 * @brief   写mac
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_mac_write(uint8_t *mac);
/**
 * @brief   读mac
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_mac_read(uint8_t *mac);
/**
 * @brief   进入浅睡眠模式
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_light_sleep(void);
/**
 * @brief   进入深度睡眠模式
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_deep_sleep(void);
/**
 * @brief   重启设备
 */
void qcloud_hal_soft_reset(void);
/**
 * @brief   设置BLE名称
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_ble_name_set(const char *name);
/**
 * @brief   设置BLE广播间隔
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_ble_adv_int_set(uint16_t min, uint16_t max);
/**
 * @brief   获取功率范围
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_ble_power_range_get(int32_t *min, int32_t *max);
/**
 * @brief   获取BLE连接状态
 *
 * @return  0:未连接 other:已连接
 */
int qcloud_hal_ble_connect_state_get(void);
/**
 * @brief   设置BLE广播使能
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_ble_disconn(void);
/**
 * @brief   设置BLE广播使能
 *
 * @param   enable 0:关闭 1:开启
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_ble_adv_en_set(uint8_t enable);
/**
 * @brief   获取BLE广播使能
 *
 * @param   enable 0:关闭 1:开启
 *
 * @return  0:关闭 1:开启
 */
int qcloud_hal_ble_adv_en_get(void);
/**
 * @brief   Qiot 初始化
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_qiot_init(void);
/**
 * @brief   Qiot 清除绑定信息
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_qiot_bind_flush(void);
/**
 * @brief   Qiot OTA使能
 *
 * @param   enable 0:关闭 1:开启
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_qiot_ota_set(uint8_t enable);
/**
 * @brief   Qiot OTA获取使能状态
 *
 * @return  0:关闭 1:开启
 */
int qcloud_hal_qiot_ota_get(void);
/**
 * @brief   Qiot 设置三元组
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_qiot_triad_set(char *product_id, char *device_name, char *device_secret);
/**
 * @brief   Qiot 请求获取 property
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_qiot_property_get(void);
/**
 * @brief   Qiot 上报 property
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_qiot_property_post(const char *data, uint32_t len);
/**
 * @brief   Qiot 推送 event
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_qiot_event_post(uint8_t event_id, const char *data, uint32_t len);
/**
 * @brief   Qiot 推送 action reply
 *
 * @return  0:成功 other:失败
 */
int qcloud_hal_qiot_action_post(uint8_t action_id, uint8_t result, const char *data, uint32_t len);

#endif
