#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "gatt.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "gatt_profile_uuid.h"
#include "devinfoservice.h"
#include "peripheral.h"
#include "gapbondmgr.h"
#include "qcloud.h"
#include "log.h"
#include "error.h"
#include "flash.h"
#include "rf_phy_driver.h"
#include "pwrmgr.h"

#include <string.h>

#include "ota.h"
#include "macmanage.h"
#include "triad.h"

#include "ble_qiot_export.h"
#include "ble_qiot_template.h"
#include "ble_qiot_llsync_ota.h"

#include "qcloud_cmd.h"

uint8_t ble_name[GAP_DEVICE_NAME_LEN + 1] = "AXKIOT";
qcloud_ble_t *qcloud_config = NULL;

static uint8 Qcloud_TaskID;

// GAP Role Callbacks
static void QcloudGapStateCB(gaprole_States_t newState);
static gapRolesCBs_t bleSmartPeripheral_PeripheralCBs = {
    QcloudGapStateCB, // Profile State Change Callbacks
    NULL              // When a valid RSSI is read from controller (not used by application)
};

// 0:Uninitialized 1:Initialized
static int qcloud_init_status = 0;
// 0:ota disable 1:ota enable
static int qcloud_ota_enable_flag = 0;
// 0:ble connect state 1:connected 0:not connected
static int ble_conn_state = 0;

// Device name attribute value
// static uint8 attDeviceName[GAP_DEVICE_NAME_LEN] = "AXKIOT";

// GATT Callback
static void Qcloud_ProcessGATTMsg(gattMsgEvent_t *pMsg)
{
    switch (pMsg->method)
    {
    case ATT_EXCHANGE_MTU_REQ:
    {
        uint16_t mtu = pMsg->msg.exchangeMTUReq.clientRxMTU;
        if (mtu > L2CAP_MTU_SIZE)
            mtu = L2CAP_MTU_SIZE;
        LOG("mtu update%d \r\n", mtu);

        ble_event_sync_mtu(mtu);
        break;
    }
    default:
        break;
    }
}

static void ble_ota_start_cb(void)
{
    LOG("ble ota start callback\r\n");

    ota_start();

    qcloud_qiot_ota_start();

    return;
}

static void ble_ota_stop_cb(uint8_t result)
{
    LOG("ble ota stop callback, result %d\r\n", result);
    if (result == BLE_QIOT_OTA_SUCCESS)
    {
        ota_finish();
        qcloud_qiot_ota_success();
        NVIC_SystemReset();
    }

    qcloud_qiot_ota_failure();

    return;
}

static ble_qiot_ret_status_t ble_ota_valid_file_cb(uint32_t file_size, char *file_version)
{
    LOG("user valid file, size %d, file_version: %s\r\n", file_size, file_version);
    ota_valid_file(file_size);

    return BLE_QIOT_RS_OK;
}

