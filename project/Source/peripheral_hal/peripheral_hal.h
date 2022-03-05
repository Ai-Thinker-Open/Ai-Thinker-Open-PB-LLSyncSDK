/*
 * @Author: your name
 * @Date: 2022-01-24 10:00:06
 * @LastEditTime: 2022-02-01 09:19:48
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \phyplus_qcloud_at\project\Source\peripheral_hal\peripheral_hal.h
 */

#ifndef __AI_THINKER_XXAA_HAL_H
#define __AI_THINKER_XXAA_HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "string.h"
#include "ap_cp.h"
#include "clock.h"
#include "pwrmgr.h"
#include "gpio.h"
#include "error.h"
#include "log.h"
#include "OSAL.h"
#include "qcloud_cmd.h"
#include "qcloud_hal.h"
#include "at_cmd.h"
#include "at_utils.h"
#include "hal_led_light.h"
#include "pwm.h"




#define RUN_PER_EVT 0x0001
#define RUN_PER_LIGHT 0x0002
#define RUN_PER_START_DEVICE_EVT 0x0003
#define TIMER_500_MS_EVT 0x0004


    typedef enum
    {
        GPIO_MODE_INPUT = IE,   /*!< GPIO mode : input only                           */
        GPIO_MODE_OUTPUT = OEN, /*!< GPIO mode : output only mode                     */
    } Gpio_mode_t;

    void Peripheral_Task(uint8 task_id);
    uint16 Peripheral_ProcessEvent(uint8 task_id, uint16 events);

    int gpio_config(GPIO_Pin_e pin, Gpio_mode_t type);
    void gpio_set_level(GPIO_Pin_e pin, uint32_t level);

    /**
 * @brief 设置RGB灯珠轮流亮起来
 * 
 * @param red 红色
 * @param green 绿色
 * @param blue 蓝色
 */
    void led_start_rgb_blink();

    /**
 * @brief 停止RGB灯珠轮流亮起来
 * 
 * @param red 红色
 * @param green 绿色
 * @param blue 蓝色
 */
    void led_stop_rgb_blink();

#ifdef __cplusplus
}
#endif

#endif
