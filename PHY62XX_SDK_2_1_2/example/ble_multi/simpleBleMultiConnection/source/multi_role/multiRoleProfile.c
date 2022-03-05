/**************************************************************************************************
 
  Phyplus Microelectronics Limited confidential and proprietary. 
  All rights reserved.

  IMPORTANT: All rights of this software belong to Phyplus Microelectronics 
  Limited ("Phyplus"). Your use of this Software is limited to those 
  specific rights granted under  the terms of the business contract, the 
  confidential agreement, the non-disclosure agreement and any other forms 
  of agreements as a customer or a partner of Phyplus. You may not use this 
  Software unless you agree to abide by the terms of these agreements. 
  You acknowledge that the Software may not be modified, copied, 
  distributed or disclosed unless embedded on a Phyplus Bluetooth Low Energy 
  (BLE) integrated circuit, either as a product or is integrated into your 
  products.  Other than for the aforementioned purposes, you may not use, 
  reproduce, copy, prepare derivative works of, modify, distribute, perform, 
  display or sell this Software and/or its documentation for any purposes.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  PHYPLUS OR ITS SUBSIDIARIES BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
  
**************************************************************************************************/

/**************************************************************************************************
  Filename:       multiRoleProfile.c
  Revised:         
  Revision:       

  Description:    This file contains the Simple GATT profile sample GATT service 
                  profile for use with the BLE sample application.

 
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "multiRoleProfile.h"
#include "log.h"

#include "multi.h"


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED        8

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0xFFF0
CONST uint8 multiProfileServUUID[ATT_UUID_SIZE] =
{ 
//  LO_UINT16(MULTIPROFILE_SERV_UUID), HI_UINT16(MULTIPROFILE_SERV_UUID)
	0x6E, 0x40, 0x00, 0x01, 0xB5, 0xA3, 0xF3, 0x93, 0xE0, 0xA9, 0xE5, 0x0E, 0x24, 0xDC, 0x41,0x79

};

// Characteristic 1 UUID: 0xFFF1
CONST uint8 multiProfilechar1UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MULTIPROFILE_CHAR1_UUID), HI_UINT16(MULTIPROFILE_CHAR1_UUID)
};

// Characteristic 2 UUID: 0xFFF2
CONST uint8 multiProfilechar2UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MULTIPROFILE_CHAR2_UUID), HI_UINT16(MULTIPROFILE_CHAR2_UUID)
};

// Characteristic 3 UUID: 0xFFF3
CONST uint8 multiProfilechar3UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MULTIPROFILE_CHAR3_UUID), HI_UINT16(MULTIPROFILE_CHAR3_UUID)
};

// Characteristic 4 UUID: 0xFFF4
CONST uint8 multiProfilechar4UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MULTIPROFILE_CHAR4_UUID), HI_UINT16(MULTIPROFILE_CHAR4_UUID)
};

// Characteristic 5 UUID: 0xFFF5
CONST uint8 multiProfilechar5UUID[ATT_UUID_SIZE] =
{ 
//  LO_UINT16(MULTIPROFILE_CHAR5_UUID), HI_UINT16(MULTIPROFILE_CHAR5_UUID)
//	0x6E, 0x40, 0x00, 0x02, 0xB5, 0xA3, 0xF3, 0x93, 0xE0, 0xA9, 0xE5, 0x0E, 0x24, 0xDC, 0x41,0x79
    0x79, 0x41, 0xDC, 0x24, 0x0E, 0xE5, 0xa9, 0xe0, 0x93, 0xf3, 0xa3, 0xb5, 0x02, 0x00, 0x40,0x6e

};

// Characteristic 6 UUID: 0xFFF6
CONST uint8 multiProfilechar6UUID[ATT_UUID_SIZE] =
{ 
//  LO_UINT16(MULTIPROFILE_CHAR6_UUID), HI_UINT16(MULTIPROFILE_CHAR6_UUID)
//0x6E, 0x40, 0x00, 0x03, 0xB5, 0xA3, 0xF3, 0x93, 0xE0, 0xA9, 0xE5, 0x0E, 0x24, 0xDC, 0x41,0x79
    0x79, 0x41, 0xDC, 0x24, 0x0E, 0xE5, 0xa9, 0xe0, 0x93, 0xf3, 0xa3, 0xb5, 0x03, 0x00, 0x40,0x6e

};

// Characteristic 7 UUID: 0xFFF7
CONST uint8 multiProfilechar7UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MULTIPROFILE_CHAR7_UUID), HI_UINT16(MULTIPROFILE_CHAR7_UUID)
};


/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static multiProfileCBs_t *multiProfile_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// multi Profile Service attribute
static CONST gattAttrType_t multiProfileService = { ATT_UUID_SIZE, multiProfileServUUID };

// multi Profile Characteristic 5 Properties
static uint8 multiProfileChar5Props = GATT_PROP_READ | GATT_PROP_WRITE;//GATT_PROP_WRITE_NO_RSP;   // to change to write only, HZF

// Characteristic 5 Value
static uint8 multiProfileChar5[MULTI_ATT_LONG_PKT];

// multi Profile Characteristic 5 User Description
static uint8 multiProfileChar5UserDesp[] = "Reset\0";

// multi Profile Characteristic 6 Properties
static uint8 multiProfileChar6Props = GATT_PROP_READ | GATT_PROP_NOTIFY;   

// Characteristic 6 Value
static uint8 multiProfileChar6[MULTI_ATT_LONG_PKT];
static uint16 multiChar6Notifylen = 0;

// multi Profile Characteristic 6 User Description
static uint8 multiProfileChar6UserDesp[] = "NOTIFY\0";
// multi Profile Characteristic 6 Configuration Each client has its own
// instantiation of the Client Characteristic Configuration. Reads of the
// Client Characteristic Configuration only shows the configuration for
// that client and writes only affect the configuration of that client.
static gattCharCfg_t multiProfileChar6Config[GATT_MAX_NUM_CONN];




/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t multiProfileAttrTbl[/*SERVAPP_NUM_ATTR_SUPPORTED*/8] = 
{
  // multi Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&multiProfileService            /* pValue */
  },
   
    // Characteristic 5 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &multiProfileChar5Props 
    },

      // Characteristic Value 5
      { 
        { ATT_UUID_SIZE, multiProfilechar5UUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        &multiProfileChar5[0]  
      },

      // Characteristic 5 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        multiProfileChar5UserDesp 
      },

    // ----------------------------------------------------------------------
    // Characteristic 6 Declaration, NOTify
    { 
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ, 
        0,
        &multiProfileChar6Props 
    },

    // Characteristic Value 6
    { 
        { ATT_UUID_SIZE, multiProfilechar6UUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8 *)&multiProfileChar6 
    },

    // Characteristic 6 configuration
    { 
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8 *)multiProfileChar6Config 
    }, 

    // Characteristic 6 User Description
    { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        multiProfileChar6UserDesp 
    }, 
    
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 multiProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint16 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t multiProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint16 len, uint16 offset );

