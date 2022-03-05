#include "phy_uart.h"
#include "uart.h"
#include "gpio.h"
#include "log.h"
#include "pwrmgr.h"

#include "at_cmd.h"
#include "at_task.h"

static void phy_uart_rx_irq_handle(uart_Evt_t *pev)
{
    // exit sleep mode, must set rx fifo to 1
    if(pev->len > 1 && hal_pwrmgr_is_lock(MOD_USR1) == FALSE) {
        hal_pwrmgr_lock_modify(MOD_USR1);
    }

    _at_recv((char *)pev->data, pev->len);
    at_task_run();
}

void phy_uart_init(void)
{
    uart_Cfg_t cfg = {
        .tx_pin = P9,
        .rx_pin = P10,
        .rts_pin = GPIO_DUMMY,
        .cts_pin = GPIO_DUMMY,
        .baudrate = 115200,
        .use_fifo = TRUE,
        .hw_fwctrl = FALSE,
        .use_tx_buf = FALSE,
        .parity = FALSE,
        .evt_handler = phy_uart_rx_irq_handle,
    };
    hal_uart_init(cfg); //uart init
}

void phy_uart_deinit(void)
{
    hal_uart_deinit(); //uart deinit
}

void phy_uart_write(uint8_t *buff, uint16_t len)
{
    hal_uart_send_buff(buff, len);
}

int _at_write_data(const char* data, uint16_t len)
{
    phy_uart_write((uint8_t *)data, len);

    return len;
}
