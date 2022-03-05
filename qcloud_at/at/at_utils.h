/*
 * @Author: your name
 * @Date: 2022-01-23 10:19:46
 * @LastEditTime: 2022-02-01 13:42:38
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \phyplus_qcloud_at\qcloud_at\at\at_utils.h
 */
#ifndef __AT_UTILS_H__
#define __AT_UTILS_H__

void str2bytes(const char *in, unsigned char *out, int len);
int  strsplit(char *in, char **out, const char *delim, int limit);


#endif
