#include "qcloud_cmd.h"
#include "qcloud_hal.h"
#include "at_cmd.h"
#include "at_utils.h"
#include "peripheral_hal.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AT_TABLE_SIZE (sizeof(at_table) / sizeof(at_cmd_t))

static qcloud_cmd_t user_info;
static char         at_buf[QCLOUD_AT_BUFFER_SIZE];

static int AT_CmdFunc_AT(int opt, int argc, char *argv[]);
static int AT_CmdFunc_ATE1(int opt, int argc, char *argv[]);
static int AT_CmdFunc_ATE0(int opt, int argc, char *argv[]);
static int AT_CmdFunc_Help(int opt, int argc, char *argv[]);
static int AT_CmdFunc_VER(int opt, int argc, char *argv[]);
static int AT_CmdFunc_RST(int opt, int argc, char *argv[]);
static int AT_CmdFunc_RESTORE(int opt, int argc, char *argv[]);
static int AT_CmdFunc_Sleep(int opt, int argc, char *argv[]);
static int AT_CmdFunc_BleMac(int opt, int argc, char *argv[]);
static int AT_CmdFunc_BleName(int opt, int argc, char *argv[]);
static int AT_CmdFunc_BleTxPower(int opt, int argc, char *argv[]);
static int AT_CmdFunc_BleConnState(int opt, int argc, char *argv[]);
static int AT_CmdFunc_BleDisconnect(int opt, int argc, char *argv[]);
static int AT_CmdFunc_AdvInt(int opt, int argc, char *argv[]);
static int AT_CmdFunc_AdvEn(int opt, int argc, char *argv[]);
static int AT_CmdFunc_ConnInterval(int opt, int argc, char *argv[]);
static int AT_CmdFunc_QiotTriad(int opt, int argc, char *argv[]);
static int AT_CmdFunc_QiotBindFlush(int opt, int argc, char *argv[]);
static int AT_CmdFunc_QiotInit(int opt, int argc, char *argv[]);
static int AT_CmdFunc_QiotOta(int opt, int argc, char *argv[]);
static int AT_CmdFunc_QiotPropGet(int opt, int argc, char *argv[]);
static int AT_CmdFunc_QiotPropPost(int opt, int argc, char *argv[]);
static int AT_CmdFunc_QiotEvtPost(int opt, int argc, char *argv[]);

at_cmd_t at_table[] = {
    {"AT+QTRIAD", "Qiot set triad", (fp_at_callback)AT_CmdFunc_QiotTriad},
    {"AT+QFBIND", "Qiot flush bind", (fp_at_callback)AT_CmdFunc_QiotBindFlush},
    {"AT+QINIT", "Qiot init", (fp_at_callback)AT_CmdFunc_QiotInit},
    {"AT+QOTA", "Qiot set OTA", (fp_at_callback)AT_CmdFunc_QiotOta},
    {"AT+QGPROP", "Qiot get status", (fp_at_callback)AT_CmdFunc_QiotPropGet},
    {"AT+QPPROP", "Qiot property", (fp_at_callback)AT_CmdFunc_QiotPropPost},
    {"AT+QPEVT", "Qiot event", (fp_at_callback)AT_CmdFunc_QiotEvtPost},

    {"AT+BLECONINTV", "set adv conintv", (fp_at_callback)AT_CmdFunc_ConnInterval},
    // {"AT+BLEADVEN", "set adv able", (fp_at_callback)AT_CmdFunc_AdvEn},
    {"AT+BLEADVINTV", "set adv", (fp_at_callback)AT_CmdFunc_AdvInt},
    {"AT+BLENAME", "set ble name", (fp_at_callback)AT_CmdFunc_BleName},
    {"AT+BLEDISCON", "ble discon", (fp_at_callback)AT_CmdFunc_BleDisconnect},
    {"AT+BLESTATE", "ble discon", (fp_at_callback)AT_CmdFunc_BleConnState},
    {"AT+BLERFPWR", "set RF power", (fp_at_callback)AT_CmdFunc_BleTxPower},
    {"AT+BLEMAC", "set addr", (fp_at_callback)AT_CmdFunc_BleMac},

    {"AT+SLEEP", "set power mode", (fp_at_callback)AT_CmdFunc_Sleep},
    {"AT+RESTORE", "Restore", (fp_at_callback)AT_CmdFunc_RESTORE},
    {"AT+RST", "Rst", (fp_at_callback)AT_CmdFunc_RST},
    {"AT+GMR", "get version", (fp_at_callback)AT_CmdFunc_VER},
    {"AT+HELP", "func desc", (fp_at_callback)AT_CmdFunc_Help},
    {"ATE1", "on echo", (fp_at_callback)AT_CmdFunc_ATE1},
    {"ATE0", "off echo", (fp_at_callback)AT_CmdFunc_ATE0},
    {"AT", "Check AT mode", (fp_at_callback)AT_CmdFunc_AT},
};

