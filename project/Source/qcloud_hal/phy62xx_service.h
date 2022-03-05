#ifndef PHY62XX_SERVICE_H
#define PHY62XX_SERVICE_H

#include <OSAL.h>

#ifndef VERIFY_SUCCESS
    #define VERIFY_SUCCESS(statement)                       \
    do                                                      \
    {                                                       \
        uint32_t _err_code = (uint32_t) (statement);        \
        if (_err_code != PPlus_SUCCESS)                     \
        {                                                   \
            return _err_code;                               \
        }                                                   \
    } while(0)
#endif // VERIFY_SUCCESS

typedef struct
{
    uint16_t    uuid_cnt;     /**< Number of UUID entries. */
    uint16_t   *uuids;        /**< Pointer to UUID array entries. */
} ble_advdata_uuid_list_t;

typedef struct
{
    uint16_t                     company_identifier;
    uint8_t                     *data;
    uint16_t                     size;
} ble_advdata_manuf_data_t;

typedef struct
{
    uint8_t                     *name;
    uint16_t                     size;
} ble_advdata_name_t;

typedef struct
{
    uint8_t                      flags;                    /**< Advertising data Flags field. */
    ble_advdata_uuid_list_t      uuid;                     /**< List of UUIDs in the 'Complete' list. */
    ble_advdata_manuf_data_t    *manuf_specific_data;      /**< Manufacturer specific data. */
    ble_advdata_name_t          *devname;                     /**< device name. */
} ble_advdata_t;

uint32_t ble_advdata_encode(ble_advdata_t *p_advdata,
                            uint8_t       *p_encoded_data,
                            uint16_t      *p_len);


#endif  // PHY62XX_SERVICE_H