void Qcloud_Init(uint8 task_id)
{
    Qcloud_TaskID = task_id;

#if (IS_SOC_OR_MODULE == 1)
#include "qcloud_hal.h"

    if (0 != qcloud_hal_qiot_init())
    {

        LOG("Qcloud_Init Fail , start set 3 oauth params and mac. \r\n");

        //you can set mac by uart set or the tool PhyPlusKit
        uint8_t mac[6] = {0, 0, 0, 0, 0, 0};
        uint64_t passcode = 0;
        LL_Rand((uint8_t *)(&passcode), 8);
        LOG("passcode = %x \n", passcode);
        mac[0] = ((passcode >> 0) & 0x000000ff);
        mac[1] = ((passcode >> 8) & 0x000000ff);
        mac[2] = ((passcode >> 16) & 0x000000ff);
        mac[3] = ((passcode >> 24) & 0x000000ff);
        mac[4] = ((passcode >> 32) & 0x000000ff);
        mac[5] = ((passcode >> 40) & 0x000000ff);

        LOG("Mac = %x:%x:%x:%x:%x:%x \n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        if (qcloud_hal_mac_write(mac) != 0)
        {
            LOG("set mac fail \r\n");
        }

        int ret = qcloud_hal_qiot_triad_set(PRODUCT_ID, DEVICE_NAME, PRODUCT_SECRET);
        if (0 != ret)
        {
            LOG("set 3 oauth params fail \r\n");
        }
        else
        {
            LOG("set 3 oauth params OK \r\n");
        }

        qcloud_hal_qiot_init();
    }
    else
    {
        LOG("arleady set 3 oauth params \r\n");
    }
    hal_pwrmgr_unlock_modify(MOD_USR1);
#endif
}

uint16 Qcloud_ProcessEvent(uint8 task_id, uint16 events)
{
    VOID task_id; // OSAL required parameter that isn't used in this function
    LOG("Qcloud_ProcessEvent: 0x%x\n", events);

    if (events & SYS_EVENT_MSG)
    {
        uint8 *pMsg;

        if ((pMsg = osal_msg_receive(Qcloud_TaskID)) != NULL)
        {
            // Release the OSAL message
            VOID osal_msg_deallocate(pMsg);
        }

        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if (events & START_DEVICE_EVT)
    {
        // Start the Device
        //VOID GAPRole_StartDevice(NULL);
        VOID GAPRole_StartDevice(&bleSmartPeripheral_PeripheralCBs);

        ble_qiot_advertising_start();

        return (events ^ START_DEVICE_EVT);
    }

    return 0;
}

static void QcloudGapStateCB(gaprole_States_t newState)
{
    static gaprole_States_t gapProfileState = GAPROLE_INIT;

    // connected
    if (newState == GAPROLE_CONNECTED)
    {
        gapProfileState = GAPROLE_CONNECTED;
        ble_gap_connect_cb();

#if 1
        // if use the internal 32K crystal oscillator, exit sleep state
        if (hal_pwrmgr_is_lock(MOD_USR1) == FALSE)
        {
            hal_pwrmgr_lock_modify(MOD_USR1);
        }
#endif

        ble_conn_state = 1;
    }
    // disconnected
    else if (gapProfileState == GAPROLE_CONNECTED && newState != GAPROLE_CONNECTED)
    {
        gapProfileState = newState;
        ble_gap_disconnect_cb();
        ble_qiot_advertising_start();

        ble_conn_state = 0;
    }
    else
    {
        //LOG("GapState:%d \n", newState);
    }
}

void tx_power_set(int32_t power)
{
    uint8 t_rfpw;

    switch (power)
    {
    case 5:
        t_rfpw = RF_PHY_TX_POWER_5DBM;
        break;
    case 4:
        t_rfpw = RF_PHY_TX_POWER_4DBM;
        break;
    case 3:
    case 2:
        t_rfpw = RF_PHY_TX_POWER_3DBM;
        break;
    case 1:
    case 0:
    case -1:
        t_rfpw = RF_PHY_TX_POWER_0DBM;
        break;
    case -2:
    case -3:
        t_rfpw = RF_PHY_TX_POWER_N3DBM;
        break;
    case -4:
    case -5:
        t_rfpw = RF_PHY_TX_POWER_N5DBM;
        break;
    case -6:
        t_rfpw = RF_PHY_TX_POWER_N6DBM;
        break;
    default:
        if (power > 5)
        {
            t_rfpw = RF_PHY_TX_POWER_EXTRA_MAX;
        }
        if (power >= -10)
        {
            t_rfpw = RF_PHY_TX_POWER_N10DBM;
        }
        else if (power >= -15)
        {
            t_rfpw = RF_PHY_TX_POWER_N15DBM;
        }
        else
        {
            t_rfpw = RF_PHY_TX_POWER_N20DBM;
        }
        break;
    }

    rf_phy_set_txPower(t_rfpw);
}

int qcloud_start(void)
{
    if (0 != qcloud_init_status)
    {
        return PPlus_ERR_FATAL;
    }

    // set tx power
    tx_power_set(qcloud_config->tx_power);

    // set ota enable
    qcloud_set_ota_enable(qcloud_config->ota_enable);

    // set device name
    qcloud_ble_name_set(qcloud_config->ble_name);

    // set connect interval
    uint8 enable_update_request = TRUE;
    uint16 desired_min_interval = qcloud_config->conn_int_min;
    uint16 desired_max_interval = qcloud_config->conn_int_max;
    uint16 desired_slave_latency = qcloud_config->conn_latency;
    uint16 desired_conn_timeout = qcloud_config->conn_timeout;

    GAPRole_SetParameter(GAPROLE_PARAM_UPDATE_ENABLE, sizeof(uint8), &enable_update_request);
    GAPRole_SetParameter(GAPROLE_MIN_CONN_INTERVAL, sizeof(uint16), &desired_min_interval);
    GAPRole_SetParameter(GAPROLE_MAX_CONN_INTERVAL, sizeof(uint16), &desired_max_interval);
    GAPRole_SetParameter(GAPROLE_SLAVE_LATENCY, sizeof(uint16), &desired_slave_latency);
    GAPRole_SetParameter(GAPROLE_TIMEOUT_MULTIPLIER, sizeof(uint16), &desired_conn_timeout);

    // set adverting interval
    uint16 adv_min_interval = qcloud_config->adv_int_min;
    uint16 adv_max_interval = qcloud_config->adv_int_max;
    GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MIN, adv_min_interval);
    GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MAX, adv_max_interval);
    GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, adv_min_interval);
    GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, adv_max_interval);

    // set GATT callback
    gattServApp_RegisterCB(Qcloud_ProcessGATTMsg);

    // Set the GAP Characteristics
    // GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName);
    GGS_SetParameter(GGS_DEVICE_NAME_ATT, strlen((char *)ble_name), ble_name);

    // Initialize GATT attributes
    GGS_AddService(GATT_ALL_SERVICES);         // GAP
    GATTServApp_AddService(GATT_ALL_SERVICES); // GATT attributes

    osal_set_event(Qcloud_TaskID, START_DEVICE_EVT);

    // register ota event && init ble qiot
