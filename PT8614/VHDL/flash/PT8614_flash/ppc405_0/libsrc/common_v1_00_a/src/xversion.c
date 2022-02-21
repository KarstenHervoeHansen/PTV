/* $Id: xversion.c,v 1.7 2005/01/04 18:12:29 moleres Exp $ */
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
* @file xversion.c
*
* This file contains the implementation of the XVersion component. This
* component represents a version ID.  It is encapsulated within a component
* so that it's type and implementation can change without affecting users of
* it.
*
* The version is formatted as X.YYZ where X = 0 - 9, Y = 00 - 99, Z = a - z
* X is the major revision, YY is the minor revision, and Z is the
* compatability revision.
*
* Packed versions are also utilized for the configuration ROM such that
* memory is minimized. A packed version consumes only 16 bits and is
* formatted as follows.
*
* <pre>
* Revision                  Range       Bit Positions
*
* Major Revision            0 - 9       Bits 15 - 12
* Minor Revision            0 - 99      Bits 11 - 5
* Compatability Revision    a - z       Bits 4 - 0
*
* MODIFICATION HISTORY:
*
* Ver   Who    Date   Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a xd   11/03/04 Improved support for doxygen.
</pre>
*
******************************************************************************/


/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xversion.h"

/************************** Constant Definitions *****************************/

/* the following constants define the masks and shift values to allow the
 * revisions to be packed and unpacked, a packed version is packed into a 16
 * bit value in the following format, XXXXYYYYYYYZZZZZ, where XXXX is the
 * major revision, YYYYYYY is the minor revision, and ZZZZZ is the compatability
 * revision
 */
#define XVE_MAJOR_SHIFT_VALUE       12
#define XVE_MINOR_ONLY_MASK         0x0FE0
#define XVE_MINOR_SHIFT_VALUE       5
#define XVE_COMP_ONLY_MASK          0x001F

/* the following constants define the specific characters of a version string
 * for each character of the revision, a version string is in the following
 * format, "X.YYZ" where X is the major revision (0 - 9), YY is the minor
 * revision (00 - 99), and Z is the compatability revision (a - z)
 */
#define XVE_MAJOR_CHAR      0   /* major revision 0 - 9 */
#define XVE_MINOR_TENS_CHAR 2   /* minor revision tens 0 - 9 */
#define XVE_MINOR_ONES_CHAR 3   /* minor revision ones 0 - 9 */
#define XVE_COMP_CHAR       4   /* compatability revision a - z */
#define XVE_END_STRING_CHAR 5

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

static Xboolean IsVersionStringValid(Xint8* StringPtr);

/*****************************************************************************/
/**
*
* Unpacks a packed version into the specified version. Versions are packed
* into the configuration ROM to reduce the amount storage. A packed version
* is a binary format as oppossed to a non-packed version which is implemented
* as a string.
*
* @param    InstancePtr points to the version to unpack the packed version into.
* @param    PackedVersion contains the packed version to unpack.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
void XVersion_UnPack(XVersion *InstancePtr, Xuint16 PackedVersion)
{
    /* not implemented yet since CROM related */
}

/*****************************************************************************/
/**
*
* Packs a version into the specified packed version. Versions are packed into
* the configuration ROM to reduce the amount storage.
*
* @param    InstancePtr points to the version to pack.
* @param    PackedVersionPtr points to the packed version which will receive
*           the new packed version.
*
* @return
*
* A status, XST_SUCCESS, indicating the packing was accomplished
* successfully, or an error, XST_INVALID_VERSION, indicating the specified
* input version was not valid such that the pack did not occur
* <br><br>
* The packed version pointed to by PackedVersionPtr is modified with the new
* packed version if the status indicates success.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XVersion_Pack(XVersion *InstancePtr, Xuint16 *PackedVersionPtr)
{
    /* not implemented yet since CROM related */

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Determines if two versions are equal.
*
* @param    InstancePtr points to the first version to be compared.
* @param    VersionPtr points to a second version to be compared.
*
* @return
*
* XTRUE if the versions are equal, XFALSE otherwise.
*
* @note
*
* None.
*
******************************************************************************/
Xboolean XVersion_IsEqual(XVersion *InstancePtr, XVersion *VersionPtr)
{
    Xuint8 *Version1 = (Xuint8 *)InstancePtr;
    Xuint8 *Version2 = (Xuint8 *)VersionPtr;
    int Index;

    /* assert to verify input arguments */

    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(VersionPtr != XNULL);

    /* check each byte of the versions to see if they are the same,
     * return at any point a byte differs between them
     */
    for (Index = 0; Index < sizeof(XVersion); Index++)
    {
        if (Version1[Index] != Version2[Index])
        {
            return XFALSE;
        }
    }

    /* No byte was found to be different between the versions, so indicate
     * the versions are equal
     */
    return XTRUE;
}

