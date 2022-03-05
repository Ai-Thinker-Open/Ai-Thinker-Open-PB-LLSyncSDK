#ifndef MACMANAGE_H
#define MACMANAGE_H

#include "OSAL.h"

#define VENDOR_PRODUCT_ID_ADDR          0x4030
#define VENDOR_PRODUCT_SECRERT_ADDR     0x4010
#define VENDOR_PRODUCT_MAC_ADDR         0x4000


void read_mac(uint8_t *mac);

void write_mac(uint8_t *mac);

void gen_rand_mac(uint8_t *mac);

bool is_mac_exist(uint8_t *mac);



#endif /* MACMANAGE_H */
