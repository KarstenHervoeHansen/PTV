/* $Id: xutil_memtest.c,v 1.9 2005/01/04 18:12:29 moleres Exp $ */
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
* @file xutil_memtest.c
*
* Contains the memory test utility functions.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who    Date    Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ecm  11/01/01 First release
* 1.00a xd   11/03/04 Improved support for doxygen.
* </pre>
*
*****************************************************************************/

/***************************** Include Files ********************************/
#include "xbasic_types.h"
#include "xstatus.h"
#include "xutil.h"

/************************** Constant Definitions ****************************/
/************************** Function Prototypes *****************************/

static Xuint32 RotateLeft(Xuint32 Input, Xuint8 Width);

/* define ROTATE_RIGHT to give access to this functionality */
/* #define ROTATE_RIGHT */
#ifdef ROTATE_RIGHT
static Xuint32 RotateRight(Xuint32 Input, Xuint8 Width);
#endif /* ROTATE_RIGHT*/


/*****************************************************************************/
/**
*
* Performs a destructive 32-bit wide memory test.
*
* @param    Addr is a pointer to the region of memory to be tested.
* @param    Words is the length of the block.
* @param    Pattern is the constant used for the constant pattern test, if 0,
*           0xDEADBEEF is used.
* @param    Subtest is the test selected. See xutil.h for possible values.
*
* @return
*
* - XST_MEMTEST_FAILED is returned for a failure
* - XST_SUCCESS is returned for a pass
*
* @note
*
* Used for spaces where the address range of the region is smaller than
* the data width. If the memory range is greater than 2 ** width,
* the patterns used in XUT_WALKONES and XUT_WALKZEROS will repeat on a
* boundry of a power of two making it more difficult to detect addressing
* errors. The XUT_INCREMENT and XUT_INVERSEADDR tests suffer the same
* problem. Ideally, if large blocks of memory are to be tested, break
* them up into smaller regions of memory to allow the test patterns used
* not to repeat over the region tested.
*
*****************************************************************************/
XStatus XUtil_MemoryTest32(Xuint32 *Addr,Xuint32 Words,Xuint32 Pattern,
                           Xuint8 Subtest)
{
    Xuint32 i;
    Xuint32 j;
    Xuint32 Val = XUT_MEMTEST_INIT_VALUE;
    Xuint32 FirstVal = XUT_MEMTEST_INIT_VALUE;
    Xuint32 Word;

    XASSERT_NONVOID(Words != 0);
    XASSERT_NONVOID(Subtest <= XUT_MAXTEST);

    /*
     * Select the proper Subtest
     */


    switch (Subtest)
    {

        case XUT_ALLMEMTESTS:

        /* this case executes all of the Subtests */

        /* fall through case statement */

        case XUT_INCREMENT:
        {

            /*
             * Fill the memory with incrementing
             * values starting from 'FirstVal'
             */
            for (i = 0L; i < Words; i++)
            {
                Addr[i] = Val;

                /* write memory location */

                Val++;
            }

            /*
             * Restore the reference 'Val' to the
             * initial value
             */

            Val = FirstVal;

            /*
             * Check every word within the Words
             * of tested memory and compare it
             * with the incrementing reference
             * Val
             */

            for (i = 0L; i < Words; i++)
            {
                Word = Addr[i];

                if (Word != Val)
                {
                    return XST_MEMTEST_FAILED;
                }

                Val++;
            }


            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }


        } /* end of case 1 */

        /* fall through case statement */

        case XUT_WALKONES:
        {
            /*
             * set up to cycle through all possible initial
             * test Patterns for walking ones test
             */

            for (j = 0L; j < 32; j++)
            {
                /*
                 * Generate an initial value for walking ones test to test for bad
                 * data bits
                 */

                Val = 1 << j;

                /*
                 * START walking ones test
                 * Write a one to each data bit indifferent locations
                 */

                for (i = 0L; i < 32; i++)
                {

                    /* write memory location */

                    Addr[i] = Val;
                    Val = (Xuint32) RotateLeft(Val, 32);

                }

                /*
                 * Restore the reference 'Val' to the
                 * initial value
                 */
                Val = 1 << j;

                /* Read the values from each location that was written */

                for (i = 0L; i < 32; i++)
                {
                    /* read memory location */

                    Word = Addr[i];

                    if (Word != Val)
                    {
                        return XST_MEMTEST_FAILED;
                    }

                    Val = (Xuint32) RotateLeft(Val, 32);

                }

            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }


        } /* end of case 2 */

        /* fall through case statement */

        case XUT_WALKZEROS:
        {
            /*
             * set up to cycle through all possible
             * initial test Patterns for walking zeros test
             */

            for (j = 0L; j < 32; j++)
            {

                /*
                 * Generate an initial value for walking ones test to test for
                 * bad data bits
                 */

                Val = ~(1 << j);

                /*
                 * START walking zeros test
                 * Write a one to each data bit indifferent locations
                 */

                for (i = 0L; i < 32; i++)
                {

                    /* write memory location */

                    Addr[i] = Val;
                    Val = ~((Xuint32) RotateLeft(~Val, 32));

                }

                /*
                 * Restore the reference 'Val' to the
                 * initial value
                 */

                Val = ~(1 << j);

                /* Read the values from each location that was written */

                for (i = 0L; i < 32; i++)
                {

                    /* read memory location */

                    Word = Addr[i];

                    if (Word != Val)
                    {
                        return XST_MEMTEST_FAILED;
                    }

                    Val = ~((Xuint32) RotateLeft(~Val, 32));

                }

            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }

        } /* end of case 3 */

        /* fall through case statement */

        case XUT_INVERSEADDR:
        {

            /* Fill the memory with inverse of address */

            for (i = 0L; i < Words; i++)
            {

                /* write memory location */

                Val = (Xuint32) (~((Xuint32)(&Addr[i])));

                Addr[i] = Val;

            }

            /*
             * Check every word within the Words
             * of tested memory
             */

            for (i = 0L; i < Words; i++)
            {

                /* Read the location */

                Word = Addr[i];

                Val = (Xuint32) (~((Xuint32)(&Addr[i])));

                if ((Word ^ Val) != 0x00000000)
                {
                    return XST_MEMTEST_FAILED;
                }
            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }


        } /* end of case 4 */


        /* fall through case statement */

        case XUT_FIXEDPATTERN:
        {

            /*
             * Generate an initial value for
             * memory testing
             */

            if (Pattern == 0)
            {
                Val = 0xDEADBEEF;

            }
            else
            {
                Val = Pattern;

            }

            /*
             * Fill the memory with fixed pattern
             */

            for (i = 0L; i < Words; i++)
            {
                /* write memory location */

                Addr[i] = Val;

            }

            /*
             * Check every word within the Words
             * of tested memory and compare it
             * with the fixed pattern
             */

            for (i = 0L; i < Words; i++)
            {

                /* read memory location */

                Word = Addr[i];

                if (Word != Val)
                {
                    return XST_MEMTEST_FAILED;
                }
            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }

        } /* end of case 5 */

        /* this break is for the prior fall through case statements */

        break ;

        default:
        {
            return XST_MEMTEST_FAILED;
        }

    }  /* end of switch */

    /* Successfully passed memory test ! */

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* Performs a destructive 16-bit wide memory test.
*
* @param    Addr is a pointer to the region of memory to be tested.
* @param    Words is the length of the block.
* @param    Pattern is the constant used for the constant pattern test, if 0,
*           0xDEADBEEF is used.
* @param    Subtest is the test selected. See xutil.h for possible values.
*
* @return
*
* - XST_MEMTEST_FAILED is returned for a failure
* - XST_SUCCESS is returned for a pass
*
* @note
*
* Used for spaces where the address range of the region is smaller than
* the data width. If the memory range is greater than 2 ** width,
* the patterns used in XUT_WALKONES and XUT_WALKZEROS will repeat on a
* boundry of a power of two making it more difficult to detect addressing
* errors. The XUT_INCREMENT and XUT_INVERSEADDR tests suffer the same
* problem. Ideally, if large blocks of memory are to be tested, break
* them up into smaller regions of memory to allow the test patterns used
* not to repeat over the region tested.
*
*****************************************************************************/
XStatus XUtil_MemoryTest16(Xuint16 *Addr,Xuint32 Words, Xuint16 Pattern,
                           Xuint8 Subtest)
{
    Xuint32 i;
    Xuint32 j;
    Xuint16 Val= XUT_MEMTEST_INIT_VALUE;
    Xuint16 FirstVal= XUT_MEMTEST_INIT_VALUE;
    Xuint16 Word;

    XASSERT_NONVOID(Words != 0);
    XASSERT_NONVOID(Subtest <= XUT_MAXTEST);

    /*
     * selectthe proper Subtest(s)
     */

    switch (Subtest)
    {

        case XUT_ALLMEMTESTS:

        /* this case executes all of the Subtests */

        /* fall through case statement */

        case XUT_INCREMENT:
        {

            /*
             * Fill the memory with incrementing
             * values starting from 'FirstVal'
             */
            for (i = 0L; i < Words; i++)
            {
                /* write memory location */

                Addr[i] = Val;

                Val++;
            }

            /*
             * Restore the reference 'Val' to the
             * initial value
             */

            Val = FirstVal;

            /*
             * Check every word within the Words
             * of tested memory and compare it
             * with the incrementing reference
             * Val
             */

            for (i = 0L; i < Words; i++)
            {

                /* read memory location */

                Word = Addr[i];

                if (Word != Val)
                {
                    return XST_MEMTEST_FAILED;
                }
                Val++;
            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }

        } /* end of case 1 */

        /* fall through case statement */

        case XUT_WALKONES:
        {
            /*
             * set up to cycle through all possible initial test
             * Patterns for walking ones test
             */

            for (j = 0L; j < 16; j++)
            {
                /*
                 * Generate an initial value for walking ones test to test for bad
                 * data bits
                 */

                Val = 1 << j;

                /*
                 * START walking ones test
                 * Write a one to each data bit indifferent locations
                 */

                for (i = 0L; i < 16; i++)
                {

                    /* write memory location */

                    Addr[i] = Val;

                    Val = (Xuint16) RotateLeft(Val, 16);

                }

                /*
                 * Restore the reference 'Val' to the
                 * initial value
                 */

                Val = 1 << j;

                /* Read the values from each location that was written */

                for (i = 0L; i < 16; i++)
                {

                    /* read memory location */

                    Word = Addr[i];

                    if (Word != Val)
                    {
                        return XST_MEMTEST_FAILED;
                    }

                    Val = (Xuint16) RotateLeft(Val, 16);

                }

            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }


        } /* end of case 2 */

        /* fall through case statement */

        case XUT_WALKZEROS:
        {
            /*
             * set up to cycle through all possible initial
             * test Patterns for walking zeros test
             */

            for (j = 0L; j < 16; j++)
            {

                /*
                 * Generate an initial value for walking ones
                 * test to test for bad
                 * data bits
                 */

                Val = ~(1 << j);

                /*
                 * START walking zeros test
                 * Write a one to each data bit indifferent locations
                 */

                for (i = 0L; i < 16; i++)
                {


                    /* write memory location */

                    Addr[i] = Val;
                    Val = ~((Xuint16) RotateLeft(~Val, 16));

                }

                /*
                 * Restore the reference 'Val' to the
                 * initial value
                 */

                Val = ~(1 << j);

                /* Read the values from each location that was written */

                for (i = 0L; i < 16; i++)
                {

                    /* read memory location */

                    Word = Addr[i];

                    if (Word != Val)
                    {
                        return XST_MEMTEST_FAILED;
                    }

                    Val = ~((Xuint16) RotateLeft(~Val, 16));

                }

            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }

        } /* end of case 3 */

        /* fall through case statement */

        case XUT_INVERSEADDR:
        {

            /* Fill the memory with inverse of address */

            for (i = 0L; i < Words; i++)
            {
                /* write memory location */

                Val = (Xuint16) (~((Xuint32)(&Addr[i])));
                Addr[i] = Val;

            }

            /*
             * Check every word within the Words
             * of tested memory
             */

            for (i = 0L; i < Words; i++)
            {

                 /* read memory location */

                Word = Addr[i];

                Val = (Xuint16) (~((Xuint32)(&Addr[i])));

                if ((Word ^ Val) != 0x0000)
                {
                    return XST_MEMTEST_FAILED;
                }
            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }


        } /* end of case 4 */


        /* fall through case statement */

        case XUT_FIXEDPATTERN:
        {

            /*
             * Generate an initial value for
             * memory testing
             */

            if (Pattern == 0)
            {
                Val = 0xDEAD;

            }
            else
            {
                Val = Pattern;

            }

            /*
             * Fill the memory with fixed pattern
             */

            for (i = 0L; i < Words; i++)
            {

                /* write memory location */

                Addr[i] = Val;

            }

            /*
             * Check every word within the Words
             * of tested memory and compare it
             * with the fixed pattern
             */

            for (i = 0L; i < Words; i++)
            {

                /* read memory location */

                Word = Addr[i];

                if (Word != Val)
                {
                    return XST_MEMTEST_FAILED;
                }
            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }

        } /* end of case 5 */

        /* this break is for the prior fall through case statements */

        break ;

        default:
        {
            return XST_MEMTEST_FAILED;
        }

    }  /* end of switch */

    /* Successfully passed memory test ! */

    return XST_SUCCESS;
}