static void multiProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType );


/*********************************************************************
 * PROFILE CALLBACKS
 */
// multi Profile Service Callbacks
CONST gattServiceCBs_t multiProfileCBs =
{
  multiProfile_ReadAttrCB,  // Read callback function pointer
  multiProfile_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      multiProfile_AddService
 *
 * @brief   Initializes the multi Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t MultiProfile_AddService( uint32 services )
{
  uint8 status = SUCCESS;

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, multiProfileChar6Config );

  // Register with Link DB to receive link status change callback
  linkDB_Register( multiProfile_HandleConnStatusCB );  
  
  if ( services & MULTIPROFILE_SERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( multiProfileAttrTbl, 
                                          GATT_NUM_ATTRS( multiProfileAttrTbl ),
                                          &multiProfileCBs );
  }

  return ( status );
}


/*********************************************************************
 * @fn      multiProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t MultiProfile_RegisterAppCBs( multiProfileCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    multiProfile_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}
  

/*********************************************************************
 * @fn      multiProfile_SetParameter
 *
 * @brief   Set a multi Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t MultiProfile_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case MULTIPROFILE_CHAR5:
      if ( len <= MULTI_ATT_LONG_PKT)   
      {
        osal_memcpy(multiProfileChar5, value, len);				
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn      multiProfile_GetParameter
 *
 * @brief   Get a multi Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t MultiProfile_GetParameter( uint16 connHandle,uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case MULTIPROFILE_CHAR5:
       VOID osal_memcpy( value, multiProfileChar5, ATT_GetCurrentMTUSize( connHandle )-3 );  
      break; 
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          multiProfile_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 multiProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint16 *pLen, uint16 offset, uint8 maxLen )
{
  bStatus_t status = SUCCESS;

//	AT_LOG("Read call back connhandle : %d\n",connHandle);
  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }
 
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
      // gattserverapp handles those reads

      case MULTIPROFILE_CHAR1_UUID:
	      *pLen = MULTI_UUID_LEN;
	      VOID osal_memcpy( pValue, pAttr->pValue, MULTI_UUID_LEN );
	      break;
      case MULTIPROFILE_CHAR2_UUID:
      case MULTIPROFILE_CHAR3_UUID:
	  	//case multiPROFILE_CHAR4_UUID:
	      *pLen = 2;
	      VOID osal_memcpy( pValue, pAttr->pValue, *pLen );
	      break;
      case MULTIPROFILE_CHAR4_UUID:
	      *pLen = 1;
	      pValue[0] = *pAttr->pValue;
	      break;

      case MULTIPROFILE_CHAR7_UUID:

            *pLen = ATT_GetCurrentMTUSize(connHandle)-3;
	      
	        VOID osal_memcpy( pValue, pAttr->pValue, ATT_GetCurrentMTUSize(connHandle)-3);


	      break;
          
        
      default:
	      // Should never get here! (characteristics 3 and 4 do not have read permissions)
	      *pLen = 0;
	      status = ATT_ERR_ATTR_NOT_FOUND;
	      break;
    }
  }
  else if ( pAttr->type.len == ATT_UUID_SIZE )
  {
  	switch( pAttr->type.uuid[12])
	{
		case 0x03:
		{
//			*pLen = ATT_GetCurrentMTUSize(connHandle)-3
            LOG("READ CB %d\n",multiChar6Notifylen);
			*pLen = multiChar6Notifylen;			
			VOID osal_memcpy( pValue, pAttr->pValue, multiChar6Notifylen );
		}
		break;
		default:
			break;
	}
  }
  else
  {
    // 128-bit UUID
    *pLen = 0;
    status = ATT_ERR_INVALID_HANDLE;
  }

  return ( status );
}

/*********************************************************************
 * @fn      multiProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 *
 * @return  Success or Failure
 */
 // TODO: test this function
