/*
 * @Author: your name
 * @Date: 2022-01-23 10:19:46
 * @LastEditTime: 2022-02-01 13:43:19
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \phyplus_qcloud_at\qcloud_at\at\at_utils.c
 */
#include "at_utils.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

void str2bytes(const char *in, unsigned char *out, int len)
{
    char          h1, h2;
    unsigned char s1, s2;
    int           i;

    for (i = 0; i < len; i++) {
        h1 = in[2 * i];
        h2 = in[2 * i + 1];

        s1 = toupper(h1) - 0x30;
        if (s1 > 9)
            s1 -= 7;

        s2 = toupper(h2) - 0x30;
        if (s2 > 9)
            s2 -= 7;

        out[i] = s1 * 16 + s2;
    }
}

/**
 * @brief   split string
 *
 * @param   in: input string
 *
 * @param   out: output string
 *
 * @param   delim: delimiters
 *
 * @param   limit: Maximum number of split
 *
 * @note    sizeof(out) equals (limit + 1)
 *
 * @return  actual number of split
 */
int strsplit(char *in, char **out, const char *delim, int limit)
{
    int   cnt       = 0;
    int   delim_len = strlen(delim);
    char *pos       = NULL;

    if (NULL == in && 0 == limit) {
        return 0;
    }

    out[cnt] = in;
    while (cnt < limit) {
        if (NULL == (pos = strstr(out[cnt], delim))) {
            break;
        }

        *pos = '\0';
        pos += delim_len;
        cnt++;
        out[cnt] = pos;
    };

    return cnt;
}

void ByteToHexStr(const char *a, int len)
{
    
}
