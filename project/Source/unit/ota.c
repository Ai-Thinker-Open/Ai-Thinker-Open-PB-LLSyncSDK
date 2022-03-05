#include "ota.h"

#include "flash.h"
#include "ota_flash.h"
#include "log.h"
#include "error.h"

// define in scatter_load.sct
#define PART_NUM            2
#define FLASH_ADDR_PART_1   0x00000000
#define RUN_ADDR_PART_1     0x1fff0800
#define SIZE_PART_1         0x00000800
#define FLASH_ADDR_PART_2   0x00000800
#define RUN_ADDR_PART_2     0x1fff4800
#define SIZE_PART_2         0x00010000

static uint32_t ota_fw_addr;
static uint32_t ota_fw_size;

extern int ota_flash_read(uint32_t* dest, uint32_t addr, uint32_t size);

static uint16_t crc16_byte(uint16_t crc, uint8_t byte)
{
    static const uint16_t crc16_table[16] =
    {
        0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
        0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
    };

    uint16_t temp;

    // Compute checksum of lower four bits of a byte.
    temp         = crc16_table[crc & 0xF];
    crc  = (crc >> 4u) & 0x0FFFu;
    crc  = crc ^ temp ^ crc16_table[byte & 0xF];

    // Now compute checksum of upper four bits of a byte.
    temp         = crc16_table[crc & 0xF];
    crc  = (crc >> 4u) & 0x0FFFu;
    crc  = crc ^ temp ^ crc16_table[(byte >> 4u) & 0xF];

    return crc;
}

// read flash data and check crc16
static int crc16_from_flash(uint16_t *seed, uint32_t addr, uint32_t size)
{
    uint32_t val;

    if(addr % 4 || size % 4)
        return PPlus_ERR_DATA_ALIGN;

    while (size != 0)
    {
        ota_flash_read(&val, addr, 4);
        *seed = crc16_byte(*seed, (0x000000ff & val) >> 0);
        *seed = crc16_byte(*seed, (0x0000ff00 & val) >> 8);
        *seed = crc16_byte(*seed, (0x00ff0000 & val) >> 16);
        *seed = crc16_byte(*seed, (0xff000000 & val) >> 24);

        size -= 4;
        addr += 4;
    }

   return PPlus_SUCCESS;
}

// write application boot sector data
static int write_app_boot_sector(void)
{
    int ret;
    uint16_t crc16;
    uint32_t size;
    uint32_t bs[4];
    osal_memset(bs, 0, 16);

    flash_sector_erase(OTAF_2nd_BOOTINFO_ADDR);

    // write head
    bs[0] = PART_NUM;
    if(CFG_OTA_BANK_MODE == OTA_SINGLE_BANK)
        bs[1] = OTAF_SINGLE_BANK;
    else
        bs[1] = (ota_fw_addr == OTAF_APP_BANK_0_ADDR) ? OTAF_DUAL_BANK_0 : OTAF_DUAL_BANK_1;
    bs[2] = 0;
    bs[3] = 0xffffffff;
    ret = ota_flash_write_boot_sector(bs, 16, 0);
    if(ret)
        return ret;

    // write part1 record
    bs[0] = FLASH_ADDR_PART_1;
    bs[1] = RUN_ADDR_PART_1;
    size  = SIZE_PART_1;
    bs[2] = size;
    crc16 = 0;
    crc16_from_flash(&crc16, ota_fw_addr + FLASH_ADDR_PART_1, size);
    bs[3] = crc16;
    ret = ota_flash_write_boot_sector(bs, 16, 16*(1));
    if(ret)
        return ret;

    // write part2 record
    bs[0] = FLASH_ADDR_PART_2;
    bs[1] = RUN_ADDR_PART_2;
    size  = ota_fw_size - SIZE_PART_1;
    bs[2] = size;
    crc16 = 0;
    crc16_from_flash(&crc16, ota_fw_addr + FLASH_ADDR_PART_2, size);
    bs[3] = crc16;
    ret = ota_flash_write_boot_sector(bs, 16, 16*(2));
    if(ret)
        return ret;
    
    return PPlus_SUCCESS;
}

int ota_valid_file(uint32_t file_size)
{
    ota_fw_size = file_size;

    return PPlus_SUCCESS;
}

void ota_start(void)
{
    LOG("ota_fw_addr %x \n", ota_fw_addr);
    ota_flash_erase(ota_fw_addr);
}

void ota_finish(void)
{
    // write boot record
    write_app_boot_sector();
}

uint32_t ota_get_download_addr(void)
{
    uint32_t addr = 0;

    ota_flash_read_bootsector(&addr);
    ota_fw_addr = addr;

    return addr;
}

int ota_write_flash(uint32_t flash_addr, const char *write_buf, uint16_t write_len)
{
    if(write_len % 4)
        return PPlus_ERR_DATA_ALIGN;
    return ota_flash_write_partition(flash_addr,(uint32_t*)write_buf, write_len);
}

int ota_read_flash(uint32_t flash_addr, char *read_buf, uint16_t read_len)
{
    if(read_len % 4)
        return PPlus_ERR_DATA_ALIGN;
    
    //extern int ota_flash_read();
    return ota_flash_read((uint32_t*)read_buf, flash_addr, read_len);
}
