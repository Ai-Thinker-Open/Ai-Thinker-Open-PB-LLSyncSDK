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
#include "log.h"
#include "error.h"

#include "at_task.h"

#include "qcloud_cmd.h"
#include "at_cmd.h"

uint8 AT_TaskID;

void ATTask_Init(uint8 task_id)
{
    AT_TaskID = task_id;

    qcloud_at_init();

    // start at task 
    osal_set_event(AT_TaskID, RUN_AT_EVT);
}

uint16 ATTask_ProcessEvent(uint8 task_id, uint16 events)
{
    VOID task_id; // OSAL required parameter that isn't used in this function
    //LOG("Qcloud_ProcessEvent: 0x%x\n", events);

    if (events & SYS_EVENT_MSG)
    {
        uint8 *pMsg;

        if ((pMsg = osal_msg_receive(AT_TaskID)) != NULL)
        {
            // Release the OSAL message
            VOID osal_msg_deallocate(pMsg);
        }

        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if (events & RUN_AT_EVT)
    {
        at_task();
        
        // loop
        return (events ^ RUN_AT_EVT);
    }
    

    return 0;
}

void at_task_run(void)
{
    osal_set_event(AT_TaskID, RUN_AT_EVT);
}