static void at_print_info(void)
{
    at_printf("at version:%s\r\n", AT_VERSION);
    at_printf("sdk version:%s\r\n", SDK_VERSION);
    at_printf("qcloud sdk version:%s\r\n", QCLOUD_VERSION);
    at_printf("firmware version:%s\r\n", FW_VERSION);
}

static void qcloud_at_config_reset(qcloud_cmd_t *cfg)
{
    cfg->magic_num = QCLOUD_AT_MAGIC_NUM;

    cfg->echo = 0;

    strcpy(cfg->ble.ble_name, QCLOUD_AT_BLE_NAME_DEFAULT);
    cfg->ble.auto_light_sleep_enable = 0;    // disable auto enter light sleep mode
    cfg->ble.ota_enable              = 1;    // enable ota
    cfg->ble.tx_power                = 0;    // set tx power
    cfg->ble.adv_int_min             = 800;  // 500ms
    cfg->ble.adv_int_max             = 820;  // 500ms
    cfg->ble.conn_int_min            = 64;   // 80ms
    cfg->ble.conn_int_max            = 80;   // 100ms
    cfg->ble.conn_latency            = 0;    // 0
    cfg->ble.conn_timeout            = 600;  // 600*10ms

    // save config
    qcloud_hal_at_config_save(cfg);
}

static int AT_CmdFunc_AT(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}

