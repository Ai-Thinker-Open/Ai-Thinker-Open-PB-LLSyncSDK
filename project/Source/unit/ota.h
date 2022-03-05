#ifndef OTA_H
#define OTA_H

#include "OSAL.h"


int ota_valid_file(uint32_t file_size);

void ota_start(void);

void ota_finish(void);

uint32_t ota_get_download_addr(void);

int ota_write_flash(uint32_t flash_addr, const char *write_buf, uint16_t write_len);

int ota_read_flash(uint32_t flash_addr, char *read_buf, uint16_t read_len);

#endif /* OTA_H */
