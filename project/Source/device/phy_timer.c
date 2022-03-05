#include "phy_uart.h"
#include "ap_timer.h"

#include "at_cmd.h"

static uint32_t phy_time = 0;

static void phy_timer_irq_handle(uint8_t evt)
{
    if (APTM_EVT_TIMER_1 == evt)
    {
        phy_time++;
    }
}

void phy_timer_init(void)
{
    ap_timer_init(phy_timer_irq_handle);
    ap_timer_set(0, 1000); // 1000us
    ap_timer_int_mask(0, 0); // set 0 to enable interrupt
}

uint32_t _at_timemark(void)
{
    return phy_time;
}

int _at_timeout(uint32_t timemark, uint32_t timeout)
{
    if ((uint32_t)(phy_time - timemark) <= timeout)
    {
        return 0;
    }

    return 1;
}
