/**************************************************************************************************
  Filename:       simpleBLECentral_Main.c
  Revised:        $Date: 2020-02-24 15:48:00 -0800 (Thu, 24 Feb 2011) $
  Revision:       $Revision: 11 $

  Description:    This file contains the main and callback functions for
                  the Simple BLE Central sample application.

**************************************************************************************************/

/**************************************************************************************************
 *                                           Includes
 **************************************************************************************************/
 
#if (APP_CFG == 5)

/* Hal Drivers */

/* OSAL */
#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "OSAL_PwrMgr.h"

/**************************************************************************************************
 * FUNCTIONS
 **************************************************************************************************/

/**************************************************************************************************
 * @fn          main
 *
 * @brief       Start of application.
 *
 * @param       none
 *
 * @return      none
 **************************************************************************************************
 */
int app_main(void)
{
    /* Initialize the operating system */
    osal_init_system();

    osal_pwrmgr_device( PWRMGR_BATTERY );
//  /* Enable interrupts */
//  HAL_ENABLE_INTERRUPTS();

    
    /* Start OSAL */
    osal_start_system(); // No Return from here

    return 0;
}

#endif
/**************************************************************************************************
                                           CALL-BACKS
**************************************************************************************************/


/*************************************************************************************************
**************************************************************************************************/