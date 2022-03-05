#include "at_cmd.h"
#include "at_ringbuff.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define CLI_STR_COMPARE(s0, s1) strcmp((const char *)(s0), (const char *)(s1))

#define CLI_IS_WHITE_SPACE(ch)   ((' ' == (ch)) || ('\t' == (ch)))
#define CLI_IS_CMD_SEPARATOR(ch) ((',' == (ch)) || ('\r' == (ch)) || ('\n' == (ch)))

static ring_buff_t     at_buff;
static at_cmd_handle_t at_handle;

static uint16_t at_cmd_parse(char *buffer, int buffer_len, at_cmd_t *cmd_list, int cmd_count)
{
    uint32_t argcLimit = 0;
    uint32_t argc;
    char *   argv[CLI_MAX_ARGS];
    char *   cmd;
    int      index;

    at_cmd_type_t option = AT_TYPE_EXECUTE;

    if (NULL == (buffer) || 0 == buffer_len) {
        return 0xffff;
    }

    // for (; CLI_IS_WHITE_SPACE(*buffer) && (0 != buffer_len); buffer++, buffer_len--);

    argc      = 0;
    argcLimit = CLI_MAX_ARGS;

    //		 AIT_printf(
    //		"[CLI] Command %s, Number of arguments %d buffer_len:%d\n", buffer, argc,buffer_len);
    for (cmd = buffer + 1; cmd < (buffer + buffer_len); cmd++) {
        if ('\0' == (*(cmd - 1))) {
            argv[argc++] = cmd;
            if (argcLimit == argc) {
                argcLimit = 0;
                break;
            }
        } else if (CLI_IS_CMD_SEPARATOR(*cmd)) {
            *cmd = '\0';
        } else if ('=' == *(cmd)) {
            *(cmd) = '\0';
            option = AT_TYPE_SET;
        } else if ('?' == *(cmd) && '\0' == *(cmd + 1) /* && '\n' == *(cmd + 2) */) {
            *(cmd) = '\0';
            option = AT_TYPE_QUERY;
        } else {
            /* Nothing to do */
        }
    }

    argc -= 1;

    /* Identified command name */
    cmd = buffer;

    /* Search command and call associated callback */
    for (index = 0; index < cmd_count; index++) {
        if (0 == CLI_STR_COMPARE(buffer, cmd_list[index].cmd))
        // if (strstr(cmd_list[index].cmd,buffer)!=NULL)
        {
            uint16_t ret = cmd_list[index].callback(option, argc, argv);
            if (ret == AT_OK) {
                respond_ok();
            } else {
                respond_error();
            }

            goto exit;
        }
    }

    // if no found
    respond_error();

exit:
    return 0;
}

void at_echo_enable(int enable)
{
    at_handle.echo = enable;
}

/**
 * @brief 从at接口接收指定长度数据
 *
 */
void at_receive(char *buf, uint32_t size)
{
    uint32_t len    = 0;
    uint32_t n_read = 0;

    while (n_read < size) {
        len = ring_buff_get_size(&at_buff);
        if (0 == len) {
            continue;
        }

        len = len < (size - n_read) ? len : size - n_read;

        ring_buff_pop_data(&at_buff, (buf + n_read), len);
        n_read += len;
    }
}

#if 0
/**
 * @brief  从at接口接收指定长度数据-带超时
 *
 * @note   请在一个循环中调用此函数
 *
 * @return 收到数据长度
 */
int at_receive_timeout(char *buf, uint32_t size, uint32_t timeout)
{
    uint32_t len    = 0;
    uint32_t n_read = 0;

    uint32_t timemask = _at_timemark();

    while (n_read < size && 0 == _at_timeout(timemask, timeout)) {
        len = ring_buff_get_size(&at_buff);
        if (0 == len) {
            continue;
        }

        len = len < (size - n_read) ? len : size - n_read;

        ring_buff_pop_data(&at_buff, (buf + n_read), len);
        n_read += len;
    }

    return n_read;
}

/**
 * @brief  从at接口接收指定长度数据-带超时&带终止符
 *
 * @note   请在一个循环中调用此函数
 *
 * @return 收到数据长度
 */
int at_receive_until_timeout(char *buf, uint32_t size, char *flag, uint32_t timeout)
{
    uint32_t len    = 0;
    uint32_t n_read = 0;

    uint32_t timemask = _at_timemark();

    memset(buf, 0, size);
    while (n_read < size && 0 == _at_timeout(timemask, timeout)) {
        len = ring_buff_get_size(&at_buff);
        if (0 == len) {
            continue;
        }

        ring_buff_pop_data(&at_buff, (buf + n_read), 1);
        n_read++;
        if (NULL != strstr(buf, flag)) {
            return n_read;
        }
    }

    return 0;
}
#endif

/**
 * @brief
 *
 * @note  请在一个循环中调用此函数
 */
void at_task(void)
{
    static char cmdstr[CLI_CMD_SIZE];
    static char cmdlen = 0;

    while (1) {
        if (ring_buff_get_size(&at_buff) <= 0) {
            break;
        }

        if (cmdlen + 1 > CLI_CMD_SIZE) {
            cmdlen = 0;
            break;
        }

        ring_buff_pop_data(&at_buff, (cmdstr + cmdlen), 1);
        cmdlen += 1;

        if (cmdlen >= 4 && ('\r' == cmdstr[cmdlen - 2]) && ('\n' == cmdstr[cmdlen - 1])) {
            cmdstr[cmdlen - 2] = '\0';

            if (at_handle.echo == 1) {
                at_printf(cmdstr);
                at_printf("\r\n");
            }

            at_cmd_parse(cmdstr, cmdlen, at_handle.table, at_handle.table_size);
            cmdlen = 0;
        }
    }
}

/**
 * @brief   向AT端口写入数据
 */
int at_write_data(const char *data, uint32_t len)
{
    return _at_write_data(data, len);
}

int at_printf(char *format, ...)
{
    char    buf[AT_PRINT_SIZE];
    va_list args;
    int     ret;

    va_start(args, format);
    ret = vsprintf(buf, format, args);
    va_end(args);

    _at_write_data(buf, strlen(buf));

    return ret;
}

/**
 * @brief   AT初始化
 *
 * @param   table      指令表
 *
 * @param   table_size 指令表大小
 *
 * @param   buf        环形缓冲区
 *
 * @param   buffer_len 缓冲区大小
 *
 * @return  none
 */
void at_init(at_cmd_t *table, uint32_t table_size, char *buf, uint32_t buffer_len)
{
    at_handle.status        = AT_STATUS_NORMAL;
    at_handle.echo          = false;
    at_handle.at_through_cb = NULL;
    at_handle.table         = table;
    at_handle.table_size    = table_size;

    ring_buff_init(&at_buff, buf, buffer_len);
}

///////////////////////hal interface///////////////////////

/**
 * @brief   AT端口收到数据时调用此函数
 */
void _at_recv(char *data, uint16_t len)
{
    ring_buff_push_data(&at_buff, data, len);
}
