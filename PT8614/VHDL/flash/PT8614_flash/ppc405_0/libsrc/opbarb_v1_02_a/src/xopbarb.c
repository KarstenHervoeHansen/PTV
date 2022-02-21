/* $Id: xopbarb.c,v 1.1 2002/06/26 17:36:23 linnj Exp $ */
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
*       (c) Copyright 2002 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xopbarb.c
*
* This component contains the implementation of the XOpbArb driver component.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.02a rpm  08/13/01 First release
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xopbarb.h"
#include "xio.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/

/*
 * Create the table of options which are processed to get/set the device
 * options. These options are table driven to allow easy maintenance and
 * expansion of the options.
 */
typedef struct
{
    Xuint32 Option;
    Xuint32 Mask;
} OptionsMap;

static OptionsMap OptionsTable[] =
{
    {XOA_DYNAMIC_PRIORITY_OPTION,   XOA_CR_DYNAMIC_ENABLE_MASK},
    {XOA_PARK_ENABLE_OPTION,        XOA_CR_PARK_ENABLE_MASK},
    {XOA_PARK_BY_ID_OPTION,         XOA_CR_PARK_ON_ID_MASK}
};

#define XOA_NUM_OPTIONS      (sizeof(OptionsTable) / sizeof(OptionsMap))

