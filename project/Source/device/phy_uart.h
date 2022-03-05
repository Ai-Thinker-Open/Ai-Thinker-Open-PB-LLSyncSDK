#ifndef __PHY_UART_H__
#define __PHY_UART_H__

#include "uart.h"

void phy_uart_init(void);
void phy_uart_deinit(void);
void phy_uart_write(uint8_t *buff, uint16_t len);
int _at_write_data(const char* data, uint16_t len);

#endif
