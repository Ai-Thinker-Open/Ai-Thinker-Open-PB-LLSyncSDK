#ifndef __AT_CMD_H__
#define __AT_CMD_H__

#include "stdbool.h"
#include "stdint.h"

#include "at_ringbuff.h"

#define CLI_CMD_SIZE  128
#define CLI_MAX_ARGS  16
#define AT_PRINT_SIZE 128

#define AT_OK    (0)  /* There is no error		*/
#define AT_ERROR (-1) /* A generic error happens	*/

#define respond_error() at_printf("ERROR\r\n")
#define respond_ok()    at_printf("OK\r\n");

typedef void (*fp_at_through)(char* data, uint32_t len);

typedef enum {
    AT_TYPE_EXECUTE,
    AT_TYPE_SET,
    AT_TYPE_QUERY,
} at_cmd_type_t;

typedef enum {
    /* normal mode */
    AT_STATUS_NORMAL,
    /* transparent transport mode*/
    AT_TYPE_TRANS,
} at_cmd_status_t;

typedef int (*fp_at_callback)(at_cmd_type_t type, int argc, char* argv[]);
typedef struct {
    const char*    cmd;
    const char*    help;
    fp_at_callback callback;
} at_cmd_t;

typedef struct {
    int             echo;
    at_cmd_status_t status;
    fp_at_through   at_through_cb;
    at_cmd_t*       table;
    uint32_t        table_size;
} at_cmd_handle_t;

void at_echo_enable(int enable);
void at_receive(char* buf, uint32_t size);
int  at_receive_timeout(char* buf, uint32_t size, uint32_t timeout);
int  at_receive_until_timeout(char* buf, uint32_t size, char* flag, uint32_t timeout);
void at_task(void);
int  at_write_data(const char* data, uint32_t len);
int  at_printf(char* format, ...);
void at_init(at_cmd_t* table, uint32_t table_size, char* buf, uint32_t buffer_len);
///////////////////////hal interface///////////////////////
/**
 * @brief   AT端口数据写入
 *
 * @return  实际写入数据长度
 */
int _at_write_data(const char* data, uint16_t len);
/**
 * @brief   AT端口收数据时调用此函数
 *
 * @return  实际写入数据长度
 */
void _at_recv(char* data, uint16_t len);
#if 0
/**
 * @brief   AT获取时间标记
 *
 * @return  时间标记
 */
uint32_t _at_timemark(void);
/**
 * @brief   AT超时检测(ms)
 *
 * @return  0:未超时 other:超时
 */
int _at_timeout(uint32_t timemark, uint32_t timeout);
#endif

#endif  // __AT_CMD_H__
