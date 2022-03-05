#include "qcloud_hal.h"

#include "bcomdef.h"
#include "pwrmgr.h"
#include "phy_uart.h"

#include "qcloud.h"
#include "triad.h"
#include "macmanage.h"
#include "error.h"

int qcloud_hal_config_exec(qcloud_ble_t *qcloud_cfg)
{
    qcloud_config_exec(qcloud_cfg);
    return 0;
}

int qcloud_hal_at_config_read(qcloud_cmd_t *cfg)
{
    qcloud_at_config_read(cfg);

    return 0;
}

int qcloud_hal_at_config_save(qcloud_cmd_t *cfg)
{
    if (0 != qcloud_config_save(cfg))
    {
        return -1;
    }

    return 0;
}

int qcloud_hal_mac_write(uint8_t *mac)
{
    write_mac((uint8_t *)mac);
    return 0;
}

int qcloud_hal_mac_read(uint8_t *mac)
{
    read_mac(mac);

    return 0;
}

void gpio_wakeup_handle(GPIO_Pin_e pin, IO_Wakeup_Pol_e type)
{
    // if(hal_pwrmgr_is_lock(MOD_USR1) == FALSE) {
    //     hal_pwrmgr_lock_modify(MOD_USR1);
    // }
}

int qcloud_hal_light_sleep(void)
{
    hal_gpioin_register(P10, NULL, gpio_wakeup_handle);
    
    return PPlus_SUCCESS == hal_pwrmgr_unlock_modify(MOD_USR1) ? 0 : 1;
}

int qcloud_hal_deep_sleep(void)
{
    // set rx pin to wake 
    GPIO_Pin_e pin = P10;
    pwroff_cfg_t pwr_wkp_cfg[] = {{pin, POSEDGE}};
    hal_pwrmgr_poweroff(pwr_wkp_cfg, sizeof(pwr_wkp_cfg) / sizeof(pwr_wkp_cfg[0]));
    
    return 0;
}

void qcloud_hal_soft_reset(void)
{
    NVIC_SystemReset();
}

int qcloud_hal_ble_name_set(const char *name)
{
    qcloud_ble_name_set(name);
    return 0;
}

int qcloud_hal_ble_adv_int_set(uint16_t min, uint16_t max)
{
    return 0;
}

int qcloud_hal_ble_power_range_get(int32_t *min, int32_t *max)
{
    *min = -20;
    *max = 10;
    
    return 0;
}

int qcloud_hal_ble_connect_state_get(void)
{
    return qcloud_ble_connect_state_get();
}

int qcloud_hal_ble_disconn(void)
{
    qcloud_ble_disconn();

    return 0;
}

int qcloud_hal_ble_adv_en_set(uint8_t enable)
{
    qcloud_adv_en_set(enable);

    return 0;
}

int qcloud_hal_ble_adv_en_get(void)
{
    return qcloud_adv_en_get();
}

int qcloud_hal_qiot_init(void)
{
    if (0 != triad_init())
    {
        return -1;
    }

    if (0 != qcloud_start())
    {
        return -1;
    }

    return 0;
}

int qcloud_hal_qiot_bind_flush(void)
{
    if (0 != qcloud_bind_flush())
    {
        return -1;
    }

    return 0;
}

int qcloud_hal_qiot_ota_set(uint8_t enable)
{
    return qcloud_set_ota_enable(enable);
}

int qcloud_hal_qiot_ota_get(void)
{
    return qcloud_get_ota_is_enable();
}

int qcloud_hal_qiot_triad_set(char *product_id, char *device_name, char *device_secret)
{
    return 0 == triad_set(product_id, device_name, device_secret) ? 0 : 1;
}

int qcloud_hal_qiot_property_get(void)
{
    return 0 == qcloud_qiot_property_get() ? 0 : 1;
}

int qcloud_hal_qiot_property_post(const char *data, uint32_t len)
{
    return 0 == qcloud_qiot_property_post(data, len) ? 0 : 1;
}

int qcloud_hal_qiot_event_post(uint8_t event_id, const char *data, uint32_t len)
{
    return 0 == qcloud_qiot_event_post(event_id, data, len) ? 0 : 1;
}

int qcloud_hal_qiot_action_post(uint8_t action_id, uint8_t result, const char *data, uint32_t len)
{
    return 0 == qcloud_qiot_action_post(action_id, result, data, len) ? 0 : 1;
}