#if BLE_QIOT_SUPPORT_OTA
    ble_ota_callback_reg(ble_ota_start_cb, ble_ota_stop_cb, ble_ota_valid_file_cb);
#endif

    ble_qiot_explorer_init();

    qcloud_init_status = 1;

    return 0;
}

int qcloud_bind_flush(void)
{
    // if (0 != qcloud_init_status)
    // {
    //     return PPlus_ERR_FATAL;
    // }

    flash_erase_ucds(USER_QIOT_RECORD_FLASH_ADDR);
    return 0;
}

void qcloud_config_exec(qcloud_ble_t *qcloud_cfg)
{
    qcloud_config = qcloud_cfg;
}

void qcloud_at_config_read(qcloud_cmd_t *cfg)
{
    flash_read_ucds_block_byte(USER_QIOT_AT_FLASH_ADDR, sizeof(qcloud_cmd_t), (uint8_t *)cfg);
}

int qcloud_config_save(qcloud_cmd_t *cfg)
{
    flash_erase_ucds(USER_QIOT_AT_FLASH_ADDR);
    flash_write_ucds_block_byte(USER_QIOT_AT_FLASH_ADDR, sizeof(qcloud_cmd_t), (uint8_t *)cfg);

    return 0;
}

int qcloud_ble_name_set(const char *name)
{
    if (strlen(name) > GAP_DEVICE_NAME_LEN)
    {
        return 1;
    }

    strcpy((char *)ble_name, name);

    return 0;
}

int qcloud_set_ota_enable(int enable)
{
    qcloud_ota_enable_flag = enable;

    return 0;
}

int qcloud_get_ota_is_enable(void)
{
    return qcloud_ota_enable_flag;
}

int qcloud_adv_en_set(uint8_t enable)
{
    uint8_t advertising_enable = (enable == 0) ? FALSE : TRUE;
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8), &advertising_enable);

    return 0;
}

int qcloud_adv_en_get(void)
{
    uint8_t advertising_enable;
    GAPRole_GetParameter(GAPROLE_ADVERT_ENABLED, &advertising_enable);

    return (advertising_enable == FALSE) ? 0 : 1;
}

int qcloud_ble_disconn(void)
{
    GAPRole_TerminateConnection();

    return 0;
}

int qcloud_ble_connect_state_get(void)
{
    return ble_conn_state;
}

int qcloud_qiot_property_get(void)
{
    return BLE_QIOT_RS_OK == ble_event_get_status() ? 0 : 1;
}

int qcloud_qiot_property_post(const char *data, uint32_t len)
{
    return BLE_QIOT_RS_OK == aithinker_ble_user_property_report_data(data, len) ? 0 : 1;
}

int qcloud_qiot_event_post(uint8_t event_id, const char *data, uint32_t len)
{
    return BLE_QIOT_RS_OK == aithinker_ble_event_post(event_id, data, len) ? 0 : 1;
}

int qcloud_qiot_action_post(uint8_t action_id, uint8_t result, const char *data, uint32_t len)
{
    return BLE_QIOT_RS_OK == aithinker_ble_lldata_action_reply_handle(action_id, result, data, len) ? 0 : 1;
}
