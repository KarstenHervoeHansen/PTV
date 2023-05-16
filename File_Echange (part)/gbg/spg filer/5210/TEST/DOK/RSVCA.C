/*===========================================================================*/
/*  core_srl.c contains variable declarations and definitions of the         */
/*  Serial utility routines described in the LabWindows Instrument Library   */
/*  Developer's Guide.  This file allows for multiple sessions of the        */
/*  same instrument and should be used as a starting point for developing    */
/*  new Serial instrument drivers.  For more instructions on how to use the  */
/*  Core instrument driver files, see the LabWindows Instrument Library      */
/*  Developer's Guide, or the Application Note, Developing a LabWindows/CVI  */
/*  Instrument Driver, part number 340062B-01.  Contact National             */
/*  Instruments to obtain a free copy of the application note.               */
/*                                                                           */
/*  To alter this program for use with a specific instrument perform the     */
/*  following steps:                                                         */
/*===========================================================================*/
/*    1.  Save this file as PREFIX.c, where "PREFIX" is a prefix for your    */
/*        instrument.                                                        */
/*                                                                           */
/*    2.  Select Replace... from the Edit menu and replace all occurrences   */
/*        of the word "PREFIX" with the prefix of your instrument.  Be sure  */
/*        to select the Case Sensitive and Wrap options.                     */
/*                                                                           */
/*    3.  Edit the file PREFIX .H.  Save As PREFIX.h, where "PREFIX" is the  */
/*        prefix of your instrument.  Select Replace... from the Edit menu   */
/*        and replace all occurrences of the word "PREFIX" with the prefix   */
/*        of your instrument.  Be sure to select the Case Sensitive and Wrap */
/*        options.                                                           */
/*                                                                           */
/*    4.  Device-dependent commands in the file PREFIX.c are marked with the */
/*        comment CHANGE.  Search for occurrences of this comment and make   */
/*        the appropriate changes.  Delete comments when completed           */
/*                                                                           */
/*    5.  Edit the function tree of the function panel file, PREFIX .FP.     */
/*        Save As PREFIX.fp where "PREFIX" is the prefix of your instrument. */
/*        Edit the instrument node.  Change the Name to be the name of your  */
/*        instrument and the Prefix to the prefix of your instrument.        */
/*                                                                           */
/*    6.  Insert your instrument-dependent functions.  Be sure to declare    */
/*        all of the user callable functions in the include file "PREFIX.h". */
/*                                                                           */
/*    7.  The define statements for instrument specific WARNINGS and ERRORS  */
/*        need to be inserted in the include file "PREFIX.h".                */
/*                                                                           */
/*        The codes for instrument specific ERRORS must be in the range      */
/*        0xBFFC0900 to 0xBFFC0FFF and defined as follows:                   */
/*        #define PREFIX_SPECIFIC_ERROR (VI_INSTR_ERROR_OFFSET + n)          */
/*                                                                           */
/*        The codes for instrument specific WARNINGS must be in the range    */
/*        0x3FFC0900 to 0x3FFC0FFF and defined as follows:                   */
/*        #define PREFIX_SPECIFIC_WARNING (VI_INSTR_WARNINGS_OFFSET + n)     */
/*                                                                           */
/*        Where n represents each instrument specific error/warning number.  */
/*        The valid range for n is 0 to 0x6FF. For ease of use the lower 16  */
/*        bits of warnings and errors should not overlap in the same driver. */
/*                                                                           */
/*    8.  The PREFIX_SPECIFIC_ERROR/WARNING constants and string descriptors */
/*        then need to be added to the PREFIX_errorMessage function.         */
/*                                                                           */
/*    9.  Delete these instructions and save the modified program.           */
/*                                                                           */
/*===========================================================================*/

#include <visa.h>
#include <ansi_c.h>
#include <formatio.h>
#include "rsvca.h"
  
/*=CHANGE:===================================================================*/
/* Modify the defines below for your particular instrument.                  */
/*= DEFINES =================================================================*/
/* The format for rsvca_REVISION is a string with three fields separated by */ 
/* commas, the first field is the revision of the instrument driver, the     */
/* second field is the month and year of the version's release, and the third*/
/* field is the version of CVI used to write the current version.  Please    */
/* modify here and in the function panel help for the rsvca_revisionQuery   */
/* function panel.                                                           */
/*===========================================================================*/
#define rsvca_REVISION     "Rev 1.0,961111,4.0.1" /*  Instrument driver revision */
#define BUFFER_SIZE         512L         /* I/O buffer size */

/*===========================================================================*/
/* CHANGE: Replace <Instrument Model Name> with your instrument model name.  */
/* Rohde & Schwarz VCA ======================================================*/
/* LabWindows/CVI 3.1 Instrument Driver                                      */
/* Original Release:                                                         */
/* By:                                                                       */
/*     PH.                  Fax                                              */
/*                                                                           */
/* Modification History: None                                                */
/*===========================================================================*/

/*****************************************************************************/
/*= INSTRUMENT-DEPENDENT COMMAND ARRAYS =====================================*/
/*****************************************************************************/

/*= CHANGE: The following is an example of how instrument-dependent command =*/
/*          arrays can  be used in your code.                                */
/*                                                                           */
/* static ViString primDispFunc[] = {"AAC","ADC","OHMS","VAC","VDC"};        */
/* static ViString secDispFunc[]  = {"AAC2","ADC2","OHMS2","VAC2","VDC2"};   */
/*                                                                           */
/*  When formatting strings to send to your instrument, these arrays can be  */
/*  used to simplify your code, for example:                                 */
/*                                                                           */
/*  Fmt (cmd, "%s<"%s; %s;\r\n", primFunc[func1], secFunc[func2]);           */                
/*  if ((rsvca_err = rsvca_writeASCII (instrID, cmd, NumFmtdBytes())) != 0)*/
/*      return rsvca_err;                                                   */
/*                                                                           */
/*  or                                                                       */
/*                                                                           */
/*  if ((rsvca_status = viPrintf (instrSession, "%s; %s;\r\n",              */
/*          primDispFunc[func1], secDispFunc[func2])) < 0)                   */
/*      return rsvca_status;                                                */
/*                                                                           */
/*  The variables func1 and func2 are being used as indexing parameters that */
/*  are passed into the function.                                            */
/*                                                                           */
/*===========================================================================*/

