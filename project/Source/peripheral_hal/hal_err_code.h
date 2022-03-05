

#ifndef _HAL_ERROR_H
#define _HAL_ERROR_H


typedef int hal_err_t;

#define HAL_SUCCESS                         (0)  /*Success*/
#define HAL_ERR_FATAL                       (1)  /*unrecoverable error*/
#define HAL_ERR_INTERNAL                    (2)  /*Internal Error*/
#define HAL_ERR_NO_MEM                      (3)  /*No Memory for operation*/
#define HAL_ERR_NOT_FOUND                   (4)  /*Not found*/
#define HAL_ERR_NOT_SUPPORTED               (5)  /*Not supported*/
#define HAL_ERR_INVALID_PARAM               (6)  /*Invalid Parameter*/
#define HAL_ERR_INVALID_STATE               (7)  /*Invalid state, operation disallowed in this state*/
#define HAL_ERR_INVALID_LENGTH              (8)  /*Invalid Length*/
#define HAL_ERR_INVALID_FLAGS               (9)  /*Invalid Flags*/
#define HAL_ERR_INVALID_DATA                (10) /*Invalid Data*/
#define HAL_ERR_DATA_SIZE                   (11) /*Data size exceeds limit*/
#define HAL_ERR_DATA_ALIGN                  (12) /*Data alignment is not correct*/
#define HAL_ERR_TIMEOUT                     (13) /*Operation timed out*/
#define HAL_ERR_NULL                        (14) /*Null Pointer*/
#define HAL_ERR_FORBIDDEN                   (15) /*Forbidden Operation*/
#define HAL_ERR_INVALID_ADDR                (16) /*Bad Memory Address*/
#define HAL_ERR_BUSY                        (17) /*Busy*/
#define HAL_ERR_NOT_REGISTED                (18) /*not registed*/
#define HAL_ERR_IO_CONFILCT                 (19) /*IO config error*/
#define HAL_ERR_IO_FAIL                 		(20) /*IO fail error*/
#define HAL_ERR_NOT_IMPLEMENTED             (22) /*Function is not provide now*/
#define HAL_ERR_SPI_FLASH                   (23) /*spi falsh operation error*/
#define HAL_ERR_FS_WRITE_FAILED       			(31)
#define HAL_ERR_FS_CONTEXT     							(32)
#define HAL_ERR_FS_FULL              				(33)
#define HAL_ERR_FS_PARAMETER   							(34)
#define HAL_ERR_FS_NOT_ENOUGH_SIZE   				(35)
#define HAL_ERR_FS_EXIST_SAME_ID     				(36)
#define HAL_ERR_FS_NOT_FIND_ID       				(37)
#define HAL_ERR_FS_BUFFER_TOO_SMALL  				(38)
#define HAL_ERR_FS_UNINITIALIZED     				(39)
#define HAL_ERR_FS_HAVE_INITED              (40)
#define HAL_ERR_FS_IN_INT                   (41)
#define HAL_ERR_FS_RESERVED_ERROR    				(42)
#define HAL_ERR_VERSION                     (43)
#define HAL_ERR_NO_DEV                      (44)


#define HAL_ERR_BLE_NOT_READY               (80) /*BLE not ready error*/
#define HAL_ERR_BLE_BUSY                    (81) /*BLE operation failed becuase of busy*/
#define HAL_ERR_BLE_FAIL                    (82) /*BLE operation failed*/

#define HAL_ERR_OTA_INVALID_STATE           (100) /*state machine error when OTA*/
#define HAL_ERR_OTA_DATA_SIZE               (101) /*data size is not correct*/
#define HAL_ERR_OTA_CRC                     (102) /*bad checksum*/
#define HAL_ERR_OTA_NO_APP                  (103) /*No application data*/
#define HAL_ERR_OTA_BAD_DATA                (104) /*bad application data*/
#define HAL_ERR_OTA_UNKNOW_CMD              (105) /*unknow command*/
#define HAL_ERR_OTA_CRYPTO                  (106) /*crypto verify error*/

#endif

