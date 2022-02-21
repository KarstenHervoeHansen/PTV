
#ifndef _NV_DRV_H
#define _NV_DRV_H


int NV_Init(void);          /* Initialise NV hardware    */

typedef unsigned int NvPointer;

#define NvNull 0

// Eventlog is fixed located :
#define EventLogStart   0
#define EventLogSize    4000

//#define MaxCountEvent   512
//#define EventLogItem    32
//#define EventLogSize    (EventLogItem * MaxCountEvent)



// This overhead are moved to private part of nv_
#define NV_overhead   5    /* Physical number of checksum bytes + .. */


// Allocation routines.
// Allocation has to be done in a fixed sequence (Otherwise backup data 
// is not at allocated location)

/* Allocate memory for presets  (single storage w/checksum) */
/* Access by NV_Store and NV_Recall */
/* Allocation error returns NvNull */

NvPointer NV_AllocP( int numbytes);

int NV_Store( void *source, NvPointer dest, int bytes);

int NV_Recall( void *dest, NvPointer source, int bytes);

/* allocate memory for storing data with double store & checksum */
/* Access by NV_Write and NV_Read */
/* Allocation error returns NvNull */

NvPointer NV_Alloc( int numbytes);

/* Write array of bytes from RAM 'source' to Non-volatile back up 'dest' */
/* Writing includes checksum update and two copies in bytewide NV-RAM    */
/* Calling modules needs to allocate NV-memory using NV_Alloc()          */

int NV_Write( void *source, NvPointer dest, int bytes);

/* Reads array of bytes from Non-volatile RAM 'source' to RAM 'dest' */
/* Reading includes checksum check and returns only 'OK' if a valid  */
/* backup was found and copied                                       */
/* In case of fail 'dest' is not modified                            */

int NV_Read( void *dest, NvPointer source, int bytes);

//void NV_file_update( void);  		// REMOVED KEn 000412

#endif