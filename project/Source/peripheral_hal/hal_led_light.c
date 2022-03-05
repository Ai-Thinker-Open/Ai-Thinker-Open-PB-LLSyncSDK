/*
 * @Author: xuhong��������ࣩ
 * @Date: 2022-01-29 11:53:20
 * @LastEditTime: 2022-02-01 13:50:28
 * @LastEditors: Please set LastEditors
 * @Description: ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \phyplus_qcloud_at\project\Source\peripheral_hal\hal_led_light.c
 */
#include "hal_led_light.h"
#include "pwm.h"
#include "OSAL.h"
#include "gpio.h"
#include "peripheral_hal.h"

uint32_t max_pwm_channle_value = 0, max_pwm_channle = 0;
// pwm pin number
uint32_t pwm_pin_number[6] = {0, 0, 0, 0, 0, 0};
// duties table, can not over max_pwm_channle_value
uint32_t pwm_duties[6] = {0, 0, 0, 0, 0, 0};

hal_err_t pwm_init(uint32_t max_value, uint32_t *duties, uint8_t channel_num, const uint32_t *pin_num)
{
    uint8_t i;
    max_pwm_channle_value = max_value;
    max_pwm_channle = channel_num;
    for (i = 0; i < channel_num; i++)
    {
        HAL_PWM_LOG("[%d] duties = %d , pin_num=%d \n", i, duties[i], pin_num[i]);

        if (duties[i] > 0)
        {
            //ȫ�ֱ�������
            pwm_pin_number[i] = pin_num[i];
            pwm_duties[i] = duties[i];

            //PWM ��ʼ������������ʱ�ӣ�����ͨ�������÷�Ƶ����������ģʽ������
            hal_pwm_init(i, PWM_CLK_DIV_64, PWM_CNT_UP, PWM_POLARITY_FALLING);
            //���ü��������ݣ��������� PWM ռ�ձȡ�
            hal_pwm_set_count_val(i, duties[i], max_value);
            //ʹ�� PWM ͨ������ IO pin��
            hal_pwm_open_channel(i, pwm_pin_number[i]);
            //��ʼ
            hal_pwm_start();
        }
        else
        {
            //ȫ�ֱ�������
            pwm_pin_number[i] = pin_num[i];
            pwm_duties[i] = 0;

            hal_pwm_close_channel(i);
            hal_pwm_destroy(i);
            hal_pwm_stop();

            hal_gpio_pin_init(pin_num[i], OEN);
            hal_gpio_write(pin_num[i], 0);
        }
    }

    return HAL_SUCCESS;
}

hal_err_t pwm_set_duty(uint8_t channel_num, uint32_t duty)
{

    if (channel_num > max_pwm_channle)
    {
        return HAL_ERR_FATAL;
    }

    if (duty == max_pwm_channle_value)
    {
        duty = duty - 1;
    }

    pwm_duties[channel_num] = duty;

    return pwm_init(max_pwm_channle_value, pwm_duties, max_pwm_channle, pwm_pin_number);
}

//****************  Application start **************************//

//pin number chennle
#define CHANNLE_RED_PIN 0
#define CHANNLE_GREEN_PIN 1
#define CHANNLE_BLUE_PIN 2

#define AI_THINKER_PB_02_KIT_RED_PIN (P23)
#define AI_THINKER_PB_02_KIT_GREEN_PIN (P14)
#define AI_THINKER_PB_02_KIT_BLUE_PIN (P18)
#define AI_THINKER_PB_02_KIT_CW_PIN (P34)
#define AI_THINKER_PB_02_KIT_WW_PIN (P0)

#define MAX_CHANNLE_VALUE (256)
#define TURN_OFF_CHANNLE_VALUE (0)
#define TURN_ON_CHANNLE_VALUE (MAX_CHANNLE_VALUE - 1)

//timer

uint8 LED_TaskID = 0;
#define TIMER_1000_MS_EVT 0x0001

// �¼�ID
#define LED_ONCE_TIMER_EVT 0x0001
#define LED_CYCLE_TIMER_EVT 0x0002
#define LED_RED_BLINK_WIFI_CONNECTING_EVT 0x0004
#define LED_BLUE_ON_WIFI_OK_EVT 0x0008
#define LED_RED_ON_WIFI_ERR_EVT 0x0010

hal_err_t led_config()
{

    // pwm pin number
    const uint32_t pin_num[3] = {
        AI_THINKER_PB_02_KIT_RED_PIN,
        AI_THINKER_PB_02_KIT_GREEN_PIN,
        AI_THINKER_PB_02_KIT_BLUE_PIN,
    };

    // duties table, can not over MAX_CHANNLE_VALUE
    uint32_t duties[3] = {
        TURN_OFF_CHANNLE_VALUE,
        TURN_OFF_CHANNLE_VALUE,
        TURN_OFF_CHANNLE_VALUE,
    };

    pwm_init(MAX_CHANNLE_VALUE, duties, 3, pin_num);

    return HAL_SUCCESS;
}

void led_set_rgb(uint32_t red, uint32_t green, uint32_t blue)
{

    // pwm pin number
    const uint32_t pin_num[3] = {
        AI_THINKER_PB_02_KIT_RED_PIN,
        AI_THINKER_PB_02_KIT_GREEN_PIN,
        AI_THINKER_PB_02_KIT_BLUE_PIN,
    };

    // duties table, can not over MAX_CHANNLE_VALUE
    uint32_t duties[3] = {
        red,
        green,
        blue,
    };

    pwm_init(MAX_CHANNLE_VALUE, duties, 3, pin_num);
}