static bStatus_t multiProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint16 len, uint16 offset )
{
	bStatus_t status = SUCCESS;
	uint8 notifyApp = 0xFF;
	// If attribute permissions require authorization to write, return error
	if ( gattPermitAuthorWrite( pAttr->permissions ) )
	{
		// Insufficient authorization
		return ( ATT_ERR_INSUFFICIENT_AUTHOR );
	}

	if ( pAttr->type.len == ATT_BT_UUID_SIZE )
	{
		// 16-bit UUID
		uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
		switch ( uuid )
		{
			case MULTIPROFILE_CHAR1_UUID:
			{
				//Validate the value
				// Make sure it's not a blob oper
				if ( offset == 0 )
				{
					if ( len != MULTI_UUID_LEN )
					{
						status = ATT_ERR_INVALID_VALUE_SIZE;
					}
				}
				else
				{
					status = ATT_ERR_ATTR_NOT_LONG;
				}
				//Write the value
				if ( status == SUCCESS )
				{
					uint8 *pCurValue = (uint8 *)pAttr->pValue;        
					VOID osal_memcpy( pCurValue, pValue, MULTI_UUID_LEN );

					notifyApp = MULTIPROFILE_CHAR1;   
				}
			}
			break;
			case MULTIPROFILE_CHAR2_UUID:	  	
			case MULTIPROFILE_CHAR3_UUID:
			{
				//Validate the value
				// Make sure it's not a blob oper
				if ( offset == 0 )
				{
					if ( len != 2 )
					{
						status = ATT_ERR_INVALID_VALUE_SIZE;
					}
				}
				else
				{
					status = ATT_ERR_ATTR_NOT_LONG;
				}
				//Write the value
				if ( status == SUCCESS )
				{
					uint8 *pCurValue = (uint8 *)pAttr->pValue;        
					*pCurValue = pValue[0];
					*(pCurValue + 1	) =  pValue[1];   
					if( uuid == MULTIPROFILE_CHAR2_UUID)
					{
						notifyApp = MULTIPROFILE_CHAR2;        
					}
					else
					{
						notifyApp = MULTIPROFILE_CHAR3;           
					}
				}
			}
			break;	  	
			case MULTIPROFILE_CHAR4_UUID:
			{
				//Validate the value
				// Make sure it's not a blob oper
				if ( offset == 0 )
				{
					if ( len != 1 )
					{
						status = ATT_ERR_INVALID_VALUE_SIZE;
					}
				}
				else
				{
					status = ATT_ERR_ATTR_NOT_LONG;
				}
				//Write the value
				if ( status == SUCCESS )
				{
					uint8 *pCurValue = (uint8 *)pAttr->pValue;        
					*pCurValue = pValue[0];

					if( uuid == MULTIPROFILE_CHAR4_UUID )
					{
						notifyApp = MULTIPROFILE_CHAR4;        
					}
				}
			}
			break;
			       
			case MULTIPROFILE_CHAR7_UUID:
			{
				if ( offset == 0 )
				{
					if ( len >ATT_GetCurrentMTUSize(connHandle)-3 )
					{
						status = ATT_ERR_INVALID_VALUE_SIZE;
					}
				}
				else
				{
					status = ATT_ERR_ATTR_NOT_LONG;
				}
				if ( status == SUCCESS )
				{
					notifyApp = MULTIPROFILE_CHAR7;   
				}
			}
			break;  
			case GATT_CLIENT_CHAR_CFG_UUID:
			{
				status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
				offset, GATT_CLIENT_CFG_NOTIFY );
				if ( status == SUCCESS )
				{
					notifyApp = MULTIPROFILE_CHAR6;  
				}
			}
			break;

			default:
			// Should never get here! (characteristics 2 and 4 do not have write permissions)
			status = ATT_ERR_ATTR_NOT_FOUND;
			break;
		}
	}
	else if ( pAttr->type.len == ATT_UUID_SIZE )
	{
		switch( pAttr->type.uuid[12])
		{
			case 0x02:
			{
				// 数据接收UUID 
				//Validate the value
				// Make sure it's not a blob oper
				if ( offset == 0 )
				{
					if ( len >ATT_GetCurrentMTUSize(connHandle)-3 )
					{
						status = ATT_ERR_INVALID_VALUE_SIZE;
					}
				}
				else
				{
					status = ATT_ERR_ATTR_NOT_LONG;
				}
				//Write the value
				if ( status == SUCCESS )
				{
					uint8 *pCurValue = (uint8 *)pAttr->pValue;		  
					VOID osal_memcpy( pCurValue, pValue, len );
					notifyApp = MULTIPROFILE_CHAR5;   
				}
			} 
			break;
			default:
				break;
		}

	}
	else
	{
		// 128-bit UUID
		status = ATT_ERR_INVALID_HANDLE;
	}

	// If a charactersitic value changed then callback function to notify application of change
	if ( (notifyApp != 0xFF ) && multiProfile_AppCBs && multiProfile_AppCBs->pfnMultiProfileChange )
	{
		multiProfile_AppCBs->pfnMultiProfileChange(connHandle,notifyApp,len );  
	}

	return ( status );
}