static int AT_CmdFunc_ATE1(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        at_echo_enable(1);

        user_info.echo = 1;
        qcloud_hal_at_config_save(&user_info);
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_ATE0(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        at_echo_enable(0);

        user_info.echo = 0;
        qcloud_hal_at_config_save(&user_info);
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_Help(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        uint8_t count = 0;
        while (count < (AT_TABLE_SIZE)) {
            // at_printf("cmd:%s descaiption:%s\r\n", at_table[count].cmd, at_table[count].help);
            at_printf("%s:%s\r\n", at_table[count].cmd, at_table[count].help);
            count++;
        }
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_RST(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        respond_ok();
        qcloud_hal_soft_reset();
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_RESTORE(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        // flush bind info
        qcloud_hal_qiot_bind_flush();

        // reset at config
        qcloud_at_config_reset(&user_info);

        // response ok && reboot
        respond_ok();
        qcloud_hal_soft_reset();
    } else if (opt == AT_TYPE_QUERY) {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_Sleep(int opt, int argc, char *argv[])
{
    /* 0:light sleep
     * 1:light sleep && automatically enter light sleep after power-on
     * 2:deep sleep
     */
    if (opt == AT_TYPE_SET) {
        int8_t mode = atoi(argv[0]);

        if (mode >= 0 && mode <= 2) {
            switch (mode) {
                case 0:
                    user_info.ble.auto_light_sleep_enable = 0;
                    qcloud_hal_at_config_save(&user_info);
                    if (0 != qcloud_hal_light_sleep()) {
                        return AT_ERROR;
                    }
                    break;
                case 1:
                    user_info.ble.auto_light_sleep_enable = 1;
                    qcloud_hal_at_config_save(&user_info);
                    if (0 != qcloud_hal_light_sleep()) {
                        return AT_ERROR;
                    }
                    break;
                case 2:
                    // response ok && enter deep mode
                    respond_ok();
                    qcloud_hal_deep_sleep();
                    break;
                // Don’t worry, "default" is unreachable
                default:
                    break;
            }
        } else {
            return AT_ERROR;
        }
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_VER(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        at_print_info();
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_BleMac(int opt, int argc, char *argv[])
{
    uint8_t mac[6] = {0};

    if (opt == AT_TYPE_SET) {
        if (strlen(argv[0]) != 12) {
            return AT_ERROR;
        }
        str2bytes(argv[0], mac, 6);
        if (qcloud_hal_mac_write(mac) != 0) {
            return AT_ERROR;
        }
    } else if (opt == AT_TYPE_QUERY) {
        int i;

        qcloud_hal_mac_read(mac);

        at_printf("+BLEMAC:");
        for (i = 0; i < 6; i++) {
            at_printf("%02X", mac[i]);
        }
        at_printf("\r\n");
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_BleName(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_SET) {
        uint8_t size = strlen(argv[0]);

        if (size < QCLOUD_AT_BLE_NAME_LIMIT) {
            if (0 != qcloud_hal_ble_name_set(argv[0])) {
                return AT_ERROR;
            }

            // save name to at config
            memset(user_info.ble.ble_name, 0, QCLOUD_AT_BLE_NAME_LIMIT + 1);
            strcpy(user_info.ble.ble_name, argv[0]);
            qcloud_hal_at_config_save(&user_info);
        } else {
            return AT_ERROR;
        }
    } else if (opt == AT_TYPE_QUERY) {
        at_printf("+BLENAME:%s\r\n", user_info.ble.ble_name);
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_BleTxPower(int opt, int argc, char *argv[])
{
    int32_t tx_power = 0;
    int32_t power_min, power_max;

    qcloud_hal_ble_power_range_get(&power_min, &power_max);

    if (opt == AT_TYPE_SET) {
        tx_power = atoi(argv[0]);

        if (tx_power < power_min || tx_power > power_max) {
            return AT_ERROR;
        }

        user_info.ble.tx_power = tx_power;
        qcloud_hal_at_config_save(&user_info);
    } else if (opt == AT_TYPE_QUERY) {
        at_printf("+BLERFPWR:MAX:%d MIN:%d CURRENT:%d\r\n", power_max, power_min, user_info.ble.tx_power);
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_BleConnState(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_QUERY) {
        int state = qcloud_hal_ble_connect_state_get() ? 1 : 0;

        at_printf("+BLESTATE:%d\r\n", state);
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_BleDisconnect(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        if (0 != qcloud_hal_ble_disconn()) {
            return AT_ERROR;
        }
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_AdvInt(int opt, int argc, char *argv[])
{
    /* Minimum advertising interval for undirected and low duty cycle directed advertising. Value range: 0x0020 - 0x4000
     * Value range: 0x0020 - 0x4000 (20ms - 10240ms 0.625ms/step),Read/Write Size is uint16_t.
     */
    if (opt == AT_TYPE_SET) {
        uint16_t adv_int_min = atoi(argv[0]);
        // uint16_t adv_int_max = atoi(argv[1]);
        uint16_t adv_int_max = adv_int_min;

        if (adv_int_min < 0x0020 || adv_int_min > 0x4000) {
            return AT_ERROR;
        }
        if (0 != qcloud_hal_ble_adv_int_set(adv_int_min, adv_int_max)) {
            return AT_ERROR;
        }

        user_info.ble.adv_int_min = adv_int_min;
        user_info.ble.adv_int_max = adv_int_max;
        qcloud_hal_at_config_save(&user_info);
    } else if (opt == AT_TYPE_QUERY) {
        at_printf("+BLEADVINTV:%d\r\n", user_info.ble.adv_int_min);
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_ConnInterval(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_SET) {
        if (4 != argc) {
            return AT_ERROR;
        }

        uint16_t interval_min = atoi(argv[0]);
        uint16_t interval_max = atoi(argv[1]);
        uint16_t latency      = atoi(argv[2]);
        uint16_t timeout      = atoi(argv[3]);

        // check param
        if (interval_min >= 0x0006 && interval_min <= 0x0C80 && interval_max >= 0x0006 && interval_max <= 0x0C80 &&
            latency >= 0 && latency <= 499 && timeout >= 10 && timeout <= 3200) {
            user_info.ble.conn_int_min = interval_min;
            user_info.ble.conn_int_max = interval_max;
            user_info.ble.conn_latency = latency;
            user_info.ble.conn_timeout = timeout;

            qcloud_hal_at_config_save(&user_info);
        }
    } else if (opt == AT_TYPE_QUERY) {
        at_printf("AT+BLECONINTV=%d,%d,%d,%d\r\n", user_info.ble.conn_int_min, user_info.ble.conn_int_max,
                  user_info.ble.conn_latency, user_info.ble.conn_timeout);
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_AdvEn(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_SET) {
        uint8_t enable = atoi(argv[0]);

        if (enable == 0 || enable == 1) {
            qcloud_hal_ble_adv_en_set(enable);
        } else {
            return AT_ERROR;
        }
    } else if (opt == AT_TYPE_QUERY) {
        at_printf("BLEADVEN:%d\r\n", qcloud_hal_ble_adv_en_get());
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_QiotTriad(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_SET) {
        uint32_t len = atoi(argv[0]);
        int      ret = 0;

        char *data     = NULL;
        char *triad[3] = {NULL};

        if (len == 0 || len > QCLOUD_QIOT_TRIAD_SIZE) {
            return AT_ERROR;
        }

        data = malloc(len + 1);
        if (NULL == data) {
            return AT_ERROR;
        }

        at_printf(">");
        at_receive(data, len);
        data[len] = '\0';

        /* ProductId,DeviceName,DeviceSecret */
        if (2 != strsplit(data, triad, ",", 2)) {
            return AT_ERROR;
        }

        if (0 == strlen(triad[0]) || 0 == strlen(triad[1]) || 0 == strlen(triad[2])) {
            return AT_ERROR;
        }

        // at_printf("%s\r\n%s\r\n%s\r\n", triad[0], triad[1], triad[2]);
        ret = qcloud_hal_qiot_triad_set(triad[0], triad[1], triad[2]);
        free(data);

        if (0 != ret) {
            return AT_ERROR;
        }
    } else {
        return AT_ERROR;
    }
    return AT_OK;
}

static int AT_CmdFunc_QiotBindFlush(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        if (0 != qcloud_hal_qiot_bind_flush()) {
            return AT_ERROR;
        }
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_QiotInit(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        if (0 != qcloud_hal_qiot_init()) {
            return AT_ERROR;
        }
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_QiotOta(int opt, int argc, char *argv[])
{
    uint8_t enable = 0;

    if (opt == AT_TYPE_SET) {
        enable = atoi(argv[0]);

        if (enable == 0 || enable == 1) {
            qcloud_hal_qiot_ota_set(enable);
        } else {
            return AT_ERROR;
        }
    } else if (opt == AT_TYPE_QUERY) {
        enable = qcloud_hal_qiot_ota_get();
        at_printf("ota:%d\r\n", enable);
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_QiotPropGet(int opt, int argc, char *argv[])
{
    if (opt == AT_TYPE_EXECUTE) {
        if (0 != qcloud_hal_qiot_property_get()) {
            return AT_ERROR;
        }
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_QiotPropPost(int opt, int argc, char *argv[])
{
    /**
     * @param length
     */
    if (opt == AT_TYPE_SET) {
        uint32_t len  = atoi(argv[0]);
        char *   data = NULL;
        int      ret  = 0;

        if (len == 0 || len > QCLOUD_QIOT_EVENT_SIZE) {
            return AT_ERROR;
        }

        data = malloc(len);
        if (NULL == data) {
            return AT_ERROR;
        }

        at_printf(">");
        at_receive(data, len);

        ret = qcloud_hal_qiot_property_post(data, len);
        free(data);

        if (0 != ret) {
            return AT_ERROR;
        }
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static int AT_CmdFunc_QiotEvtPost(int opt, int argc, char *argv[])
{
    /**
     * @param eventId
     * @param length
     */
    if (opt == AT_TYPE_SET) {
        uint8_t  event_id = atoi(argv[0]);
        uint32_t len      = atoi(argv[1]);
        char *   data     = NULL;
        int      ret      = 0;

        if (len == 0 || len > QCLOUD_QIOT_EVENT_SIZE) {
            return AT_ERROR;
        }

        data = malloc(len);
        if (NULL == data) {
            return AT_ERROR;
        }

        at_printf(">");
        at_receive(data, len);

        ret = qcloud_hal_qiot_event_post(event_id, data, len);
        free(data);

        if (0 != ret) {
            return AT_ERROR;
        }
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

static void qcloud_at_config_exec(qcloud_cmd_t *cfg)
{
    // if set 1, automatically enter light sleep after power-on
    if (1 == user_info.ble.auto_light_sleep_enable) {
        at_printf("sleep\r\n");
        qcloud_hal_light_sleep();
    }

#if 0
    switch (cfg->ble.conn_int_min)
    {
    case 80:
        cfg->ble.conn_int_min = 160;
        break;
    case 160:
        cfg->ble.conn_int_min = 400;
        break;
    case 400:
        cfg->ble.conn_int_min = 800;
        break;
    case 800:
        cfg->ble.conn_int_min = 1600;
        break;
    case 1600:
        cfg->ble.conn_int_min = 80;
        break;
    default:
        cfg->ble.conn_int_min = 80;
        break;
    }
    cfg->ble.conn_int_max = cfg->ble.conn_int_min;
    cfg->ble.conn_latency = cfg->ble.conn_latency ? 0 : 4;
    qcloud_hal_at_config_save(cfg);
#endif

    at_echo_enable(user_info.echo);

    qcloud_hal_config_exec(&(cfg->ble));
}

void qcloud_at_init(void)
{
    at_init(at_table, AT_TABLE_SIZE, at_buf, QCLOUD_AT_BUFFER_SIZE);

    qcloud_hal_at_config_read(&user_info);
    /* Check magic number
     * set at defalut config if at config uninitialized
     */
    if (QCLOUD_AT_MAGIC_NUM != user_info.magic_num) {
        qcloud_at_config_reset(&user_info);
    }
    // execute at config
    qcloud_at_config_exec(&user_info);

    at_print_info();
    at_printf("ready\r\n");
}

///////////////////////callback function///////////////////////


void qcloud_property_down(const char *data, int len)
{
#if (IS_SOC_OR_MODULE == 1)
    at_printf("+E:QPROP,%d:", len);
    at_write_data(data, len);
    at_printf("\r\n");
#else
    at_printf("qcloud_property_down:%d\n", len);
    int i, j;
    for (i = 0; i < len; ++i) {
        at_printf("%02x ", data[i]);
    }
    at_printf("\n");

    switch (data[0]) {
        case 0:
            if (data[1]) {
                led_set_rgb(255, 0, 0);
            } else {
                led_set_rgb(0, 0, 0);
            }
            break;

        case 0x81:
            if (data[2] == 0) {
                led_set_rgb(255, 0, 0);
            } else if (data[2] == 1) {
                led_set_rgb(0, 255, 0);
            } else if (data[2] == 2) {
                led_set_rgb(0, 0, 255);
            }
            break;

        default:
            break;
    }
#endif
}

#if 0
void qcloud_action_down(int action_id, const char *data, int len)
{
    char     data_buf[QCLOUD_QIOT_ACT_SIZE];
    uint32_t data_len = 0;
    uint32_t timeout  = 3500;
    char *   pos      = NULL;
    uint32_t result   = 0;

    at_printf("+E:QACT,%d:", len);
    at_write_data(data, len);
    at_printf("\r\n");

    uint32_t timemask = _at_timemark();

    //"+ACTREP,ret,len\r\n"
    data_len = at_receive_until_timeout(data_buf, 32, "\r\n", timeout);
    if (0 == data_len) {
        goto error;
    }
    data_buf[data_len - 2] = '\0';

    pos = strstr(data_buf, "+ACTREP,");
    if (NULL == pos) {
        goto error;
    }
    // get result
    pos += strlen("+ACTREP,");
    result = atoi(pos);
    if (result > 1) {
        goto error;
    }
    // get post len
    if (',' != pos[1]) {
        goto error;
    }
    data_len = atoi(pos + 2);
    if (result > QCLOUD_QIOT_ACT_SIZE) {
        goto error;
    }

    // post action data
    at_printf(">");
    at_receive(data_buf, data_len);
    if (0 != qcloud_hal_qiot_action_post(action_id, 0, data_buf, data_len)) {
        goto error;
    }
    respond_ok();
    return;

error:
    respond_error();
    return;
}
#else
void qcloud_action_down(int action_id, const char *data, int len)
{
    at_printf("+E:QACT,%d,%d:", action_id, len);
    at_write_data(data, len);
    at_printf("\r\n");
}
#endif

/**
 * @brief   call by llsync_connection_state_set
 */
void qcloud_qiot_connected(void)
{
    at_printf("+E:QCONN\r\n");
}

/**
 * @brief   call by llsync_connection_state_set
 */
void qcloud_qiot_disconnected(void)
{
    at_printf("+E:QDISCONN\r\n");
}

/**
 * @brief   call this function when the device is bind
 */
void qcloud_qiot_bind(void)
{
    at_printf("+E:QBIND\r\n");
}

/**
 * @brief   call this function when the device is unbind
 */
void qcloud_qiot_unbind(void)
{
    at_printf("+E:QUNBIND\r\n");
}

/**
 * @brief   call this function when the ota is start
 */
void qcloud_qiot_ota_start(void)
{
    at_printf("+E:QOTASTART\r\n");
}

/**
 * @brief   call this function when the ota is success
 */
void qcloud_qiot_ota_success(void)
{
    at_printf("+E:QOTASUCC\r\n");
}

/**
 * @brief   call this function when the ota is failure
 */
void qcloud_qiot_ota_failure(void)
{
    at_printf("+E:QOTAFAIL\r\n");
}
