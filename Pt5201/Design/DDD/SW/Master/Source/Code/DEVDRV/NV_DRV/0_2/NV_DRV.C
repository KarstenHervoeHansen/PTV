
#include "define.h"
//#include "clock.h"
#include "nv_drv.h"

#ifdef __BORLANDC__
 #include <stdio.h>
#endif

/* Write array of bytes from RAM 'source' to Non-volatile back up 'dest' */
/* Writing includes checksum update and two copies in bytewide NV-RAM    */
/* Calling modules needs to allocate NV-memory with a total of 4 times   */
/* the required datastorage plus 4 times the checksum size */


/* Memory allocation in nonvolatile RAM         */
/* Start address fixed in HW (PC fixed or file) */

/* In the top of the memory som fixed locations are used :*/
/* Real time clock (HW ) */
/* Some housekeping data with fixed allocation 
   (only accessed via function calls)          */
   
/* The bottom part is used as array of events  */


/* NV ram is 32 kbytes excluding 16 bytes reserved for HW RTC */
#define NV_size ((NvPointer)32*1024-16)

// Allocation definitions
#define NV_time (NV_size-16)


/* Checksum related definitions */
/* By changing CheckBase you can force all checksums i NV to fail */

typedef UI CheckType ;
#define CheckSumSize sizeof(CheckType)
#define CheckBase  0x1234

/* prototype of checksum calculator */
static CheckType CheckUpdate(CheckType sum,UC data);

/* Define pointer-type used as reference to NV_array*/
/* ( done in .h file ) */

/* declare  struct occupying 16 bits */
typedef struct halfwidth
{
   UC low;
// borland does not like array of 64 kbytes
#ifndef __BORLANDC__
   UC high;
#endif

}  NvByte;



#ifdef __BORLANDC__
static far NvByte NV_array[NV_size];
#else
static NvByte NV_array[NV_size] _at (0x80000) ;
#endif

//  Addressing : NV_array[Nvpointer].low


#define First_nv        0
/* Zero used as null pointer, allocation error => dont start alloc @ 0 */



#define AllocateStart   (EventLogStart+EventLogSize)
#define AllocateEnd     (NV_time-1)

#if (AllocateStart>20000)
#Not space for NV items
#endif

// Layout of fixed data

// Eventlog definitions
// Two pointers to eventlog

// Pointers to nonvolatile variables for eventlog :
//static NvPointer FirstEventP,CountEventP;
// Volatile memory locations :
//static UI        FirstEvent,CountEvent;


// allocation pointer
static NvPointer AllocTop;
#ifdef __BORLANDC__
FILE* fp;
#endif

int NV_Init(void)          /* Initialise NV hardware    */
{
   // PC version : load NV file ?


   //Allocate fixed part of nv_ram
   // Reset allocation pointers, REQUIRED before any allocation
   AllocTop = AllocateStart;

   // Allocation has to be in the same order if NV readings
   // are supposed to be identical to earlier writes

// Eventlog controls MOVED TO EVLOG.c
//   FirstEventP  = NV_Alloc(sizeof(FirstEvent));
//   CountEventP  = NV_Alloc(sizeof(CountEvent));

//   if ((OK==NV_Read(&FirstEvent,FirstEventP,sizeof(FirstEvent)))
//        && (OK == NV_Read(&CountEvent,CountEventP,sizeof(CountEvent)))  )
//   {

//   }
//   else
//   {
//      FirstEvent = 0;
//      CountEvent = 0;
//      NV_Write(&FirstEvent,FirstEventP,sizeof(FirstEvent));
//      NV_Write(&CountEvent,CountEventP,sizeof(CountEvent));
//   }

#ifdef __BORLANDC__
 if ((fp=fopen("NV.dat","rb"))==NULL)
 {
   fp=fopen("NV.dat","wb");	
   fwrite(&NV_array,NV_size,1,fp);
 }
 else
  fread(&NV_array,NV_size,1,fp);
 fclose(fp);
#endif

return (OK);
}






/* allocate preset memories using allocP */
NvPointer NV_AllocP(int numbytes)
{
   NvPointer loc;
   loc = AllocTop;
   if (AllocTop+numbytes+CheckSumSize <= AllocateEnd )
   {
      AllocTop = AllocTop+numbytes+CheckSumSize;
   }	
   else
   {
      /* Allocation error */
      loc = NvNull;
   }
   return(loc);
}

/* allocate memory for storing data with double & checksum */
NvPointer NV_Alloc(int numbytes)
{
   NvPointer loc;
   loc = AllocTop;
   if (AllocTop+2*numbytes+2*CheckSumSize <= AllocateEnd )
   {
      AllocTop = AllocTop+2*numbytes+2*CheckSumSize;
   }	
   else
   {
      /* Allocation error */
      loc = NvNull;
   }
   return(loc);
}

