/****************************************************************************/
/* MODULE:                                                                  */
/*   comn_api.h - RTXCio common header function prototypes.                 */
/****************************************************************************/
/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef _COMN_API_H_  /* { _COMN_API_H_ */
#define  _COMN_API_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/*------------------------*/
/* MUST ALWAYS BE DEFINED */
/*------------------------*/

IORC  IO_chnl_close( CHNLID );
CHNLID  IO_chnl_open( DEVID, IOTYPE, IOCOMMAND, INITCHNL * );
const char *  IO_version(void);

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _COMN_API_H_ */

/* End of the COMN_API.H module */