int rsvca_set_function(instrID,func);
{  
  Fmt (cmd, "%s<"%s; %s;\r\n", primFunc[func1], secFunc[func2]);
  if ((rsvca_err = rsvca_writeASCII (instrID, cmd, NumFmtdBytes())) != 0)
      return rsvca_err;

}


/*****************************************************************************/
/*= INSTRUMENT-DEPENDENT STATUS/RANGE STRUCTURE  ============================*/
/*****************************************************************************/
/* rsvca_stringValPair is used in the rsvca_errorMessage function          */
/* rsvca_statusDataRanges is used to track session dependent status & ranges*/
/*===========================================================================*/
typedef struct  rsvca_stringValPair
{
   ViInt32 stringVal;
   ViChar *stringName;
}  rsvca_tStringValPair;
 
    /*=CHANGE:=============================================================*/
    /* Change to reflect the global status variables that your driver needs*/
    /* to keep track of.  For example trigger mode of each session         */
    /*=====================================================================*/
struct rsvca_statusDataRanges {
    ViInt16 triggerMode;
    ViInt16 val2;
    ViInt16 val3;
    ViChar instrDriverRevision[256];
};

typedef struct rsvca_statusDataRanges *rsvca_instrRange;

/*****************************************************************************/
/*= UTILITY ROUTINE DECLARATIONS (Non-Exportable Functions) =================*/
/*****************************************************************************/
ViBoolean rsvca_invalidViBooleanRange (ViBoolean val);
ViBoolean rsvca_invalidViInt16Range (ViInt16 val, ViInt16 min, ViInt16 max);
ViBoolean rsvca_invalidViInt32Range (ViInt32 val, ViInt32 min, ViInt32 max);
ViBoolean rsvca_invalidViUInt8Range (ViUInt8 val, ViUInt8 min, ViUInt8 max);
ViBoolean rsvca_invalidViUInt16Range (ViUInt16 val, ViUInt16 min, ViUInt16 max);
ViBoolean rsvca_invalidViUInt32Range (ViUInt32 val, ViUInt32 min, ViUInt32 max);
ViBoolean rsvca_invalidViReal32Range (ViReal32 val, ViReal32 min, ViReal32 max);
ViBoolean rsvca_invalidViReal64Range (ViReal64 val, ViReal64 min, ViReal64 max);
ViStatus rsvca_initCleanUp (ViSession openRMSession, ViPSession openInstrSession, ViStatus currentStatus);
ViStatus rsvca_readASCII (ViSession instrSession, ViPByte buf, ViUInt32 cnt, ViPUInt32 retCnt);
ViStatus rsvca_readBinary (ViSession instrSession, ViUInt16 rdMode, ViPByte buf, ViUInt32 cnt, ViPUInt32 retCnt);
ViStatus rsvca_writeASCII (ViSession instrSession, ViPByte buf, ViUInt32 cnt, ViPUInt32 retCnt);
ViStatus rsvca_writeBinary (ViSession instrSession, ViUInt16 wrtMode, ViPByte buf, ViUInt32 cnt, ViPUInt32 retCnt);
ViStatus rsvca_initCleanUp (ViSession openRMSession, ViPSession openInstrSession, ViStatus currentStatus);
ViStatus rsvca_readToFile (ViSession instrSession, ViString filename, ViUInt32 readBytes, ViPUInt32 retCount);
ViStatus rsvca_writeFromFile (ViSession instrSession, ViString filename, ViUInt32 writeBytes, ViPUInt32 retCount);
ViStatus rsvca_defaultInstrSetup (ViSession openInstrSession);

/*****************************************************************************/
/*------ INSERT INSTRUMENT-DEPENDENT UTILITY ROUTINE DECLARATIONS HERE ------*/
/*****************************************************************************/


/*****************************************************************************/
/*====== USER-CALLABLE FUNCTIONS (Exportable Functions) =====================*/
/*****************************************************************************/

/*=========================================================================*/
/* Function: Initialize                                                    */
/* Purpose:  This function opens a com port for the instrument, queries    */
/*           the instrument for its ID, and initializes the instrument to  */
/*           a known state.                                                */
/*=========================================================================*/
ViStatus _VI_FUNC rsvca_init (ViRsrc resourceName, ViBoolean IDQuery, 
            ViBoolean resetDevice, ViInt32 baudRate, ViInt16 parity, 
            ViInt32 timeoutmSec, ViPSession instrSession)
{
    ViStatus rsvca_status = VI_SUCCESS;
    ViSession rmSession = 0;
    ViUInt32 retCnt = 0;
    ViByte rdBuffer[BUFFER_SIZE];

    /*- Check input parameter ranges ----------------------------------------*/
    if (rsvca_invalidViBooleanRange (IDQuery))
        return VI_ERROR_PARAMETER2;
    if (rsvca_invalidViBooleanRange (resetDevice))
        return VI_ERROR_PARAMETER3;
        
    /*=CHANGE:===============================================================*/
    /* Remove from both the source code and function panel control those     */
    /* Baud Rates not supported by your Instrument or Platform.              */
    /*=======================================================================*/
    switch (baudRate) {
        case   2400: 
        case   4800: 
        case   9600: 
        case  19200:
            break;
        default:
            rsvca_status = VI_ERROR_PARAMETER4;
            return rsvca_status;
    }
    if (rsvca_invalidViInt16Range (parity, 0, 4))
        return VI_ERROR_PARAMETER5;
    if (timeoutmSec != -1 && timeoutmSec != 0)
        if (rsvca_invalidViInt32Range (timeoutmSec, 1, 0x7fffffff))
            return VI_ERROR_PARAMETER6;

    /*- Open instrument session ---------------------------------------------*/
    if ((rsvca_status = viOpenDefaultRM (&rmSession)) < 0)
        return rsvca_status;

    if ((rsvca_status = viOpen (rmSession, resourceName, VI_NULL, VI_NULL, instrSession)) < 0) {
        viClose (rmSession);
        return rsvca_status;
    }

    /*- Configure VISA Formatted I/O ----------------------------------------*/
    /* Serial Interface initialized to 7 databits, 1 stopbit, no parity.    */
    if ((rsvca_status = viSetAttribute (*instrSession, VI_ATTR_ASRL_DATA_BITS, 7)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
    
    if ((rsvca_status = viSetAttribute (*instrSession, VI_ATTR_ASRL_STOP_BITS,
                                         VI_ASRL_STOP_ONE)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
   
    if ((rsvca_status = viSetBuf (*instrSession, VI_ASRL_IN_BUF|VI_ASRL_OUT_BUF, 512)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
   
    if ((rsvca_status = viSetBuf (*instrSession, VI_READ_BUF|VI_WRITE_BUF, 4000)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
   
    if ((rsvca_status = viSetAttribute (*instrSession, VI_ATTR_WR_BUF_OPER_MODE,
                            VI_FLUSH_ON_ACCESS)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
    
    if ((rsvca_status = viSetAttribute (*instrSession, VI_ATTR_RD_BUF_OPER_MODE,
                            VI_FLUSH_ON_ACCESS)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
 
    if ((rsvca_status = viSetAttribute (*instrSession, VI_ATTR_TERMCHAR, '\n')) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
 
    if ((rsvca_status = viSetAttribute (*instrSession, VI_ATTR_ASRL_END_IN,
                                         VI_ASRL_END_TERMCHAR)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
   
    if ((rsvca_status = viSetAttribute (*instrSession, VI_ATTR_ASRL_END_OUT,
                                         VI_ASRL_END_NONE)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);

    if ((rsvca_status = rsvca_reconfigInterface (*instrSession, baudRate, 
                            parity, timeoutmSec)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);

    /*- Identification Query ------------------------------------------------*/
    /*=CHANGE:===============================================================*/
    /* Query instrument firmware revision.                                   */
    /* Change the ID Query command to the ID Query command for your device.  */
    /* Replace the string "ID_RESPONSE" with the actual query response       */
    /* returned by the instrument.  Change the name ID_RESPONSE_LENGTH to    */
    /* the number of bytes comprising the ID response.                       */
    /*=======================================================================*/
    if (IDQuery) {
        if ((rsvca_status = rsvca_writeASCII (*instrSession, "*IDN?\r\n", 7, &retCnt)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
        if ((rsvca_status = rsvca_readASCII (*instrSession, rdBuffer, BUFFER_SIZE, &retCnt)) < 0)
            return rsvca_status;

        Scan (rdBuffer, "ID_RESPONSE");
        if (NumFmtdBytes () != ID_RESPONSE_LENGTH) 
            return rsvca_initCleanUp (rmSession, instrSession, VI_ERROR_FAIL_ID_QUERY);
    }
        
    /*- Identification Query Not Supported ----------------------------------*/
    /*=CHANGE: ==============================================================*/
    /* If the Instrument Does Not support an ID Query, delete the above      */
    /* section that sends the ID Query to the instrument and use the         */
    /* following two lines of code to return the proper warning status code. */ 
    /*                                                                       */
    /* if (IDQuery)                                                          */
    /*    rsvca_status = VI_WARN_NSUP_ID_QUERY;                             */
    /*                                                                       */
    /* If the ID Query is supported delete this section of comment and code. */
    /*=======================================================================*/

    /*- Reset instrument ----------------------------------------------------*/
    if (resetDevice) {
        if ((rsvca_status = rsvca_reset (*instrSession)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
    }       
    else  /*- Send Default Instrument Setup ---------------------------------*/
        if ((rsvca_status = rsvca_defaultInstrSetup (*instrSession)) < 0)
            return rsvca_initCleanUp (rmSession, instrSession, rsvca_status);
          
    return rsvca_status;
}

/*===========================================================================*/

/*****************************************************************************/
/*-------- INSERT USER-CALLABLE INSTRUMENT-DEPENDENT ROUTINES HERE ----------*/
/*****************************************************************************/
           
           
/*===========================================================================*/
/* Function: Write To Instrument                                             */
/* Purpose:  This function writes a command string to the instrument.        */
/*===========================================================================*/
ViStatus _VI_FUNC rsvca_writeInstrData (ViSession instrSession, ViString writeBuffer)
{
    ViStatus rsvca_status = VI_SUCCESS;
    
    if ((rsvca_status = viPrintf (instrSession, "%s", writeBuffer)) < 0)
        return rsvca_status;

    return rsvca_status;
}

/*===========================================================================*/
/* Function: Read Instrument Buffer                                          */
/* Purpose:  This function reads the output buffer of the instrument.        */
/*===========================================================================*/
ViStatus _VI_FUNC rsvca_readInstrData (ViSession instrSession, ViInt32 numberBytesToRead,
                    ViChar _VI_FAR readBuffer[], ViPInt32 numBytesRead)
{
    ViStatus rsvca_status = VI_SUCCESS;
    *numBytesRead = 0L;
        
    if ((rsvca_status = viRead (instrSession, readBuffer, numberBytesToRead, numBytesRead)) < 0)
        return rsvca_status;

    return rsvca_status;
}

/*===========================================================================*/
/* Function: Reset                                                           */
/* Purpose:  This function resets the instrument.  If the reset function     */
/*           is not supported by the instrument, this function returns       */
/*           the warning VI_WARN_NSUP_RESET.                                 */
/*===========================================================================*/
ViStatus _VI_FUNC rsvca_reset (ViSession instrSession)
{
    ViUInt32 retCnt = 0;
    ViStatus rsvca_status = VI_SUCCESS;

    /*=CHANGE:=============================================================*/
    /* The following lines are used to Reset the instrument to a known     */
    /* state.  Replace the string "*RST" with the reset string you wish to */
    /* send to the instrument.                                             */
    /*=====================================================================*/

    /*  Initialize the instrument to a known state.  */
    if ((rsvca_status = rsvca_writeASCII (instrSession, "*RST\r\n", 6, &retCnt)) < 0)
        return rsvca_status;

    if ((rsvca_status = rsvca_defaultInstrSetup (instrSession)) < 0)  
        return rsvca_status;
        
    /*=CHANGE: ==============================================================*/
    /* If the Instrument Does Not support a Reset, delete the above section  */
    /* that sends the Reset command  to the instrument and use the following */
    /* line of code to return the proper warning status code.                */
    /*                                                                       */
    /* rsvca_status = VI_WARN_NSUP_RESET;                                   */ 
    /*                                                                       */
    /* If a Reset is supported delete this section of comment and code.      */
    /*=======================================================================*/
       
    return rsvca_status;
}

/*===========================================================================*/
/* Function: Self-Test                                                       */
/* Purpose:  This function executes the instrument self-test and returns     */
/*           the result. If the self test function is not supported by the   */
/*           instrument, this function returns the warning                   */
/*           VI_WARN_NSUP_SELF_TEST.                                         */
/*===========================================================================*/
ViStatus _VI_FUNC rsvca_selfTest (ViSession instrSession, ViPInt16 testResult,
                    ViChar _VI_FAR testMessage[])
{
    ViUInt32 retCnt = 0;
    ViStatus rsvca_status = VI_SUCCESS;

    /*=CHANGE:=============================================================*/
    /* Change the *TST? command to the self-test command for your device.  */
    /*=====================================================================*/
    /* Test the test_message to see if a self-test error occurred. Assign  */
    /* the result to *test_result and return. Zero (0) means that the test */
    /* executed successfully any other number means selftest failed see    */
    /* control help for more details.                                      */
    /* This code makes the following assumption: The String returned from  */
    /* the instrument is in the format set forth in the SCPI 1994 Command  */
    /* Reference                                                           */
    /*=====================================================================*/
    if ((rsvca_status = viQueryf (instrSession, "%s", "%hd,\"%[^\"]", 
            "*TST?\r\n",  testResult, testMessage)) < 0)
        return rsvca_status;
    
    /*=CHANGE: ==============================================================*/
    /* If the Instrument Does Not support a Self Test, delete the above      */
    /* section that sends the Self Test command to the instrument and parses */
    /* the response and use the following line of code to return the proper  */ 
    /* warning status code.                                                  */
    /*                                                                       */
    /* rsvca_status = VI_WARN_NSUP_SELF_TEST;                               */
    /*                                                                       */ 
    /* If a Self Test is supported delete this section of comment and code.  */
    /*=======================================================================*/

    return rsvca_status;
}

/*===========================================================================*/
/* Function: Error Query                                                     */
/* Purpose:  This function queries the instrument error queue, and returns   */
/*           the result. If the error query function is not supported by the  */
/*           instrument, this function returns the warning                   */
/*           VI_WARN_NSUP_ERROR_QUERY.                                       */
/*===========================================================================*/
ViStatus _VI_FUNC rsvca_errorQuery (ViSession instrSession, ViPInt32 errorCode,
                    ViChar _VI_FAR errorMessage[])
{
    ViUInt32 retCnt = 0;
    ViStatus rsvca_status = VI_SUCCESS;

    /*=CHANGE:=============================================================*/
    /* Change the :SYST:ERR? command to the Error Query command for your   */
    /* device.                                                             */
    /*=====================================================================*/
    /* Parse the error message and return the Error Code and Error String. */
    /* This code makes the following assumption: The String returned from  */
    /* the instrument is in the format set forth in the SCPI 1994 Command  */
    /* Reference which is <Error/Event Number,"Error/Event Descriptor">,   */
    /* for example  0,"No Error" or -410,"Query Interrupted".              */
    /*=====================================================================*/
    if ((rsvca_status = viQueryf (instrSession, "%s", "%ld,\"%[^\"]", 
            ":SYST:ERR?\r\n", errorCode, errorMessage)) < 0)
        return rsvca_status;

    /*=CHANGE: ==============================================================*/
    /* If the Instrument Does Not support an Error Query, delete the above   */
    /* section that sends the Error Query command to the instrument and      */
    /* parses the response and use the following line of code to return the  */ 
    /* proper warning status code.                                           */
    /*                                                                       */
    /* rsvca_status = VI_WARN_NSUP_ERROR_QUERY;                             */
    /*                                                                       */ 
    /* If a Error Query is supported delete this section of comment and code.*/                                          
    /*=======================================================================*/

    return rsvca_status;
}

/*===========================================================================*/
/* Function: Error Message                                                   */
/* Purpose:  This function translates the error return value from the        */
/*           instrument driver into a user-readable string.                  */
/*===========================================================================*/
ViStatus _VI_FUNC rsvca_errorMessage (ViSession instrSession, ViStatus statusCode,
                    ViChar _VI_FAR message[])
{
    ViStatus rsvca_status = VI_SUCCESS;
    ViInt16 i;
    static rsvca_tStringValPair statusDescArray[] = {
        {VI_WARN_NSUP_ID_QUERY,     "WARNING: ID Query not supported"},
        {VI_WARN_NSUP_RESET,        "WARNING: Reset not supported"},
        {VI_WARN_NSUP_SELF_TEST,    "WARNING: Self-test not supported"},
        {VI_WARN_NSUP_ERROR_QUERY,  "WARNING: Error Query not supported"},     
        {VI_WARN_NSUP_REV_QUERY,    "WARNING: Revision Query not supported"},
        {VI_ERROR_PARAMETER1,   "ERROR: Parameter 1 out of range"},
        {VI_ERROR_PARAMETER2,   "ERROR: Parameter 2 out of range"},
        {VI_ERROR_PARAMETER3,   "ERROR: Parameter 3 out of range"},
        {VI_ERROR_PARAMETER4,   "ERROR: Parameter 4 out of range"},
        {VI_ERROR_PARAMETER5,   "ERROR: Parameter 5 out of range"},
        {VI_ERROR_PARAMETER6,   "ERROR: Parameter 6 out of range"},
        {VI_ERROR_PARAMETER7,   "ERROR: Parameter 7 out of range"},
        {VI_ERROR_PARAMETER8,   "ERROR: Parameter 8 out of range"},
        {VI_ERROR_FAIL_ID_QUERY,"ERROR: Identification query failed"},
        {VI_ERROR_INV_RESPONSE, "ERROR: Interpreting instrument response"},
        {VI_ERROR_INSTR_FILE_OPEN,    "ERROR: Opening the specified file"},
        {VI_ERROR_INSTR_FILE_WRITE,   "ERROR: Writing to the specified file"},
        {VI_ERROR_INSTR_INTERPRETING_RESPONSE, "ERROR: Interpreting the instrument's response"},
                
        /*=CHANGE:=============================================================*/
        /* Insert instrument-specific error codes here.  Example:              */
        /*=====================================================================*/
        {rsvca_ERROR_INVALID_CONFIGURATION, "ERROR: Instrument configuration error"},
        
        {VI_NULL, VI_NULL}
    };

    rsvca_status = viStatusDesc (instrSession, statusCode, message);
    if (rsvca_status == VI_WARN_UNKNOWN_STATUS) {
        for (i=0; statusDescArray[i].stringName; i++) {
            if (statusDescArray[i].stringVal == statusCode) {
                strcpy (message, statusDescArray[i].stringName);
                return (VI_SUCCESS);
            }
        }
        sprintf (message, "Unknown Error 0x%08lX", statusCode);
        return (VI_WARN_UNKNOWN_STATUS);
    }
    
    rsvca_status = VI_SUCCESS;
    return rsvca_status;
}

/*===========================================================================*/
/* Function: Revision Query                                                  */
/* Purpose:  This function returns the driver and instrument revisions.      */
/*           If the revision query function is not supported by the          */ 
/*           instrument, this function returns the warning                   */
/*           VI_WARN_NSUP_REV_QUERY.                                         */
/*===========================================================================*/
ViStatus _VI_FUNC rsvca_revisionQuery (ViSession instrSession,
                    ViChar _VI_FAR driverRev[], ViChar _VI_FAR instrRev[])
{
    ViUInt32 retCnt = 0;
    ViStatus rsvca_status = VI_SUCCESS;

    /*=CHANGE:=============================================================*/
    /* Query instrument firmware revision.                                 */
    /* Change the ID Query command to the ID Query command for your device.*/
    /*=====================================================================*/
    /* This code makes the following assumption: The String returned from  */
    /* the instrument is in the format set forth in the SCPI 1994 Command  */
    /* Reference                                                           */
    /*=====================================================================*/
    if ((rsvca_status = viQueryf (instrSession, "%s", "%*[^,],%*[^,],%*[^,],%[^\n]", 
            "*IDN?\r\n", instrRev)) < 0)
        return rsvca_status;

    /*=CHANGE: ==============================================================*/
    /* If the Instrument Does Not support a Revision Query, delete the above */
    /* section that sends the Revision Query command to the instrument and   */
    /* parses the response and use the following line of code to return the  */ 
    /* proper warning status code.                                           */
    /*                                                                       */
    /* rsvca_status = VI_WARN_NSUP_REV_QUERY;                               */
    /*                                                                       */
    /* If a Revision Query is supported delete this section of comment and   */
    /* code.                                                                 */
    /*=======================================================================*/
    
    strcpy (driverRev, rsvca_REVISION);
    
    return rsvca_status;
}

/*===========================================================================*/
/* Function: Reconfigure Interface                                           */
/* Purpose:  This function allows the user to change the Baud Rate, Parity,  */
/*           and Time Out of the serial interface.                           */
/*===========================================================================*/
ViStatus _VI_FUNC rsvca_reconfigInterface (ViSession instrSession, ViInt32 baudRate,
                                   ViInt16 parity, ViInt32 timeoutmSec)
{
    ViStatus rsvca_status = VI_SUCCESS;
    
    switch (baudRate) {
        case    110:
        case    150: 
        case    300: 
        case    600: 
        case   1200:
        case   2400: 
        case   4800: 
        case   9600: 
        case  19200:
        case  14400:
        case  28800:
        case  38400:
        case  56000:
        case  57600:
        case 115200:
        case 128000:
        case 256000:  
            break;
        default:
            rsvca_status = VI_ERROR_PARAMETER2;
            return rsvca_status;
    }
    if (rsvca_invalidViInt16Range (parity, 0, 4))
        return VI_ERROR_PARAMETER3;
    if (timeoutmSec != -1 && timeoutmSec != 0)
        if (rsvca_invalidViInt32Range (timeoutmSec, 1, 0x7fffffff))
            return VI_ERROR_PARAMETER4;
    
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_ASRL_BAUD, 
                                        (ViUInt32)baudRate)) < 0)
        return rsvca_status;                                       
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_ASRL_PARITY,
                                        (ViUInt16)parity)) < 0)
        return rsvca_status;                                       
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_TMO_VALUE,
                                        (ViUInt32)timeoutmSec)) < 0)
        return rsvca_status;                                       
    
    return rsvca_status;
}

/*===========================================================================*/
/* Function: Close                                                           */
/* Purpose:  This function closes the instrument.                            */
/*===========================================================================*/
ViStatus _VI_FUNC rsvca_close (ViSession instrSession)
{
    rsvca_instrRange instrPtr;
    ViSession rmSession;
    ViStatus rsvca_status = VI_SUCCESS;

    if ((rsvca_status = viGetAttribute (instrSession, VI_ATTR_RM_SESSION, &rmSession)) < 0)
        return rsvca_status;
    if ((rsvca_status = viGetAttribute (instrSession, VI_ATTR_USER_DATA, &instrPtr)) < 0)
        return rsvca_status;
    
    if (instrPtr != NULL) 
        free (instrPtr);
    
    rsvca_status = viClose (instrSession);
    viClose (rmSession);

    return rsvca_status;
}

/*****************************************************************************/
/*= UTILITY ROUTINES (Non-Exportable Functions) =============================*/
/*****************************************************************************/

/*===========================================================================*/
/* Function: Boolean Value Out Of Range - ViBoolean                          */
/* Purpose:  This function checks a Boolean to see if it is equal to VI_TRUE */
/*           or VI_FALSE. If the value is out of range, the return value is  */
/*           VI_TRUE, otherwise the return value is VI_FALSE.                */
/*===========================================================================*/
ViBoolean rsvca_invalidViBooleanRange (ViBoolean val)
{
    return ((val != VI_FALSE && val != VI_TRUE) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Short Signed Integer Value Out Of Range - ViInt16               */
/* Purpose:  This function checks a short signed integer value to see if it  */  
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range, the return value is VI_TRUE, otherwise the return     */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean rsvca_invalidViInt16Range (ViInt16 val, ViInt16 min, ViInt16 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Long Signed Integer Value Out Of Range - ViInt32                */
/* Purpose:  This function checks a long signed integer value to see if it   */  
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range, the return value is VI_TRUE, otherwise the return     */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean rsvca_invalidViInt32Range (ViInt32 val, ViInt32 min, ViInt32 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Unsigned Char Value Out Of Range - ViUInt8                      */
/* Purpose:  This function checks an unsigned char value to see if it        */  
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range, the return value is VI_TRUE, otherwise the return     */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean rsvca_invalidViUInt8Range (ViUInt8 val, ViUInt8 min, ViUInt8 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Short Unsigned Integer Value Out Of Range - ViUInt16            */
/* Purpose:  This function checks a short unsigned integer value to see if it*/  
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range, the return value is VI_TRUE, otherwise the return     */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean rsvca_invalidViUInt16Range (ViUInt16 val, ViUInt16 min, ViUInt16 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Long Unsigned Integer Value Out Of Range - ViUInt32             */
/* Purpose:  This function checks a long unsigned integer value to see if it */  
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range, the return value is VI_TRUE, otherwise the return     */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean rsvca_invalidViUInt32Range (ViUInt32 val, ViUInt32 min, ViUInt32 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Real (Float) Value Out Of Range - ViReal32                      */
/* Purpose:  This function checks a real (float) value to see if it lies     */  
/*           between a minimum and maximum value.  If the value is out of    */
/*           range, the return value is VI_TRUE, otherwise the return value  */
/*           is VI_FALSE.                                                    */
/*===========================================================================*/
ViBoolean rsvca_invalidViReal32Range (ViReal32 val, ViReal32 min, ViReal32 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Real (Double) Value Out Of Range - ViReal64                     */
/* Purpose:  This function checks a real (double) value to see if it lies    */  
/*           between a minimum and maximum value.  If the value is out of    */
/*           range, the return value is VI_TRUE, otherwise the return value  */
/*           is VI_FALSE.                                                    */
/*===========================================================================*/
ViBoolean rsvca_invalidViReal64Range (ViReal64 val, ViReal64 min, ViReal64 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Initialize Clean Up                                             */
/* Purpose:  This function is used only by the rsvca_init function.  When   */
/*           an error is detected this function is called to close the       */
/*           open resource manager and instrument object sessions and to     */
/*           set the instrSession that is returned from rsvca_init to       */
/*           VI_NULL.                                                        */
/*===========================================================================*/
ViStatus rsvca_initCleanUp (ViSession openRMSession,
                    ViPSession openInstrSession, ViStatus currentStatus)
{
    viClose (*openInstrSession);
    viClose (openRMSession);
    *openInstrSession = VI_NULL;
    
    return currentStatus;
}

/*===========================================================================*/
/* Function: Read To File From Instrument                                    */
/* Purpose:  This function is used to read data from the instrument and      */
/*           write it to a user specified file.                              */
/*===========================================================================*/
ViStatus rsvca_readToFile (ViSession instrSession, ViString filename,
                    ViUInt32 readBytes, ViPUInt32 retCount)
{
    ViStatus  rsvca_status = VI_SUCCESS;
    ViByte    buffer[BUFFER_SIZE];
    ViUInt32  bytesReadInstr = 0, bytesWrittenFile = 0;
    FILE     *targetFile;

    *retCount = 0L;
    if ((targetFile = fopen (filename, "wb")) == VI_NULL)
        return VI_ERROR_INSTR_FILE_OPEN; /* not defined by VTL */

    for (;;) {
        if (readBytes > BUFFER_SIZE)
            rsvca_status = viRead (instrSession, buffer, BUFFER_SIZE, &bytesReadInstr);
        else
            rsvca_status = viRead (instrSession, buffer, readBytes, &bytesReadInstr);

        bytesWrittenFile = fwrite (buffer, sizeof (ViByte), (size_t)bytesReadInstr, targetFile);
        *retCount += bytesWrittenFile;
        if (bytesWrittenFile < bytesReadInstr)
            rsvca_status = VI_ERROR_INSTR_FILE_WRITE; /* not defined by VTL */

        if ((readBytes <= BUFFER_SIZE) || (rsvca_status <= 0) || (rsvca_status == VI_SUCCESS_TERM_CHAR))
            break;

        readBytes -= BUFFER_SIZE;
    }

    fclose (targetFile);
    return rsvca_status;
}

/*===========================================================================*/
/* Function: Write From File To Instrument                                   */
/* Purpose:  This function is used to read data from a user specified file   */
/*           and write it to the instrument.                                 */
/*===========================================================================*/
ViStatus rsvca_writeFromFile (ViSession instrSession, ViString filename,
                    ViUInt32 writeBytes, ViPUInt32 retCount)
{
    ViStatus  rsvca_status = VI_SUCCESS;
    ViByte    buffer[BUFFER_SIZE];
    ViUInt32  bytesRead = 0, bytesWritten = 0;
    FILE     *sourceFile;
    ViBoolean sendEnd = VI_FALSE;

    *retCount = 0L;
    if ((sourceFile = fopen (filename, "rb")) == VI_NULL)
        return VI_ERROR_INSTR_FILE_OPEN; /* not defined by VTL */

    while (!feof (sourceFile)) {
        bytesRead = (ViUInt32)fread (buffer, sizeof (ViByte), BUFFER_SIZE, sourceFile);
        if ((writeBytes > BUFFER_SIZE) && (bytesRead == BUFFER_SIZE)) {
            viGetAttribute (instrSession, VI_ATTR_SEND_END_EN, &sendEnd);
            viSetAttribute (instrSession, VI_ATTR_SEND_END_EN, VI_FALSE);
            rsvca_status = viWrite (instrSession, buffer, BUFFER_SIZE, &bytesWritten);
            viSetAttribute (instrSession, VI_ATTR_SEND_END_EN, sendEnd);
            writeBytes -= BUFFER_SIZE;
            *retCount += bytesWritten;
            if (rsvca_status < 0)
                break;
        }
        else {
            rsvca_status = viWrite (instrSession, buffer, ((bytesRead < writeBytes) ? bytesRead : writeBytes), &bytesWritten);
            *retCount += bytesWritten;
            break;
        }
    }

    fclose (sourceFile);
    return rsvca_status;
}

/*=========================================================================*/
/* Function: Read ASCII Data                                               */
/* Purpose:  This function reads data in ASCII format from the instrument. */
/*           the read is terminated upon reaching MaxCount or upon receipt */
/*           of the termination character. The return value is equal to the*/
/*           global error variable. The only valid VI_ATTR_ASRL_END_IN     */
/*           attribute for this function is VI_ASRL_END_TERMCHAR. The      */
/*           Termination  character is set in the rsvca_init function.    */
/*=========================================================================*/
ViStatus rsvca_readASCII (ViSession instrSession, ViPByte rdBuffer, ViUInt32 cnt,
            ViPUInt32 retCnt)
{
    ViStatus  rsvca_status = VI_SUCCESS;
    ViUInt16 rdEnd = VI_ASRL_END_TERMCHAR;

    if ((rsvca_status = viGetAttribute (instrSession, VI_ATTR_ASRL_END_IN, &rdEnd)) < 0)
        return rsvca_status;                                       
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_ASRL_END_IN,
                                         VI_ASRL_END_TERMCHAR)) < 0)
        return rsvca_status;                                       

    if (rsvca_status = viRead (instrSession, rdBuffer, cnt, retCnt) < 0) {
        viSetAttribute (instrSession, VI_ATTR_ASRL_END_IN, rdEnd);
        return rsvca_status;
    }
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_ASRL_END_IN, rdEnd)) < 0)
        return rsvca_status;                                       

    return rsvca_status;
}


/*=========================================================================*/
/* Function: Read Binary Data                                              */
/* Purpose:  This function reads a buffer of data from the instrument. The */
/*           return value is equal to the global error variable.           */
/*           The valid VI_ATTR_ASRL_END_IN attributes for this function    */
/*           are VI_ASRL_END_LAST_BIT and VI_ASRL_END_NONE.                */
/*           If VI_ASRL_END_LAST_BIT is selected then the read will        */
/*           terminate as soon as a character arrives with its last bit    */
/*           set. If VI_ASRL_END_NONE is selected then the read will not   */
/*           terminate until all of the requested data is received (or an  */
/*           error occurs). If count equals -1 the function will find out  */
/*           how many bytes are at the serial port and read them all.      */
/*=========================================================================*/
ViStatus rsvca_readBinary (ViSession instrSession, ViUInt16 rdMode,
            ViPByte rdBuffer, ViUInt32 cnt, ViPUInt32 retCnt)
{
    ViStatus  rsvca_status = VI_SUCCESS;
    ViUInt16 rdEndMode = VI_ASRL_END_LAST_BIT;    
    
    if ((rsvca_status = viGetAttribute (instrSession, VI_ATTR_ASRL_END_IN, &rdEndMode)) < 0)
        return rsvca_status;                                       
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_ASRL_END_IN, rdMode)) < 0)
        return rsvca_status;                                       
    if (cnt == -1)
        if ((rsvca_status = viGetAttribute (instrSession, VI_ATTR_ASRL_AVAIL_NUM, &cnt)) < 0)
            return rsvca_status;                                       
    
    if (rsvca_status = viRead (instrSession, rdBuffer, cnt, retCnt) < 0) {
        viSetAttribute (instrSession, VI_ATTR_ASRL_END_IN, rdEndMode);
        return rsvca_status;
    }   
    
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_ASRL_END_IN, rdEndMode)) < 0) 
        return rsvca_status;
        
    return rsvca_status;
}

/*=========================================================================*/
/* Function: Write ASCII Data                                              */
/* Purpose:  This function Writes data in ASCII format to the instrument.  */
/*           the write is terminated upon reaching MaxCount. The return    */
/*           value is equal to the global error variable. The only valid   */
/*           VI_ATTR_ASRL_END_OUT attribute for this function is           */
/*           VI_ASRL_END_NONE.                                             */
/*=========================================================================*/
ViStatus rsvca_writeASCII (ViSession instrSession, ViPByte wrtBuffer, ViUInt32 cnt,
            ViPUInt32 retCnt)
{
    ViStatus  rsvca_status = VI_SUCCESS;
    ViUInt16 wrtEnd = VI_ASRL_END_NONE;

    if ((rsvca_status = viGetAttribute (instrSession, VI_ATTR_ASRL_END_OUT, &wrtEnd)) < 0)
        return rsvca_status;                                       
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_ASRL_END_OUT, VI_ASRL_END_NONE)) < 0)
        return rsvca_status;                                       

    if (rsvca_status = viWrite (instrSession, wrtBuffer, cnt, retCnt)) {
        viSetAttribute (instrSession, VI_ATTR_ASRL_END_OUT, wrtEnd);
        return rsvca_status;
    }
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_ASRL_END_OUT, wrtEnd)) < 0)
        return rsvca_status;                                       
             
    return rsvca_status;
}

/*=========================================================================*/
/* Function: Write Binary Data                                             */
/* Purpose:  This function writes a buffer of data to the instrument. The  */
/*           return value is equal to the global error variable.           */
/*           The valid VI_ATTR_ASRL_END_IN attributes for this function    */
/*           are VI_ASRL_END_LAST_BIT and VI_ASRL_END_BREAK                */
/*           If VI_ASRL_END_LAST_BIT is selected then the write will send  */
/*           all but the last character with the last bit clear, then      */
/*           transmit the last character with the last bit set.            */
/*           If VI_ASRL_END_BREAK is selected the write will transmit a    */
/*           break after all the characters for the write have been sent.  */
/*=========================================================================*/
ViStatus rsvca_writeBinary (ViSession instrSession, ViUInt16 wrtMode, 
            ViPByte wrtBuffer, ViUInt32 cnt, ViPUInt32 retCnt)
{
    ViStatus rsvca_status = VI_SUCCESS;
    ViUInt16 wrtEndMode = VI_ASRL_END_NONE;
    
    if ((rsvca_status = viGetAttribute (instrSession, VI_ATTR_ASRL_END_OUT, &wrtEndMode)) < 0)
        return rsvca_status;                                       
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_ASRL_END_OUT, wrtMode)) < 0)
        return rsvca_status;                                       
    
    if (rsvca_status = viWrite (instrSession, wrtBuffer, cnt, retCnt) < 0) {
        viSetAttribute (instrSession, VI_ATTR_ASRL_END_OUT, wrtEndMode);
        return rsvca_status;
    }   
    
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_ASRL_END_OUT, wrtEndMode)) < 0) 
        return rsvca_status;
        
    return rsvca_status;
}