/*****************************************************************************/
/**
*
* Performs a destructive 8-bit wide memory test.
*
* @param    Addr is a pointer to the region of memory to be tested.
* @param    Words is the length of the block.
* @param    Pattern is the constant used for the constant pattern test, if 0,
*           0xDEADBEEF is used.
* @param    Subtest is the test selected. See xutil.h for possible values.
*
* @return
*
* - XST_MEMTEST_FAILED is returned for a failure
* - XST_SUCCESS is returned for a pass
*
* @note
*
* Used for spaces where the address range of the region is smaller than
* the data width. If the memory range is greater than 2 ** width,
* the patterns used in XUT_WALKONES and XUT_WALKZEROS will repeat on a
* boundry of a power of two making it more difficult to detect addressing
* errors. The XUT_INCREMENT and XUT_INVERSEADDR tests suffer the same
* problem. Ideally, if large blocks of memory are to be tested, break
* them up into smaller regions of memory to allow the test patterns used
* not to repeat over the region tested.
*
*****************************************************************************/
XStatus XUtil_MemoryTest8(Xuint8 *Addr,Xuint32 Words, Xuint8 Pattern,
                          Xuint8 Subtest)
{
    Xuint32 i;
    Xuint32 j;
    Xuint8 Val= XUT_MEMTEST_INIT_VALUE;
    Xuint8 FirstVal= XUT_MEMTEST_INIT_VALUE;
    Xuint8 Word;

    XASSERT_NONVOID(Words != 0);
    XASSERT_NONVOID(Subtest <= XUT_MAXTEST);

    /*
     * select the proper Subtest(s)
     */

    switch (Subtest)
    {

        case XUT_ALLMEMTESTS:

        /* this case executes all of the Subtests */

        /* fall through case statement */

        case XUT_INCREMENT:
        {

            /*
             * Fill the memory with incrementing
             * values starting from 'FirstVal'
             */
            for (i = 0L; i < Words; i++)
            {

                /* write memory location */

                Addr[i] = Val;
                Val++;
            }

            /*
             * Restore the reference 'Val' to the
             * initial value
             */

            Val = FirstVal;

            /*
             * Check every word within the Words
             * of tested memory and compare it
             * with the incrementing reference
             * Val
             */

            for (i = 0L; i < Words; i++)
            {

                /* read memory location */

                Word = Addr[i];

                if (Word != Val)
                {
                    return XST_MEMTEST_FAILED;
                }
                Val++;
            }


            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }


        } /* end of case 1 */

        /* fall through case statement */

        case XUT_WALKONES:
        {
            /*
             * set up to cycle through all possible initial
             * test Patterns for walking ones test
             */

            for (j = 0L; j < 8; j++)
            {
                /*
                 * Generate an initial value for walking ones test to test
                 * for bad data bits
                 */

                Val = 1 << j;

                /*
                 * START walking ones test
                 * Write a one to each data bit indifferent locations
                 */

                for (i = 0L; i < 8; i++)
                {

                    /* write memory location */

                    Addr[i] = Val;
                    Val = (Xuint8) RotateLeft(Val, 8);
                }

                /*
                 * Restore the reference 'Val' to the
                 * initial value
                 */
                Val = 1 << j;

                /* Read the values from each location that was written */

                for (i = 0L; i < 8; i++)
                {

                    /* read memory location */

                    Word = Addr[i];

                    if (Word != Val)
                    {
                        return XST_MEMTEST_FAILED;
                    }

                    Val = (Xuint8) RotateLeft(Val, 8);

                }

            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }


        } /* end of case 2 */

        /* fall through case statement */

        case XUT_WALKZEROS:
        {
            /*
             * set up to cycle through all possible initial test
             * Patterns for walking zeros test
             */

            for (j = 0L; j < 8; j++)
            {

                /*
                 * Generate an initial value for walking ones test to test
                 * for bad data bits
                 */

                Val = ~(1 << j);

                /*
                 * START walking zeros test
                 * Write a one to each data bit indifferent locations
                 */

                for (i = 0L; i < 8; i++)
                {


                    /* write memory location */

                    Addr[i] = Val;
                    Val = ~((Xuint8) RotateLeft(~Val, 8));

                }

                /*
                 * Restore the reference 'Val' to the
                 * initial value
                 */

                Val = ~(1 << j);

                /* Read the values from each location that was written */

                for (i = 0L; i < 8; i++)
                {

                    /* read memory location */

                    Word = Addr[i];

                    if (Word != Val)
                    {
                        return XST_MEMTEST_FAILED;
                    }

                    Val = ~((Xuint8) RotateLeft(~Val, 8));

                }

            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }

        } /* end of case 3 */

        /* fall through case statement */

        case XUT_INVERSEADDR:
        {

            /* Fill the memory with inverse of address */

            for (i = 0L; i < Words; i++)
            {

                /* write memory location */

                Val = (Xuint8) (~((Xuint32)(&Addr[i])));
                Addr[i] = Val;

            }

            /*
             * Check every word within the Words
             * of tested memory
             */

            for (i = 0L; i < Words; i++)
            {

                /* read memory location */

                Word = Addr[i];

                Val = (Xuint8) (~((Xuint32)(&Addr[i])));

                if ((Word ^ Val) != 0x00)
                {
                    return XST_MEMTEST_FAILED;
                }
            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }


        } /* end of case 4 */


        /* fall through case statement */

        case XUT_FIXEDPATTERN:
        {

            /*
             * Generate an initial value for
             * memory testing
             */

            if (Pattern == 0)
            {
                Val = 0xA5;

            }
            else
            {
                Val = Pattern;

            }

            /*
             * Fill the memory with fixed pattern
             */

            for (i = 0L; i < Words; i++)
            {

                /* write memory location */

                Addr[i] = Val;

            }

            /*
             * Check every word within the Words
             * of tested memory and compare it
             * with the fixed pattern
             */

            for (i = 0L; i < Words; i++)
            {

                /* read memory location */

                Word = Addr[i];

                if (Word != Val)
                {
                    return XST_MEMTEST_FAILED;
                }
            }

            if (Subtest != XUT_ALLMEMTESTS)
            {
                return XST_SUCCESS;
            }

        } /* end of case 5 */

        /* this break is for the prior fall through case statements */

        break ;

        default:
        {
            return XST_MEMTEST_FAILED;
        }

    }  /* end of switch */

    /* Successfully passed memory test ! */

    return XST_SUCCESS;
}


