#include "mytimer.h"

#define TIMER_MAX_TASK      4
#define TIMER_FULL_FLAG     0x0f

typedef struct timer_evt {
    uint8_t id;
    uint8_t type;
    ble_timer_cb timer_cb;
}timer_evt_t;

static uint8 Timer_TaskID;
static uint8 timer_flag;

static timer_evt_t timer_event[TIMER_MAX_TASK];

void Timer_Init(uint8 task_id)
{
    Timer_TaskID = task_id;
}

uint16 Timer_ProcessEvent(uint8 task_id, uint16 events)
{
    VOID task_id; // OSAL required parameter that isn't used in this function

    if (events & SYS_EVENT_MSG)
    {
        uint8 *pMsg;

        if ((pMsg = osal_msg_receive(Timer_TaskID)) != NULL)
        {
            // Release the OSAL message
            VOID osal_msg_deallocate(pMsg);
        }

        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if (events & timer_flag)
    {
        uint8_t id;
        
        for(id = 0; id < TIMER_MAX_TASK; id++)
        {
            if((1 << id) & timer_flag)
                break;
        }
        timer_event[id].timer_cb(NULL);
        
        return (events ^ timer_flag);
    }

    return 0;
}

ble_timer_t timer_create(uint8_t type, ble_timer_cb timer_cb)
{
    uint8_t id;

    if((timer_flag & TIMER_FULL_FLAG) == TIMER_FULL_FLAG) {
        return NULL;
    }

    for(id = 0; id < TIMER_MAX_TASK; id++)
    {
        if((1 << id) & ~timer_flag)
            break;
    }
    
    timer_flag |= 1 << id;
    timer_event[id].type = type;
    timer_event[id].timer_cb = timer_cb;

    return (ble_timer_t)&timer_event[id];
}

ble_qiot_ret_status_t timer_start(ble_timer_t timer_id, uint32_t period)
{
    uint8_t id = ((timer_evt_t *)timer_id)->id;

    if(id & ~timer_flag)
        return BLE_QIOT_RS_ERR;

    if(BLE_TIMER_ONE_SHOT_TYPE == timer_event[id].type) {
        osal_start_timerEx(Timer_TaskID, 1 << id, period);
    } else if(BLE_TIMER_PERIOD_TYPE == timer_event[id].type) {
        osal_start_reload_timer(Timer_TaskID, 1 << id, period);
    } else {
        return BLE_QIOT_RS_ERR;
    }

    return BLE_QIOT_RS_OK;
}

ble_qiot_ret_status_t timer_stop(ble_timer_t timer_id)
{
    uint8_t id = ((timer_evt_t *)timer_id)->id;

    if(id & ~timer_flag)
        return BLE_QIOT_RS_ERR;

    osal_stop_timerEx(Timer_TaskID, 1 << id);

    return BLE_QIOT_RS_OK;
}

ble_qiot_ret_status_t timer_delete(ble_timer_t timer_id)
{
    uint8_t id = ((timer_evt_t *)timer_id)->id;

    if(id & ~timer_flag)
        return BLE_QIOT_RS_ERR;

    // VOID osal_memset(&timer_event[id], 0, sizeof(struct timer_evt));
    timer_flag &= ~(1 << id);

    return BLE_QIOT_RS_OK;
}