/*****************************************************************************/
/*----------- INSERT INSTRUMENT-DEPENDENT UTILITY ROUTINES HERE -------------*/
/*****************************************************************************/

/*===========================================================================*/
/* Function: Default Instrument Setup                                        */
/* Purpose:  This function sends a default setup to the instrument.  This    */
/*           function is called by the rsvca_reset operation and by the     */
/*           rsvca_init function if the reset option has not been           */
/*           selected.  This function is useful for configuring any          */
/*           instrument settings that are required by the rest of the        */
/*           instrument driver functions such as turning headers ON or OFF   */
/*           or using the long or short form for commands, queries, and data.*/                                    
/*===========================================================================*/
ViStatus rsvca_defaultInstrSetup (ViSession instrSession)
{
    ViStatus rsvca_status = VI_SUCCESS;
    ViUInt32 retCnt = 0;
    rsvca_instrRange instrPtr;
    
    /* Determine if the structure has been initialized for the current VISA  */
    /* Session and malloc if it has not.                                     */
    if (rsvca_status = viGetAttribute (instrSession, VI_ATTR_USER_DATA, &instrPtr))
        return rsvca_status;
    
    if (instrPtr == NULL) 
        instrPtr = malloc (sizeof (struct rsvca_statusDataRanges));

    
    /*=CHANGE:=============================================================*/
    /* Change to reflect the global status variables that your driver needs*/
    /* to keep track of.  For example trigger mode of each session         */
    /*=====================================================================*/
    instrPtr -> triggerMode = 0;
    instrPtr -> val2 = 0;
    instrPtr -> val3 = 0;
    strcpy (instrPtr -> instrDriverRevision, rsvca_REVISION);
    
    if ((rsvca_status = viSetAttribute (instrSession, VI_ATTR_USER_DATA, 
                            (ViUInt32)instrPtr)) < 0)
        return rsvca_status;                                       

    /*=CHANGE:=============================================================*/
    /* Change "HEADERS OFF" to be the default setup command(s) you want to */
    /* send to your device.                                                */
    /*                                                                     */
    /* WARNING: This must either be changed to a valid command for your    */
    /* instrument or deleted.                                              */
    /*=====================================================================*/
    if ((rsvca_status = viWrite (instrSession, ":HEADERS OFF\r\n", 14, &retCnt)) < 0)
        return rsvca_status;

    return rsvca_status;
}

/*****************************************************************************/
/*=== END INSTRUMENT DRIVER SOURCE CODE =====================================*/
/*****************************************************************************/

void rsvca_setup_arrays (void)
{

    function[0] = "";		 // return loss
    function[1] = "";		 // amplitude spectrum
    function[2] = "IDC";
    function[3] = "IAC";
    function[4] = "RTW";
    function[5] = "RFW";
    function[6] = "TDC";
}
