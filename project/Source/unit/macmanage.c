#include "macmanage.h"

#include "flash.h"
#include "log.h"

void read_mac(uint8_t *mac)
{
    uint32 address = VENDOR_PRODUCT_MAC_ADDR;

    mac[2] = (uint8_t)ReadFlash(address++);
    mac[3] = (uint8_t)ReadFlash(address++);
    mac[4] = (uint8_t)ReadFlash(address++);
    mac[5] = (uint8_t)ReadFlash(address++);

    mac[0] = (uint8_t)ReadFlash(address++);
    mac[1] = (uint8_t)ReadFlash(address);
}

void write_mac(uint8_t *mac)
{
    uint32 address = VENDOR_PRODUCT_MAC_ADDR;
    
    flash_sector_erase(VENDOR_PRODUCT_MAC_ADDR);
    WriteFlashShort(address, *(uint16_t *)(mac + 2));
    address += 2;
    WriteFlashShort(address, *(uint16_t *)(mac + 4));
    address += 2;
    WriteFlashShort(address, *(uint16_t *)(mac + 0));
}

void gen_rand_mac(uint8_t *mac)
{
    *(uint16 *)mac = osal_rand();
    mac += 2;
    *(uint16 *)mac = osal_rand();
    mac += 2;
    *(uint16 *)mac = osal_rand();
}

bool is_mac_exist(uint8_t *mac)
{
    return *(uint32 *)mac == 0xffffffff && *(uint16 *)(mac + 4) == 0xffff ? false : true;
}
