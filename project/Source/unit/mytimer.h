#ifndef TIMER_H
#define TIMER_H

#include "OSAL.h"

#include "ble_qiot_import.h"


void Timer_Init(uint8 task_id);

uint16 Timer_ProcessEvent(uint8 task_id, uint16 events);

ble_timer_t timer_create(uint8_t type, ble_timer_cb timer_cb);

ble_qiot_ret_status_t timer_start(ble_timer_t timer_id, uint32_t period);

ble_qiot_ret_status_t timer_stop(ble_timer_t timer_id);

ble_qiot_ret_status_t timer_delete(ble_timer_t timer_id);

#endif /* TIMER_H */
