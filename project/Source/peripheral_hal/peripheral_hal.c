/*
 * @Author: your name
 * @Date: 2022-01-29 10:53:03
 * @LastEditTime: 2022-02-01 09:13:07
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \phyplus_qcloud_at\project\Source\peripheral_hal\peripheral_hal.c
 */
#include "peripheral_hal.h"
uint8 Peripheral_TaskID;

int gpio_config(GPIO_Pin_e pin, Gpio_mode_t type)
{
    return hal_gpio_pin_init(pin, type);
}

void gpio_set_level(GPIO_Pin_e pin, uint32_t level)
{
    hal_gpio_write(pin, level);
}

static void Timer_set_rgb_blink(testCase)
{
    LOG("Task_timer_int_process:%d \r\n", testCase);

    switch (testCase)
    {
    case 0:
        led_set_rgb(255, 0, 0);
        break;
    case 1:
        led_set_rgb(0, 255, 0);
        break;
    case 2:
        led_set_rgb(0, 0, 255);
        break;
    default:
        break;
    }
}
/**
 * @brief 设置RGB灯珠轮流亮起来
 * 
 * @param red 红色
 * @param green 绿色
 * @param blue 蓝色
 */
void led_start_rgb_blink()
{
    LOG("led_start_rgb_blink \r\n");
    osal_start_reload_timer(Peripheral_TaskID, TIMER_500_MS_EVT, 300); //osal_start_timerEx
}

/**
 * @brief 停止RGB灯珠轮流亮起来
 * 
 * @param red 红色
 * @param green 绿色
 * @param blue 蓝色
 */
void led_stop_rgb_refresh()
{
    osal_stop_timerEx(Peripheral_TaskID, TIMER_500_MS_EVT);
}

void Peripheral_init(uint8 task_id, uint16 events)
{

    led_config();
    
    //led_start_rgb_blink();
}

uint16 Peripheral_ProcessEvent(uint8 task_id, uint16 events)
{
    VOID task_id; // OSAL required parameter that isn't used in this function
    static uint8_t s_testCase = 0;

    if (events & SYS_EVENT_MSG)
    {
        uint8 *pMsg;

        if ((pMsg = osal_msg_receive(Peripheral_TaskID)) != NULL)
        {
            // Release the OSAL message
            VOID osal_msg_deallocate(pMsg);
        }

        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if (events & RUN_PER_EVT)
    {
        Peripheral_init(task_id, events);

        // loop
        return (events ^ RUN_PER_EVT);
    }

    if (events & TIMER_500_MS_EVT)
    {
        if (s_testCase < 2)
            s_testCase++;
        else
            s_testCase = 0;

        Timer_set_rgb_blink(s_testCase);
        return (events ^ TIMER_500_MS_EVT);
    }

    return 0;
}

void Peripheral_Task(uint8 task_id)
{

    Peripheral_TaskID = task_id;
    // start task
    osal_set_event(Peripheral_TaskID, RUN_PER_EVT);
}
