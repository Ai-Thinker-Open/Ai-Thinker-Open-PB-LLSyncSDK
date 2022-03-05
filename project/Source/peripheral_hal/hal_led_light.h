/*
 * @Author: your name
 * @Date: 2022-01-29 11:45:46
 * @LastEditTime: 2022-02-01 09:07:00
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \phyplus_qcloud_at\project\Source\peripheral_hal\hal_led_light.h
 */

#ifndef _LED_LIGHT_H
#define _LED_LIGHT_H

#include "types.h"
#include "hal_err_code.h"
#include "log.h"
#include "pwrmgr.h"
#include "ap_timer.h"

#define LOG_HAL_LIGHT_PWM 0

#if (LOG_HAL_LIGHT_PWM == 1)
#define HAL_PWM_LOG(...) phy_printf(##__VA_ARGS__)
#else
#define HAL_PWM_LOG(...)
#endif // LOG_HAL_LIGHT_PWM

/**
 * @brief LED灯设置，默认是安信可PB-02-Kit 管脚开发板
 * 
 * @return hal_err_t 返回数值
 */
hal_err_t led_config();

/**
 * @brief 设置RGB灯珠
 * 
 * @param red 红色
 * @param green 绿色
 * @param blue 蓝色
 */
void led_set_rgb(uint32_t red, uint32_t green, uint32_t blue);

/**
  * @brief  PWM function initialization, including GPIO, frequency and duty cycle.
  *
  * @param  max_value
  * @param  duties duty cycle of each channels.
  * @param  channel_num PWM channel number, maximum is 8
  * @param  pin_num GPIO number of PWM channel
  *
  * @return
  *     - HAL_SUCCESS Success
  *     - HAL_ERR_FATAL Parameter error
  */
hal_err_t pwm_init(uint32_t max_value, uint32_t *duties, uint8_t channel_num, const uint32_t *pin_num);

/**
  * @brief   Set the duty cycle of a PWM channel.
  *          Set the time that high level or low(if you invert the output of this channel)
  *          signal will last, the duty cycle cannot exceed the period.
  *
  * @note    After set configuration, pwm_start needs to be called to take effect.
  *
  * @param   channel_num PWM channel number
  *          the channel_num cannot exceed the value initialized by pwm_init.
  * @param   duty duty cycle
  *
  * @return
  *     - HAL_SUCCESS Success
  *     - HAL_ERR_FATAL Parameter error
  */
hal_err_t pwm_set_duty(uint8_t channel_num, uint32_t duty);

#endif