/* Store item in NV memory */
int NV_Write(void * source , NvPointer dest , int bytes)
{
   int i;
   UI checksum = CheckBase;
   UC *src_point = source;

   for (i=0;i<bytes ;i++,dest++,src_point++ )
   {
      NV_array[dest].low = *src_point;
      checksum = CheckUpdate(checksum,*src_point);
   }
   src_point =(UC*)&checksum;
   NV_array[dest++].low = *src_point;
   src_point++;
   NV_array[dest++].low = *src_point;

   /* Repeat for 2nd copy without recalculating checksum */
   src_point = source;
   for (i=0;i<bytes ;i++,dest++,src_point++ )
   {
      NV_array[dest].low = *src_point;
   }
   src_point = (UC*)&checksum;
   NV_array[dest++].low = *src_point;
   src_point++;
   NV_array[dest++].low = *src_point;

return(OK);
}

/* Read item from NV memory */
int NV_Read(void * dest , NvPointer source , int bytes)
{
   int i;
   CheckType checksum = CheckBase;
   CheckType testsum;
   UC *dst_point ;
	NvPointer srcP = source;

	// Checksum test
   for (i=0;i<bytes ;i++,srcP++)
   {
      //*dst_point =NV_array[source].low;
      checksum = CheckUpdate(checksum,NV_array[srcP].low);
   }
   dst_point =(UC*)&testsum;
   *dst_point = NV_array[srcP++].low;
   dst_point++;
   *dst_point = NV_array[srcP++].low;

	if (testsum != checksum)
	{
		// test 2nd copy
	   checksum = CheckBase;

		// Checksum test
	   for (i=0;i<bytes ;i++,srcP++)
	   {
	      //*dst_point =NV_array[source].low;
	      checksum = CheckUpdate(checksum,NV_array[srcP].low);
	   }
	   dst_point =(UC*)&testsum;
	   *dst_point = NV_array[srcP++].low;
	   dst_point++;
	   *dst_point = NV_array[srcP++].low;

		if (testsum != checksum)
		{
			// 2nd sum FAIL
			return(FAIL);
		}
		else
		{
			// 2nd copy works
			// Read data copy #2 and copy to #1
			srcP = source+bytes+2;
	      dst_point = dest;
		   for (i=0;i<bytes ;i++,srcP++,dst_point++)
		   {
		      *dst_point =NV_array[srcP].low;
		      NV_array[source++].low = NV_array[srcP].low;
		   }
			// copy checksum from 2nd to 1st
         NV_array[source++].low = NV_array[srcP++].low;
         NV_array[source++].low = NV_array[srcP++].low;

		   return(OK);
		}
	}
	else
	{
		srcP = source;
      dst_point = dest;
		// Read data copy #1
	   for (i=0;i<bytes ;i++,srcP++,dst_point++)
	   {
	      *dst_point =NV_array[srcP].low;
	   }
		 return(OK);
	}

}

// Store and recal presets  (single storage items with checksum ) */

int NV_Store(void * source , NvPointer dest   , int bytes)
/* Store item in NV memory */
{
   int i;
   UI checksum = CheckBase;
   UC *src_point = source;

   for (i=0;i<bytes ;i++,dest++,src_point++ )
   {
      NV_array[dest].low = *src_point;
      checksum = CheckUpdate(checksum,*src_point);
   }
   src_point =(UC*)&checksum;
   NV_array[dest++].low = *src_point;
   src_point++;
   NV_array[dest++].low = *src_point;

  
return(OK);
}

int NV_Recall(void * dest  , NvPointer source , int bytes)
/* Read item from NV memory */
{
   int i;
   CheckType checksum = CheckBase;
   CheckType testsum; 
   UC *dst_point ;
	NvPointer srcP = source;

	// Checksum test
   for (i=0;i<bytes ;i++,srcP++)
   {
      //*dst_point =NV_array[source].low;
      checksum = CheckUpdate(checksum,NV_array[srcP].low);
   }
   dst_point =(UC*)&testsum;
   *dst_point = NV_array[srcP++].low;
   dst_point++;
   *dst_point = NV_array[srcP++].low;

	if (testsum != checksum) 
	{
   	return(FAIL);
	}
	else
	{
		srcP = source;
      dst_point = dest;
	// Read data 
	   for (i=0;i<bytes ;i++,srcP++,dst_point++)
	   {
	      *dst_point =NV_array[srcP].low;
	   }
		 return(OK);
	}
}



static CheckType CheckUpdate(CheckType sum,UC data)
{
   return((sum ^ data) << 1 );
}

// PC version : update NV file
#ifdef __BORLANDC__

void NV_file_update(void)
{
 if ((fp=fopen("NV.dat","r+b"))!=NULL)
 {
   fwrite(&NV_array,NV_size,1,fp);
   fclose(fp);
 }
}

#endif