/*****************************************************************************/
/**
*
* Converts a version to a null terminated string.
*
* @param    InstancePtr points to the version to convert.
* @param    StringPtr points to the string which will be the result of the
*           conversion. This does not need to point to a null terminated
*           string as an input, but must point to storage which is an adequate
*           amount to hold the result string.
*
* @return
*
* The null terminated string is inserted at the location pointed to by
* StringPtr if the status indicates success.
*
* @note
*
* It is necessary for the caller to have already allocated the storage to
* contain the string.  The amount of memory necessary for the string is
* specified in the version header file.
*
******************************************************************************/
void XVersion_ToString(XVersion *InstancePtr, Xint8 *StringPtr)
{
    /* assert to verify input arguments */

    XASSERT_VOID(InstancePtr != XNULL);
    XASSERT_VOID(StringPtr != XNULL);

    /* since version is implemented as a string, just copy the specified
     * input into the specified output
     */
    XVersion_Copy(InstancePtr, (XVersion *)StringPtr);
}

/*****************************************************************************/
/**
*
* Initializes a version from a null terminated string. Since the string may not
* be a format which is compatible with the version, an error could occur.
*
* @param    InstancePtr points to the version which is to be initialized.
* @param    StringPtr points to a null terminated string which will be
*           converted to a version.  The format of the string must match the
*           version string format which is X.YYX where X = 0 - 9, YY = 00 - 99,
*           Z = a - z.
*
* @return
*
* A status, XST_SUCCESS, indicating the conversion was accomplished
* successfully, or XST_INVALID_VERSION indicating the version string format
* was not valid.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XVersion_FromString(XVersion *InstancePtr, Xint8 *StringPtr)
{
    /* assert to verify input arguments */

    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(StringPtr != XNULL);

    /* if the version string specified is not valid, return an error */

    if (!IsVersionStringValid(StringPtr))
    {
        return XST_INVALID_VERSION;
    }

    /* copy the specified string into the specified version and indicate the
     * conversion was successful
     */
    XVersion_Copy((XVersion *)StringPtr, InstancePtr);

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Copies the contents of a version to another version.
*
* @param    InstancePtr points to the version which is the source of data for
*           the copy operation.
* @param    VersionPtr points to another version which is the destination of
*           the copy operation.
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
void XVersion_Copy(XVersion *InstancePtr, XVersion *VersionPtr)
{
    Xuint8 *Source = (Xuint8 *)InstancePtr;
    Xuint8 *Destination = (Xuint8 *)VersionPtr;
    int Index;

    /* assert to verify input arguments */

    XASSERT_VOID(InstancePtr != XNULL);
    XASSERT_VOID(VersionPtr != XNULL);

    /* copy each byte of the source version to the destination version */

    for (Index = 0; Index < sizeof(XVersion); Index++)
    {
        Destination[Index] = Source[Index];
    }
}

/*****************************************************************************/
/**
*
* Determines if the specified version is valid.
*
* @param    StringPtr points to the string to be validated.
*
* @return
*
* XTRUE if the version string is a valid format, XFALSE otherwise.
*
* @note
*
* None.
*
******************************************************************************/
static Xboolean IsVersionStringValid(Xint8* StringPtr)
{
    /* if the input string is not a valid format, "X.YYZ" where X = 0 - 9,
     * YY = 00 - 99, and Z = a - z, then indicate it's not valid
     */
    if ((StringPtr[XVE_MAJOR_CHAR] < '0') ||
        (StringPtr[XVE_MAJOR_CHAR] > '9') ||
        (StringPtr[XVE_MINOR_TENS_CHAR] < '0') ||
        (StringPtr[XVE_MINOR_TENS_CHAR] > '9') ||
        (StringPtr[XVE_MINOR_ONES_CHAR] < '0') ||
        (StringPtr[XVE_MINOR_ONES_CHAR] > '9') ||
        (StringPtr[XVE_COMP_CHAR] < 'a') ||
        (StringPtr[XVE_COMP_CHAR] > 'z'))
    {
        return XFALSE;
    }

    return XTRUE;
}

