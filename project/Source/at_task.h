/*
 * @Author: your name
 * @Date: 2022-01-23 10:19:46
 * @LastEditTime: 2022-02-01 09:06:23
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \phyplus_qcloud_at\project\Source\at_task.h
 */
#ifndef AT_TASK_H
#define AT_TASK_H

#define RUN_AT_EVT                              0x0001

extern void ATTask_Init( uint8 task_id );

extern uint16 ATTask_ProcessEvent( uint8 task_id, uint16 events );

void at_task_run(void);


#endif /* AT_TASK_H */
