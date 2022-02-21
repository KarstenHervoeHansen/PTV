/* $Id: xgpio.c,v 1.4 2005/01/13 15:33:21 moleres Exp $ */
/******************************************************************************
*
*       XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*       AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*       SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*       OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*       APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*       THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*       AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*       FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*       WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*       IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*       REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*       INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*       FOR A PARTICULAR PURPOSE.
*
*       (c) Copyright 2002 - 2004 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/**
* @file xgpio.c
*
* The implementation of the XGpio component's basic functionality. See xgpio.h
* for more information about the component.
*
* @note
*
* None
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rmm  02/04/02 First release
* 2.00a jhl  12/16/02 Update for dual channel and interrupt support
* </pre>
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include "xparameters.h"
#include "xgpio.h"
#include "xgpio_i.h"
#include "xstatus.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Variable Definitions ****************************/


/************************** Function Prototypes *****************************/


/****************************************************************************/
/**
* Initialize the XGpio instance provided by the caller based on the
* given DeviceID.
*
* Nothing is done except to initialize the InstancePtr.
*
* @param InstancePtr is a pointer to an XGpio instance. The memory the pointer
*        references must be pre-allocated by the caller. Further calls to
*        manipulate the component through the XGpio API must be made with this
*        pointer.
*
* @param DeviceId is the unique id of the device controlled by this XGpio
*        component.  Passing in a device id associates the generic XGpio
*        instance to a specific device, as chosen by the caller or application
*        developer.
*
* @return
*
* - XST_SUCCESS           Initialization was successfull.
* - XST_DEVICE_NOT_FOUND  Device configuration data was not found for a device
*                         with the supplied device ID.
*
* @note
*
* None.
*
*****************************************************************************/
XStatus XGpio_Initialize(XGpio *InstancePtr, Xuint16 DeviceId)
{
    XGpio_Config *ConfigPtr;

    /*
     * Assert arguments
     */
    XASSERT_NONVOID(InstancePtr != XNULL);

    /*
     * Lookup configuration data in the device configuration table.
     * Use this configuration info down below when initializing this component.
     */
    ConfigPtr = XGpio_LookupConfig(DeviceId);
    if (ConfigPtr == (XGpio_Config *)XNULL)
    {
        InstancePtr->IsReady = 0;
        return(XST_DEVICE_NOT_FOUND);
    }

    /*
     * Set some default values.
     */
    InstancePtr->BaseAddress = ConfigPtr->BaseAddress;
    InstancePtr->ConfigPtr = ConfigPtr;

    /*
     * Indicate the instance is now ready to use, initialized without error
     */
    InstancePtr->IsReady = XCOMPONENT_IS_READY;
    return(XST_SUCCESS);
}


/******************************************************************************/
/**
* Lookup the device configuration based on the unique device ID.  The table
* ConfigTable contains the configuration info for each device in the system.
*
* @param DeviceId is the device identifier to lookup.
*
* @return
*
* - A pointer of data type XGpio_Config which points to the device 
*   configuration if DeviceID is found.
* - XNULL if DeviceID is not found.
*
* @note
*
* None.
*
******************************************************************************/
XGpio_Config *XGpio_LookupConfig(Xuint16 DeviceId)
{
    XGpio_Config *CfgPtr = XNULL;

    int i;

    for (i=0; i < XPAR_XGPIO_NUM_INSTANCES; i++)
    {
        if (XGpio_ConfigTable[i].DeviceId == DeviceId)
        {
            CfgPtr = &XGpio_ConfigTable[i];
            break;
        }
    }

    return CfgPtr;
}


/****************************************************************************/
/**
* Set the input/output direction of all discrete signals for the specified 
* GPIO channel.
*
* @param InstancePtr is a pointer to an XGpio instance to be worked on.
* @param Channel contains the channel of the GPIO (1 or 2) to operate on.
* @param DirectionMask is a bitmask specifying which discretes are input and
*        which are output. Bits set to 0 are output and bits set to 1 are input.
*
* @return
*
* None.
*
* @note
*
* The hardware must be built for dual channels if this function is used 
* with any channel other than 1.  If it is not, this function will assert.
*
*****************************************************************************/
void XGpio_SetDataDirection(XGpio *InstancePtr, unsigned Channel,
                            Xuint32 DirectionMask)
{
    XASSERT_VOID(InstancePtr != XNULL);
    XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);
    XASSERT_VOID((Channel == 1)  || 
                 ((Channel == 2) && 
                  (InstancePtr->ConfigPtr->IsDual == XTRUE))); 

    XGpio_mWriteReg(InstancePtr->BaseAddress, 
                    ((Channel - 1) * XGPIO_CHAN_OFFSET) + XGPIO_TRI_OFFSET, 
                    DirectionMask);
}


/****************************************************************************/
/**
* Read state of discretes for the specified GPIO channnel.
*
* @param InstancePtr is a pointer to an XGpio instance to be worked on.
* @param Channel contains the channel of the GPIO (1 or 2) to operate on.
*
* @return Current copy of the discretes register.
*
* @note
*
* The hardware must be built for dual channels if this function is used 
* with any channel other than 1.  If it is not, this function will assert.
*
*****************************************************************************/
Xuint32 XGpio_DiscreteRead(XGpio *InstancePtr, unsigned Channel)
{
    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);
    XASSERT_NONVOID((Channel == 1)  || 
                    ((Channel == 2) && 
                     (InstancePtr->ConfigPtr->IsDual == XTRUE))); 
    
    return XGpio_mReadReg(InstancePtr->BaseAddress, 
                          ((Channel - 1) * XGPIO_CHAN_OFFSET) + 
                          XGPIO_DATA_OFFSET);
}

/****************************************************************************/
/**
* Write to discretes register for the specified GPIO channel.
*
* @param InstancePtr is a pointer to an XGpio instance to be worked on.
* @param Channel contains the channel of the GPIO (1 or 2) to operate on.
* @param Data is the value to be written to the discretes register.
*
* @return
*
* None.
*
* @note
*
* The hardware must be built for dual channels if this function is used 
* with any channel other than 1.  If it is not, this function will assert.
* See also XGpio_DiscreteSet() and XGpio_DiscreteClear().
*
*****************************************************************************/
void XGpio_DiscreteWrite(XGpio *InstancePtr, unsigned Channel, Xuint32 Data)
{
    XASSERT_VOID(InstancePtr != XNULL);
    XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);
    XASSERT_VOID((Channel == 1)  || 
                 ((Channel == 2) && 
                  (InstancePtr->ConfigPtr->IsDual == XTRUE))); 

    XGpio_mWriteReg(InstancePtr->BaseAddress, 
                    ((Channel - 1) * XGPIO_CHAN_OFFSET) + XGPIO_DATA_OFFSET, 
                    Data);
}