/*********************************************************************
 * @fn          multiProfile_HandleConnStatusCB
 *
 * @brief       multi Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void multiProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{ 
  // Make sure this is not loopback connection
  if ( connHandle != LOOPBACK_CONNHANDLE )
  {
    // Reset Client Char Config if connection has dropped
    if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
         ( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) && 
           ( !linkDB_Up( connHandle ) ) ) )
    { 
      GATTServApp_InitCharCfg( connHandle, multiProfileChar6Config );
//	  AT_LOG(" multiProfile_HandleConnStatusCB \n");
    }
  }
}


bStatus_t MultiProfile_Notify(uint16 connHandle,uint8 param, uint16 len, void *value )
{
    bStatus_t ret = SUCCESS;
    uint16 notfEnable;
    switch ( param )
    {  
	 case MULTIPROFILE_CHAR6:

        notfEnable = GATTServApp_ReadCharCfg( connHandle, multiProfileChar6Config );
    
        // If notifications enabled
        if ( notfEnable & GATT_CLIENT_CFG_NOTIFY )
        {
//			for(uint8 i=0;i<len;i++)
//    		{
//				if(((uint8*)value)[i]==0x68)
//				{
//					VOID osal_memcpy( multiProfileChar6, &((uint8*)value)[i], len-i );	
//				}
//    		}	
            VOID osal_memcpy( multiProfileChar6, value, len );
			multiChar6Notifylen = len;

            //for SAR test copy the seqNum in pkt tail
//            multiProfileChar6[ATT_GetCurrentMTUSize(connHandle)-4-1]=multiProfileChar6[0];
//            multiProfileChar6[ATT_GetCurrentMTUSize(connHandle)-3-1]=multiProfileChar6[1];
            
            
            ret=GATTServApp_ProcessCharCfg( multiProfileChar6Config, multiProfileChar6, FALSE,
                                    multiProfileAttrTbl, GATT_NUM_ATTRS( multiProfileAttrTbl ),
                                    INVALID_TASK_ID );

        }
        else
        {
            ret = bleNotReady;
        }
        break;
          
      default:
        ret = INVALIDPARAMETER;
        break;
    }
    
    return ( ret );
}


/*********************************************************************
*********************************************************************/
