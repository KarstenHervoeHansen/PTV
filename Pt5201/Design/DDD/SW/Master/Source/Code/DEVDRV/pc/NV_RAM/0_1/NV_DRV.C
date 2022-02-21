
#include "define.h"
#include "nv_drv.h"

int NV_Init(void)          /* Initialise NV hardware    */
{

return (OK);
}

/* Write array of bytes from RAM 'source' to Non-volatile back up 'dest' */
/* Writing includes checksum update and two copies in bytewide NV-RAM    */
/* Calling modules needs to allocate NV-memory with a total of 4 times   */
/* the required datastorage plus 4 times the checksum size */

int NV_Write(void * source , void * dest , int bytes)
{
return(OK);
}

int NV_Read(void * dest , void * source , int bytes)
{

return(FAIL);
}