/*****************************************************************************/
/**
*
* Rotates the provided value to the left one bit position
*
* @param    Input is value to be rotated to the left
* @param    Width is the number of bits in the input data
*
* @return
*
* The resulting unsigned long value of the rotate left
*
* @note
*
* None.
*
*****************************************************************************/
static Xuint32 RotateLeft(Xuint32 Input, Xuint8 Width)
{
    Xuint32 Msb;
    Xuint32 ReturnVal;
    Xuint32 WidthMask;
    Xuint32 MsbMask;

    /*
     * set up the WidthMask and the MsbMask
     */

    MsbMask = 1 << (Width-1);

    WidthMask = (MsbMask << 1) - 1;

    /*
     * set the width of the Input to the correct width
     */

    Input = Input & WidthMask;

    Msb = Input & MsbMask;

    ReturnVal = Input << 1;

    if (Msb != 0x00000000)
    {
        ReturnVal = ReturnVal | 0x00000001;
    }

    ReturnVal = ReturnVal & WidthMask;

    return (ReturnVal);

}

#ifdef ROTATE_RIGHT
/*****************************************************************************/
/**
*
* Rotates the provided value to the right one bit position
*
* @param    Input is value to be rotated to the right
* @param    Width is the number of bits in the input data
*
* @return
*
* The resulting Xuint32 value of the rotate right
*
* @note
*
* None.
*
*****************************************************************************/
static Xuint32 RotateRight(Xuint32 Input, Xuint8 Width)
{
    Xuint32 Lsb;
    Xuint32 ReturnVal;
    Xuint32 WidthMask;
    Xuint32 MsbMask;

    /*
     * set up the WidthMask and the MsbMask
     */

    MsbMask = 1 << (Width-1);

    WidthMask = (MsbMask << 1) - 1;

    /*
     * set the width of the Input to the correct width
     */

    Input = Input & WidthMask;

    ReturnVal = Input >> 1;

    Lsb = Input & 0x00000001;

    if (Lsb != 0x00000000)
    {
        ReturnVal = ReturnVal | MsbMask;
    }

    ReturnVal = ReturnVal & WidthMask;

    return (ReturnVal);

}
#endif /* ROTATE_RIGHT */

