
/* CHANGE: Replace <Instrument Model Name> with your instrument model name.  */
/*= <Instrument Model Name> Include File ====================================*/

/*===========================================================================*/
/*  Please do not use global variables or arrays in the include file of      */
/*  instrument drivers that will be submitted for inclusion into the         */
/*  LabWindows Instrument Driver Library.                                    */
/*===========================================================================*/

#ifndef __rsvca_HEADER
#define __rsvca_HEADER

#include <vpptype.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*****************************************************************************/
/*= Define Instrument Specific Error/Warning Codes Here =====================*/
/*****************************************************************************/
#define VI_ERROR_INSTR_FILE_OPEN                (_VI_ERROR+0x3FFC0800L)
#define VI_ERROR_INSTR_FILE_WRITE               (_VI_ERROR+0x3FFC0801L)
#define VI_ERROR_INSTR_INTERPRETING_RESPONSE    (_VI_ERROR+0x3FFC0803L)

#define VI_INSTR_WARNING_OFFSET                           (0x3FFC0900L)
#define VI_INSTR_ERROR_OFFSET                   (_VI_ERROR+0x3FFC0900L)

#define rsvca_ERROR_INVALID_CONFIGURATION      (VI_INSTR_ERROR_OFFSET + 0xF0L)

/*****************************************************************************/
/*= GLOBAL USER-CALLABLE FUNCTION DECLARATIONS (Exportable Functions) =======*/
/*****************************************************************************/
ViStatus _VI_FUNC rsvca_init (ViRsrc resourceName, ViBoolean IDQuery, 
                    ViBoolean resetDevice, ViInt32 baudRate, ViInt16 parity, 
                    ViInt32 timeoutmSec, ViPSession instrSession);

/** INSERT USER-CALLABLE INSTRUMENT-DEPENDENT FUNCTION DECLARATIONS HERE **/

ViStatus _VI_FUNC rsvca_writeInstrData (ViSession instrSession, ViString writeBuffer);
ViStatus _VI_FUNC rsvca_readInstrData (ViSession instrSession,
                    ViInt32 numberBytesToRead, ViChar _VI_FAR readBuffer[], ViPInt32 numBytesRead);
ViStatus _VI_FUNC rsvca_reset (ViSession instrSession);
ViStatus _VI_FUNC rsvca_selfTest (ViSession instrSession, ViPInt16 selfTestResult,
                    ViChar _VI_FAR selfTestMessage[]);
ViStatus _VI_FUNC rsvca_errorQuery (ViSession instrSession, ViPInt32 errorCode,
                    ViChar _VI_FAR errorMessage[]);
ViStatus _VI_FUNC rsvca_errorMessage (ViSession instrSession, ViStatus statusCode,
                    ViChar _VI_FAR message[]);
ViStatus _VI_FUNC rsvca_revisionQuery (ViSession instrSession,
                    ViChar _VI_FAR instrumentDriverRevision[], ViChar _VI_FAR firmwareRevision[]);
ViStatus _VI_FUNC rsvca_reconfigInterface (ViSession instrSession, ViInt32 baudRate,
                                   ViInt16 parity, ViInt32 timeoutmSec);
ViStatus _VI_FUNC rsvca_close (ViSession instrSession);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

/*****************************************************************************/
/*=== END INCLUDE FILE ======================================================*/
/*****************************************************************************/

#endif