/*****************************************************************************/
/**
*
* Initializes a specific XOpbArb instance. The driver is initialized to allow
* access to the device registers. In addition, the configuration information
* is retrieved for the device. Currently, configuration information is stored
* in xopbarb_g.c.
*
* The state of the device after initialization is:
*   - Fixed or dynamic priority arbitration based on hardware parameter
*   - Bus parking is disabled
*
* @param    InstancePtr is a pointer to the XOpbArb instance to be worked on.
* @param    DeviceId is the unique id of the device controlled by this XOpbArb
*           component. Passing in a device id associates the generic XOpbArb
*           component to a specific device, as chosen by the caller or
*           application developer.
*
* @return
*
* The return value is XST_SUCCESS if successful or XST_DEVICE_NOT_FOUND if no
* configuration data was found for this device.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XOpbArb_Initialize(XOpbArb *InstancePtr, Xuint16 DeviceId)
{
    XOpbArb_Config *ConfigPtr;   /* Pointer to Configuration ROM data */

    XASSERT_NONVOID(InstancePtr != XNULL);

    /*
     * Lookup the device configuration in the temporary CROM table. Use this
     * configuration info down below when initializing this component.
     */
    ConfigPtr = XOpbArb_LookupConfig(DeviceId);
    if (ConfigPtr == XNULL)
    {
        return XST_DEVICE_NOT_FOUND;
    }

    /*
     * Set some default values
     */
    InstancePtr->BaseAddress = ConfigPtr->BaseAddress;
    InstancePtr->NumMasters = ConfigPtr->NumMasters;

    /*
     * Indicate the component is now ready to use since it is initialized
     * without errors
     */
    InstancePtr->IsReady = XCOMPONENT_IS_READY;

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Runs a self-test on the driver/device. The self-test simply verifies that
* the arbiter's registers can be read and written. This is an intrusive test
* in that the arbiter will not be using the priority registers while the test
* is being performed.
*
* @param    InstancePtr is a pointer to the XOpbArb instance to be worked on.
*
* @return
*
* XST_SUCCESS if successful, or XST_REGISTER_ERROR if a register did
* not read or write correctly
*
* @note
*
* The priority level registers are restored after testing them in order to
* prevent problems with the registers being the same value after the test.
* <br><br>
* If the arbiter is in dynamic priority mode, this test changes the mode to
* fixed to ensure that the priority registers aren't changed by the arbiter
* during this test. The mode is restored to it's entry value on exit.
*
******************************************************************************/
XStatus XOpbArb_SelfTest(XOpbArb *InstancePtr)
{
    Xuint8 Level;
    Xuint8 Master;
    Xuint32 LvlReg;
    Xuint32 LvlOffset;
    Xboolean RestoreControlReg = XFALSE;
    Xuint32 MasterRestore;
    Xuint32 ControlReg;

    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /* If the priority registers of the arbiter are being used by the arbiter,
     * or priorities are dynamic, then set the control register such that it
     * does not use them before modifying them, otherwise the arbiter may hang
     * the bus if a master accesses the bus without a priority
     */
    ControlReg = XIo_In32(InstancePtr->BaseAddress + XOA_CR_OFFSET);
    if (ControlReg & (XOA_CR_PRIORITY_VALID_MASK | XOA_CR_DYNAMIC_ENABLE_MASK))
    {
        XIo_Out32(InstancePtr->BaseAddress + XOA_CR_OFFSET, ControlReg &
                  ~(XOA_CR_PRIORITY_VALID_MASK | XOA_CR_DYNAMIC_ENABLE_MASK));
        RestoreControlReg = XTRUE;
    }

    /*
     * Test read/write of all the priority level registers based on
     * the number of masters.
     */
    for (Level = 0; Level < InstancePtr->NumMasters; Level++)
    {
        /*
         * Calculate the offset of the priority level register
         */
        LvlOffset = XOA_LVLX_OFFSET + (Level * XOA_LVLX_SIZE);

        /*
         * Save a copy of the priority level register being tested such that
         * it can be restored after testing the register
         */
        MasterRestore = XIo_In32(InstancePtr->BaseAddress + LvlOffset);

        /*
         * Set the priority level register to each possible master value
         */
        for (Master = 0; Master < InstancePtr->NumMasters; Master++)
        {
            XIo_Out32(InstancePtr->BaseAddress + LvlOffset, Master);

            /*
             * Get the priority level register and verify it is set correctly
             */
            LvlReg = XIo_In32(InstancePtr->BaseAddress + LvlOffset);

            if (LvlReg != Master)
            {
                return XST_REGISTER_ERROR;
            }
        }

        /*
         * Restore the priority level register being tested to it's state
         * before the test
         */
        XIo_Out32(InstancePtr->BaseAddress + LvlOffset, MasterRestore);
    }

    /* If the control register was modified to prevent the arbiter from using
     * the priority registers or to put the arbiter into fixed priority during
     * this test, restore it to it's original value
     */
    if (RestoreControlReg)
    {
        XIo_Out32(InstancePtr->BaseAddress + XOA_CR_OFFSET, ControlReg);
    }

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Sets the options for the OPB arbiter. The options control how the device
* grants the bus to requesting masters.
*
* @param    InstancePtr is a pointer to the XOpbArb instance to be worked on.
* @param    Options contains the specified options to be set. This is a bit
*           mask where a 1 means to turn the option on, and a 0 means to turn
*           the option off. See xopbarb.h for a description of the options.
*
* @return
*
*   - XST_SUCCESS if options are successfully set.
*   - XST_NO_FEATURE if an attempt was made to enable dynamic priority
*     arbitration when the device is configured only for fixed priority
*     arbitration, or an attempt was made to enable parking when bus parking
*     is not supported by the device.
*   - XST_OPBARB_PARK_NOT_ENABLED if bus parking by park ID was enabled
*     but bus parking itself was not enabled.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XOpbArb_SetOptions(XOpbArb *InstancePtr, Xuint32 Options)
{
    Xuint32 ControlReg;
    int Index;

    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    ControlReg = XIo_In32(InstancePtr->BaseAddress + XOA_CR_OFFSET);

    /*
     * If the user is trying to enable dynamic priorities, but the hardware
     * is parameterized for fixed priorities only, then return an error. We
     * determine how the hardware is parameterized based on the read/write
     * nature of the dynamic priority enable bit in the control register.
     */
    if (Options & XOA_DYNAMIC_PRIORITY_OPTION)
    {
        if ((ControlReg & XOA_CR_DYNAMIC_RW_MASK) == 0)
        {
            /*
             * Dynamic Priority Enable bit is read-only, so dynamic priority
             * arbitration is not included in the device
             */
            return XST_NO_FEATURE;
        }
    }

    /*
     * If the user is trying to enable bus parking, but the hardware does not
     * support it based on its parameters, then return an error. We determine
     * how the hardware is parameterized based on the read/write nature of the
     * park enable bit in the control register.
     */
    if (Options & (XOA_PARK_ENABLE_OPTION | XOA_PARK_BY_ID_OPTION))
    {
        if ((ControlReg & XOA_CR_PARK_RW_MASK) == 0)
        {
            /*
             * Park Enable bit is read-only, so bus parking is not included in
             * the device
             */
            return XST_NO_FEATURE;
        }
    }

    /*
     * Verify that if the user is requesting bus parking by ID, they have also
     * enabled bus parking.
     */
    if ((Options & XOA_PARK_BY_ID_OPTION) &&
        ((Options & XOA_PARK_ENABLE_OPTION) == 0))
    {
        return XST_OPBARB_PARK_NOT_ENABLED;
    }

    /*
     * Loop through the options table, turning the option on or off
     * depending on whether the bit is set in the incoming options flag.
     */
    for (Index = 0; Index < XOA_NUM_OPTIONS; Index++)
    {
        if (Options & OptionsTable[Index].Option)
        {
            ControlReg |= OptionsTable[Index].Mask;     /* turn it on */
        }
        else
        {
            ControlReg &= ~OptionsTable[Index].Mask;    /* turn it off */
        }
    }

    /*
     * Now write the control register.
     */
    XIo_Out32(InstancePtr->BaseAddress + XOA_CR_OFFSET, ControlReg);

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Gets the options for the arbiter. The options control how the device grants
* the bus to requesting masters.
*
* @param    InstancePtr is a pointer to the XOpbArb instance to be worked on.
*
* @return
*
* The options of the device. This is a bit mask where a 1 means the option is
* on, and a 0 means the option is off. See xopbarb.h for a description of the
* options.
*
* @note
*
* None.
*
******************************************************************************/
Xuint32 XOpbArb_GetOptions(XOpbArb *InstancePtr)
{
    Xuint32 OptionsFlag = 0;
    Xuint32 ControlReg;
    int Index;

    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * Get the control register to determine which options are currently set.
     */
    ControlReg = XIo_In32(InstancePtr->BaseAddress + XOA_CR_OFFSET);

    /*
     * Loop through the options table to determine which options are set
     */
    for (Index = 0; Index < XOA_NUM_OPTIONS; Index++)
    {
        if (ControlReg & OptionsTable[Index].Mask)
        {
            OptionsFlag |= OptionsTable[Index].Option;
        }
    }

    return OptionsFlag;
}

/*****************************************************************************/
/**
*
* Assigns a master ID to the given priority level. The use of priority levels by
* the device must be suspended before calling this function. Every master ID
* must be assigned to one and only one priority level. The driver enforces
* this before allowing use of priority levels by the device to be resumed.
*
* @param    InstancePtr is a pointer to the XOpbArb instance to be worked on.
* @param    Level is the priority level being set. The level can range from
*           0 (highest) to N (lowest), where N is the number of masters minus
*           one. The device currently supports up to 16 masters.
* @param    MasterId is the ID of the master being assigned to the priority
*           level. The ID can range from 0 to N, where N is the number of
*           masters minus one. The device currently supports up to 16 masters.
*
* @return
*
* - XST_SUCCESS if the slave is selected successfully.
* - XST_OPBARB_NOT_SUSPENDED if priority levels have not been suspended.
*   Before modifying the priority levels, use of priority levels by the device
*   must be suspended.
* - XST_OPBARB_NOT_FIXED_PRIORITY if the arbiter is in dynamic mode. It must
*   be in fixed mode to modify the priority levels.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XOpbArb_SetPriorityLevel(XOpbArb *InstancePtr, Xuint8 Level,
                                      Xuint8 MasterId)
{
    Xuint32 ControlReg;
    Xuint32 LvlOffset;   /* priority level offset */

    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(Level < InstancePtr->NumMasters);
    XASSERT_NONVOID(MasterId < InstancePtr->NumMasters);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * Verify that priority registers have been invalidated (suspended)
     * before allowing a level to be set.
     */
    ControlReg = XIo_In32(InstancePtr->BaseAddress + XOA_CR_OFFSET);

    if (ControlReg & XOA_CR_PRIORITY_VALID_MASK)
    {
        /*
         * Registers are valid, so priority levels have not been suspended
         */
        return XST_OPBARB_NOT_SUSPENDED;
    }

    /* Verify that the arbiter is not in dynamic priority mode because
     * the priority registers will be changing because the h/w uses them
     * even when the priority valid bit is not set
     */
    if (ControlReg & XOA_CR_DYNAMIC_ENABLE_MASK)
    {
        /*
         * Dynamic priority mode is active, don't allow priority changes
         */
        return XST_OPBARB_NOT_FIXED_PRIORITY;
    }

    /*
     * Calculate the offset of the priority level register
     */
    LvlOffset = XOA_LVLX_OFFSET + (Level * XOA_LVLX_SIZE);

    /*
     * Set the priority level register
     */
    XIo_Out32(InstancePtr->BaseAddress + LvlOffset, MasterId);

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Get the master ID at the given priority level.
*
* @param    InstancePtr is a pointer to the XOpbArb instance to be worked on.
* @param    Level is the priority level being retrieved.  The level can range
*           from 0 (highest) to N (lowest), where N is the number of masters
*           minus one. The device currently supports up to 16 masters.
*
* @return
*
* The master ID assigned to the given priority level. The ID can range from
* 0 to N, where N is the number of masters minus one.
*
* @note
*
* If the arbiter is operating in dynamic priority mode, the value returned from
* this function may not be predictable because the arbiter changes the
* values on the fly.
*
******************************************************************************/
Xuint8 XOpbArb_GetPriorityLevel(XOpbArb *InstancePtr, Xuint8 Level)
{
    Xuint32 LvlOffset;   /* priority level offset */

    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(Level < InstancePtr->NumMasters);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * Calculate the offset of the priority level register
     */
    LvlOffset = XOA_LVLX_OFFSET + (Level * XOA_LVLX_SIZE);

    /*
     * Get the priority level register
     */
    return (Xuint8)XIo_In32(InstancePtr->BaseAddress + LvlOffset);
}

/*****************************************************************************/
/**
*
* Suspends use of the priority levels by the device. Before modifying priority
* levels, the application must first suspend use of the levels by the device.
* This is to prevent possible OPB problems if no master is assigned a priority
* during the modification of priority levels. The application must resume use
* of priority levels by the device when all modifications are done. During the
* time priority levels are suspended, the device reverts to its default behavior
* of assigning priorities based on master IDs.
*
* This function can be used when the device is configured for either fixed
* priority arbitration or dynamic priority arbitration. When used during
* dynamic priority arbitration, the application can configure the priority
* levels as a starting point for the LRU algorithm.
*
* @param    InstancePtr is a pointer to the XOpbArb instance to be worked on.
*
* @return
*
* None.
*
* @note
*
* None.
*
******************************************************************************/
void XOpbArb_SuspendPriorityLevels(XOpbArb *InstancePtr)
{
    Xuint32 ControlReg;

    XASSERT_VOID(InstancePtr != XNULL);
    XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * Invalidate (suspend) priority level register use by the device
     */
    ControlReg = XIo_In32(InstancePtr->BaseAddress + XOA_CR_OFFSET);
    XIo_Out32(InstancePtr->BaseAddress + XOA_CR_OFFSET,
              ControlReg & ~XOA_CR_PRIORITY_VALID_MASK);
}

/*****************************************************************************/
/**
*
* Resumes use of the priority levels by the device. This function is typically
* called sometime after a call to SuspendPriorityLevels. The application must
* resume use of priority levels by the device when all modifications are done.
* If no call is made to this function after use of the priority levels has been
* suspended, the device will remain in its default priority arbitration mode of
* assigning priorities based on master IDs. A call to this function has no
* effect if no prior call was made to suspend the use of priority levels.
*
* Every master must be represented by one and only one fixed priority level
* before the use of priority levels can be resumed.
*
* @param    InstancePtr is a pointer to the XOpbArb instance to be worked on.
*
* @return
*
* - XST_SUCCESS if the slave is selected successfully.
* - XST_OPBARB_INVALID_PRIORITY if there is either a master that is not
*   assigned a priority level, or a master that is assigned two mor more
*   priority levels.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XOpbArb_ResumePriorityLevels(XOpbArb *InstancePtr)
{
    Xuint32 ControlReg;
    Xuint32 LvlReg;
    Xuint8 Master;
    Xuint8 Level;
    Xuint8 Found;
    Xuint16 MasterMask;

    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * Verify that every master has one and only one entry in the priority
     * level registers. Build a mask where every bit represents a master
     * (up to 16 masters). The bit is set to 1 if the master is found in
     * a priority register, and a 0 otherwise.
     */
    MasterMask = 0;
    for (Level = 0; Level < InstancePtr->NumMasters; Level++)
    {
        LvlReg = XOpbArb_GetPriorityLevel(InstancePtr, Level);

        MasterMask |= (1 << LvlReg);
    }

    /*
     * Now count how many are set
     */
    Found = 0;
    for (Master = 0; Master < InstancePtr->NumMasters; Master++)
    {
        if (MasterMask & (1 << Master))
        {
            Found++;
        }
    }

    if (Found != InstancePtr->NumMasters)
    {
        return XST_OPBARB_INVALID_PRIORITY;
    }

    /*
     * Validate (resume) priority level register use by the device
     */
    ControlReg = XIo_In32(InstancePtr->BaseAddress + XOA_CR_OFFSET);
    XIo_Out32(InstancePtr->BaseAddress + XOA_CR_OFFSET,
              ControlReg | XOA_CR_PRIORITY_VALID_MASK);

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Sets the master ID used for bus parking. Bus parking must be enabled and the
* option to use bus parking by park ID must be set for this park ID to take
* effect (see the SetOptions function). If the option to use bus parking by park
* ID is set but this function is not called, bus parking defaults to master 0.
*
* @param    InstancePtr is a pointer to the XOpbArb instance to be worked on.
* @param    MasterId is the ID of the master that will be parked if bus parking
*           is enabled. This master's grant signal remains asserted as long as
*           no other master requests the bus. The ID can range from 0 to N,
*           where N is the number of masters minus one. The device currently
*           supports up to 16 masters.
*
* @return
*
* XST_SUCCESS if the park ID is successfully set, or XST_NO_FEATURE if bus
* parking is not supported by the device.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XOpbArb_SetParkId(XOpbArb *InstancePtr, Xuint8 MasterId)
{
    Xuint32 ControlReg;

    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(MasterId < InstancePtr->NumMasters);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * Verify that bus parking is included in the hardware. We determine
     * how the hardware is parameterized based on the read/write nature of the
     * park enable bit in the control register.
     */
    ControlReg = XIo_In32(InstancePtr->BaseAddress + XOA_CR_OFFSET);

    if ((ControlReg & XOA_CR_PARK_RW_MASK) == 0)
    {
        /*
         * Park Enable bit is read-only, so bus parking is not included in
         * the device.
         */
        return XST_NO_FEATURE;
    }

    /*
     * Set the park id, which is in the LSB of the control register
     */
    XIo_Out32(InstancePtr->BaseAddress + XOA_CR_OFFSET,
              ControlReg | MasterId);

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Gets the master ID currently used for bus parking.
*
* @param    InstancePtr is a pointer to the XOpbArb instance to be worked on.
* @param    MasterIdPtr is a pointer to a byte that will hold the master ID
*           currently used for bus parking. This is an output parameter. The
*           ID can range from 0 to N, where N is the number of masters minus
*           one. The device currently supports up to 16 masters.
*
* @return
*
* XST_SUCCESS if the park ID is successfully retrieved, or XST_NO_FEATURE if
* bus parking is not supported by the device.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XOpbArb_GetParkId(XOpbArb *InstancePtr, Xuint8 *MasterIdPtr)
{
    Xuint32 ControlReg;

    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * Verify that bus parking is included in the hardware. We determine
     * how the hardware is parameterized based on the read/write nature of the
     * park enable bit in the control register.
     */
    ControlReg = XIo_In32(InstancePtr->BaseAddress + XOA_CR_OFFSET);

    if ((ControlReg & XOA_CR_PARK_RW_MASK) == 0)
    {
        /*
         * Park Enable bit is read-only, so bus parking is not included in
         * the device.
         */
        return XST_NO_FEATURE;
    }

    /*
     * Store the park id, which is stored in the LSB of the control register
     */
    *MasterIdPtr = (Xuint8)(ControlReg & XOA_CR_PARK_ID_MASK);

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Looks up the device configuration based on the unique device ID. The table
* OpbArbConfigTable contains the configuration info for each device in the
* system.
*
* @param    DeviceId is the unique device ID to match on.
*
* @return
*
* A pointer to the configuration information for the matching device instance,
* or XNULL if no match is found.
*
* @note
*
* None.
*
******************************************************************************/
XOpbArb_Config *XOpbArb_LookupConfig(Xuint16 DeviceId)
{
    extern XOpbArb_Config XOpbArb_ConfigTable[];
    XOpbArb_Config *CfgPtr = XNULL;
    int i;

    for (i=0; i < XPAR_XOPBARB_NUM_INSTANCES; i++)
    {
        if (XOpbArb_ConfigTable[i].DeviceId == DeviceId)
        {
            CfgPtr = &XOpbArb_ConfigTable[i];
            break;
        }
    }

    return CfgPtr;
}
