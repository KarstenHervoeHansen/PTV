/* Generic iic module */

/* History Rev 0_1 introduced in PT5765 pilot production */

/* Rev 1_1 has changes required for PT5765 production and for use in PT5201 project     */
/* Rev 1_1 Modify 15. Jan 2000 SAA                                                      */
/* Rev 1_1 Remove access to eventlog from this level                                    */
/* Rev 1_1 Add support for more busses                                                  */
/* Rev 1_1 Add support for enable/disable support for each individual bus               */


#include <stdio.h>       /* Using sprintf() */

#include "reg8051.h"
#include "define.h"     /* Standard PTV definitions */
#include "rtxcobj.h"    /* Kernal objects */

#include "config.h"     /* Include project specifics , Number of IIC-busses */

#include "i2c_drv.h"    /* Own h file */

// In file 'config.h' the specific project can define the number of busses supported
// By using only #define NiicBus either 1 (system) 4 or 7 busses are define
// if only a few selected busses should be included, then add a #define IicBusX for each bus


#if (NiicBus==0)
#compiletime error : This module should not be included if iic-busses not are used

#elif (NiicBus==1) //Include only system bus

#elif (NiicBus<=4) //Include 3 instrument busses
#define IicBus1
#define IicBus2
#define IicBus3

#else              // Include all supported busses

#define IicBus1
#define IicBus2
#define IicBus3
#define IicBus4
#define IicBus5
#define IicBus6

#endif





// Rev 1_1 Remove eventlog reference
// #include "EvLog.h"      /* Event (error) reporting funtions */

#include "EventDef.h"   /* Defines event IDs, used for error / status reporting */

/*  Module supports 4 busses : _ptv , _1, _2 and _3
    bus _ptv is the  local iic bus on the controller
    while busses _1, _2 and _3 are available for use on
    any instrument connector */


/* Define local PTV bus as direct pin or via FPGA */
#define FPGA 1
#define Direct 2
#define BusPTV Direct


/* Functions to support external :
   InitBus(UC bus)
   TransmitIIC(UC Address ,UC num ,UC *data ,UC bus )
   ReceiveIIC(UC Address ,UC num ,UC *data ,UC bus )
   */

/* Hardware dependent definitions for the BIT version */
/* Includes functions : */
/* Read SDA */
/* Read SCL */
/* Set SDA high or low */
/* Set SCL high or low */

/* Prototypes of local procedures */
//static void Delay(int usec);
static int SDA_get(UC bus);
static int SCL_get(UC bus);
static int SCL_set(UC level, UC bus);
static int SCL_set_wait(UC bus);
static int SDA_set(UC level, UC bus);
static int Start_IIC(UC bus);
static int Stop_IIC(UC bus);
static UC RxByte(int Last,UC bus);
static int TxByte(UC X,UC bus);


/* Delay of 1 to 5 microseconds */
/* Processor dependent delay of 5 usec */
/* XA controller running at 29.xx MHz  */
#define lopru 7        /* Number of loops executed each microsecond */

#define Delay(x)

//static void Delay(int usec)
//{
//    usec *= lopru;
//    while ( usec-- > 0 );
//}

/* Local defines */

#define High 1
#define Low  0
#define Bit7 0x80

#define IICtimeout 10    // Timeout in usec


/* Define 1+6 busses for IIC ports in FPGA  */

#if (BusPTV==FPGA)
volatile UC IICbit_0 _at(0x90012); /* For controller if via FPGA */
static UC   IICmir0;
#endif

#ifdef IicBus1
volatile UC IICbit_1 _at(0x90312); /* Instrument busses */
static UC   IICmir1;
#endif

#ifdef IicBus2
volatile UC IICbit_2 _at(0x90322);
static UC   IICmir2;
#endif

#ifdef IicBus3
volatile UC IICbit_3 _at(0x90332);
static UC   IICmir3;
#endif

#ifdef IicBus4
volatile UC IICbit_4 _at(0x90342);
static UC   IICmir4;
#endif

#ifdef IicBus5
volatile UC IICbit_5 _at(0x90352);
static UC   IICmir5;
#endif

#ifdef IicBus6
volatile UC IICbit_6 _at(0x90362);
static UC   IICmir6;
#endif


#define SDA_mask 0x02
#define SCL_mask 0x01

/* Define direct pins used for local (PTV) IIC */
_sfrbit    SCLpin  _atbit(P1,7);   // P1^7
_sfrbit    SDApin  _atbit(P1,6);   // P1^6

/* Functions for Reading SDA line */
/* Internal use only              */

static int SDA_get(UC bus)
{
   UC locvar;
   switch (bus)
   {
   default:
   case 0:

#if (BusPTV==FPGA)
      locvar = ( (IICbit_0 & SDA_mask) == 0 ? Low : High);
#elif (BusPTV==Direct)
      locvar = (SDApin == 0 ? Low : High);    /*pin reading on XA*/
#else /* Error : local bus not defined */
#compiletime error : Local bus not defined
#endif

   break;

#ifdef IicBus1
   case 1:
      locvar = ( (IICbit_1 & SDA_mask) == 0 ? Low : High);
   break;
#endif

#ifdef IicBus2
   case 2:
      locvar = ( (IICbit_2 & SDA_mask) == 0 ? Low : High);
   break;
#endif

#ifdef IicBus3
   case 3:
      locvar = ( (IICbit_3 & SDA_mask) == 0 ? Low : High);
   break;
#endif

#ifdef IicBus4
   case 4:
      locvar = ( (IICbit_4 & SDA_mask) == 0 ? Low : High);
   break;
#endif

#ifdef IicBus5
   case 5:
      locvar = ( (IICbit_5 & SDA_mask) == 0 ? Low : High);
   break;
#endif

#ifdef IicBus6
   case 6:
      locvar = ( (IICbit_6 & SDA_mask) == 0 ? Low : High);
   break;
#endif


   }
   return(locvar);
}

static int SCL_get(UC bus)
{
   UC locvar;
   switch (bus)
   {
   default:
   case 0:

#if (BusPTV==FPGA)
      locvar = ( (IICbit_0 & SCL_mask) == 0 ? Low : High);
#elif (BusPTV==Direct)
      locvar = (SCLpin == 0 ? Low : High);    /*pin reading on XA*/
#else /* Error : local bus not defined */
#compiletime error : Local bus not defined
#endif

   break;

#ifdef IicBus1
   case 1:
      locvar = ( (IICbit_1 & SCL_mask) == 0 ? Low : High);
   break;
#endif
#ifdef IicBus2
   case 2:
      locvar = ( (IICbit_2 & SCL_mask) == 0 ? Low : High);
   break;
#endif
#ifdef IicBus3
   case 3:
      locvar = ( (IICbit_3 & SCL_mask) == 0 ? Low : High);
   break;
#endif
#ifdef IicBus4
   case 4:
      locvar = ( (IICbit_4 & SCL_mask) == 0 ? Low : High);
   break;
#endif
#ifdef IicBus5
   case 5:
      locvar = ( (IICbit_5 & SCL_mask) == 0 ? Low : High);
   break;
#endif
#ifdef IicBus6
   case 6:
      locvar = ( (IICbit_6 & SCL_mask) == 0 ? Low : High);
   break;
#endif

   }
   return(locvar);
}




static int SCL_set(UC level, UC bus)
{
   switch (bus)
   {
   default:
   case 0:

#if (BusPTV==FPGA)
      if (level == High)
      {
         IICmir0   |=  SCL_mask;
         IICbit_0     = IICmir0;
      }
      else
      {
         IICmir0   &= ~SCL_mask;
         IICbit_0     = IICmir0;
      }

#elif (BusPTV==Direct)
      if (level == High)
          SCLpin = 1;
      else
          SCLpin = 0;
#else /* Error : local bus not defined */
#compiletime error : Local bus not defined
#endif

   break;

#ifdef IicBus1
   case 1:
      if (level == High)
      {
         IICmir1   |=  SCL_mask;
         IICbit_1     = IICmir1;
      }
      else
      {
         IICmir1   &= ~SCL_mask;
         IICbit_1     = IICmir1;
      }
   break;
#endif
#ifdef IicBus2
   case 2:
      if (level == High)
      {
         IICmir2   |=  SCL_mask;
         IICbit_2     = IICmir2;
      }
      else
      {
         IICmir2   &= ~SCL_mask;
         IICbit_2     = IICmir2;
      }
   break;
#endif
#ifdef IicBus3
   case 3:
      if (level == High)
      {
         IICmir3   |=  SCL_mask;
         IICbit_3     = IICmir3;
      }
      else
      {
         IICmir3   &= ~SCL_mask;
         IICbit_3     = IICmir3;
      }
   break;
#endif
#ifdef IicBus4
   case 4:
      if (level == High)
      {
         IICmir4   |=  SCL_mask;
         IICbit_4     = IICmir4;
      }
      else
      {
         IICmir4   &= ~SCL_mask;
         IICbit_4     = IICmir4;
      }
   break;
#endif
#ifdef IicBus5
   case 5:
      if (level == High)
      {
         IICmir5   |=  SCL_mask;
         IICbit_5     = IICmir5;
      }
      else
      {
         IICmir5   &= ~SCL_mask;
         IICbit_5     = IICmir5;
      }
   break;
#endif
#ifdef IicBus6
   case 6:
      if (level == High)
      {
         IICmir6   |=  SCL_mask;
         IICbit_6     = IICmir6;
      }
      else
      {
         IICmir6   &= ~SCL_mask;
         IICbit_6     = IICmir6;
      }
   break;
#endif
   }
   return(OK);  /* No errors possible */
}

static int SCL_set_wait(UC bus)
{
	UI i;
	SCL_set(High,bus);      /* Release clockline and */
	i = 0;              /* wait for physical line to go high */

	while ( (SCL_get(bus) == Low) && (i++ < IICtimeout) );

	if(i < IICtimeout)
		return(OK);
	else
		return(EventIICTimeout);
}








static int SDA_set(UC level, UC bus)
{
   switch (bus)
   {
   default:
   case 0:

#if (BusPTV==FPGA)
      if (level == High)
      {
         IICmir0   |=  SDA_mask;
         IICbit_0     = IICmir0;
      }
      else
      {
         IICmir0   &= ~SDA_mask;
         IICbit_0     = IICmir0;
      }

#elif (BusPTV==Direct)
      if (level == High)
          SDApin = 1;
      else
          SDApin = 0;
#else /* Error : local bus not defined */
#compiletime error : Local bus not defined
#endif



   break;

#ifdef IicBus6
   case 1:
      if (level == High)
      {
         IICmir1   |=  SDA_mask;
         IICbit_1     = IICmir1;
      }
      else
      {
         IICmir1   &= ~SDA_mask;
         IICbit_1     = IICmir1;
      }
   break;
#endif
#ifdef IicBus2
   case 2:
      if (level == High)
      {
         IICmir2   |=  SDA_mask;
         IICbit_2     = IICmir2;
      }
      else
      {
         IICmir2   &= ~SDA_mask;
         IICbit_2     = IICmir2;
      }
   break;
#endif
#ifdef IicBus3
   case 3:
      if (level == High)
      {
         IICmir3   |=  SDA_mask;
         IICbit_3     = IICmir3;
      }
      else
      {
         IICmir3   &= ~SDA_mask;
         IICbit_3     = IICmir3;
      }
   break;
#endif
#ifdef IicBus4
   case 4:
      if (level == High)
      {
         IICmir4   |=  SDA_mask;
         IICbit_4     = IICmir4;
      }
      else
      {
         IICmir4   &= ~SDA_mask;
         IICbit_4     = IICmir4;
      }
   break;
#endif
#ifdef IicBus5
   case 5:
      if (level == High)
      {
         IICmir5   |=  SDA_mask;
         IICbit_5     = IICmir5;
      }
      else
      {
         IICmir5   &= ~SDA_mask;
         IICbit_5     = IICmir5;
      }
   break;
#endif
#ifdef IicBus6
   case 6:
      if (level == High)
      {
         IICmir6   |=  SDA_mask;
         IICbit_6     = IICmir6;
      }
      else
      {
         IICmir6   &= ~SDA_mask;
         IICbit_6     = IICmir6;
      }
   break;
#endif
   }
   return(OK);  /* No errors possible */

}


static int Start_IIC(UC bus)
{
   /* Test for bus ready ( both lines high ) */
   if ((SCL_get(bus) == High) && (SDA_get(bus) == High))
   {
      SDA_set(Low,bus);
      Delay(5);
      SCL_set(Low,bus);
   }
   else /* Bus not ready => Start condition not generated */
   {
      return(EventIICBusError);
   }
   return(OK);
}

static int Stop_IIC(UC bus)
{
   Delay(2);                      /* prepare by setting dataline low */
   SDA_set(Low,bus);
   Delay(3);

   SCL_set_wait(bus);

   Delay(5);                     /* Setup time to stopcondition */

   SDA_set(High,bus);
   return(OK);
}

static int TxByte(UC X,UC bus) {

   int locvar;
   for (locvar = 8;locvar != 0; locvar-- )
   {
      SCL_set(Low,bus);            /* Clock line low */
      Delay(2);                   /* Small delay before changing data */

      if ((X & Bit7) == 0 )       /* Output databit */
         SDA_set(Low,bus);
      else
         SDA_set(High,bus);

      X += X;                     /* Get next databit in position   */
                                  /* Rotate left by adding to itself*/

      Delay(3);                   /* Data setup time to clock high */

      SCL_set_wait(bus);

      Delay(5);                   /* 5 usec clock high time */

   }
                        /* 8 bits are now out , get ready for acknowledge */

   SCL_set(Low,bus);            /* Clock line low */
   Delay(2);                   /* Small delay before changing data */
   SDA_set(High,bus);           /* Data line high : ready for input */
   Delay(3);                   /* Data setup time to clock high */

   SCL_set_wait(bus);

   if (SDA_get(bus) == Low)
   {                          /* Acknowledge OK */
      locvar = OK;
   }
   else
   {                          /* Acknowledge FAIL */
      locvar = FAIL;
   }

   Delay(5);                   /* 5 usec clock high time */
   SCL_set(Low,bus);            /* Clock line low */

   return(locvar);

}

static UC RxByte(int Last,UC bus) {

   int locvar;
   UC  datarx = 0;   /* Initialise datashifter */

   for (locvar = 8;locvar != 0; locvar-- )
   {
      Delay(5);

      SCL_set_wait(bus);

      datarx += datarx + SDA_get(bus);    /* Shift and input databit  */
      Delay(5);                      /* Clock high >= 5 microseconds */

      SCL_set(Low,bus);            /* Clock line low */
   }
   Delay(2);
   /* Acknowledge bit has to be set low if NOT last byte */
   /* and high if last byte */
   if(Last == TRUE)
      SDA_set(High,bus);
   else
      SDA_set(Low,bus);

   Delay(3);

   SCL_set_wait(bus);

   Delay(5);                   /* 5 usec clock high time */
   SCL_set(Low,bus);            /* Clock line low */

   Delay(2);                   /* Allways leave dataline high */
   SDA_set(High,bus);

   return(datarx);             /* Return the received byte */
}

/**********************/
/* External functions */
/* Bus initialisation */
/* Required before access to other functions */
// Arrays need to be sized after the highest number active

#define HighestBus 0
#define b0i FALSE

#ifdef IicBus1
#define b1i FALSE
#undef  HighestBus
#define HighestBus 1
#else
#define b1i EventIICNoBus
#endif

#ifdef IicBus2
#define b2i FALSE
#undef  HighestBus
#define HighestBus 2
#else
#define b2i EventIICNoBus
#endif

#ifdef IicBus3
#define b3i FALSE
#undef  HighestBus
#define HighestBus 3
#else
#define b3i EventIICNoBus
#endif

#ifdef IicBus4
#define b4i FALSE
#undef  HighestBus
#define HighestBus 4
#else
#define b4i EventIICNoBus
#endif

#ifdef IicBus5
#define b5i FALSE
#undef  HighestBus
#define HighestBus 5
#else
#define b5i EventIICNoBus
#endif

#ifdef IicBus6
#define b6i FALSE
#undef  HighestBus
#define HighestBus 6
#else
#define b6i EventIICNoBus
#endif


static int BusInitialised[7] = 
{b0i,b1i,b2i,b3i,b4i,b5i,b6i};

static   TResource *Res[HighestBus+1];

int InitBus(UC bus)
{
	UC i;
//	char report[16];


// Check for valid bus number :
   switch (bus)
   {
   case 0: // Bus 0 allways defined
   break;
#ifdef IicBus1
   case 1:
   break;
#endif
#ifdef IicBus2
   case 2:
   break;
#endif
#ifdef IicBus3
   case 3:
   break;
#endif
#ifdef IicBus4
   case 4:
   break;
#endif
#ifdef IicBus5
   case 5:
   break;
#endif
#ifdef IicBus6
   case 6:
   break;
#endif
   default:   // Undefined busses will end here
      return(EventIICNoBus+10*bus);
//   break;
   }


//Allocate resources for bus locking only first time 
// Try ignoring initial value :
   if (BusInitialised[bus] == FALSE)
//   if (BusInitialised[bus] !=EventNoMoreResources)
   {
      Res[bus] = newTResource(0);
      if (Res[bus]== NULL)
      {
         /* Error : No more resources , bus cannot initialise */
//Rev1_1 removed eventlogging from lowest level
//         LogEvent(EventNoMoreResources,NULL);
         BusInitialised[bus] = EventNoMoreResources;
      }
      else
      {
         BusInitialised[bus] = OK;
      }

   }
   else
   {  /* Repeated initialising */
//Rev1_1 removed eventlogging from lowest level
//      LogEvent(EventRepeatedInit,NULL);
   }

   if (BusInitialised[bus]==OK)
   {
   	 /* Release bus by setting SDA high and toggling SCL */
   	SDA_set(High,bus);
   	Delay(5);

   	for( i = 9 ; i != 0 ; i--  )
   	{
   		 SCL_set(Low,bus);
   		 Delay(5);
   		 SCL_set(High,bus);
   		 Delay(5);
   	}

   /* Test for actual high level on SCL then SDA */
   	 if ( SCL_get(bus) == Low ) {
   		  /* SCL Short to gnd */
   		  i |= 0x01;
   	 }
   	 if ( SDA_get(bus) == Low  ) {
   		  /* SDA Short to gnd */
   		  i |= 0x02;
   	 }

   /* Set SCL low and test for actual low level on SCL */
   	 SCL_set(Low,bus);
   	 if ( SCL_get(bus) == High ) {
   		  /* SCL Short to vcc */
   		  i |= 0x04;
   	 }
   	 /* Test for SDA low after setting SCL low (Short-circ. SDA-SCL) */
   	 if ( SDA_get(bus) == Low  ) {
   		  /* SDA Short to SCL */
   		  i |= 0x10;
   	 }
   	 /* Set SDA low and test for actual low level on SDA */

   	 SDA_set(Low,bus);
   	 SCL_set(High,bus);
       if ( SDA_get(bus) == High ) {
           /* SDA Short to vcc */
           i |= 0x08;
   	 }

   	 /* Release bus by setting SDA high and toggling SCL */
   	SDA_set(High,bus);
      Delay(5);

      SCL_set(Low,bus);
      Delay(5);
   	SCL_set(High,bus);
      Delay(5);

      SCL_set(Low,bus);
      Delay(5);
      SCL_set(High,bus);
      Delay(5);

   	Stop_IIC(bus);
      
      if(i != 0)
      {
//Rev1_1 removed eventlogging from lowest level
//         sprintf(report,"%u,%u",bus,i);
//         LogEvent(EventIICBusError,report);
         BusInitialised[bus] = EventIICBusError;
      }
      
   }
	return(BusInitialised[bus]);
}





int InitBusses(void)
{
        
   int Result=OK;

   if (InitBus(IIC_PTV) != OK)
      Result = FAIL;

// Instrument iic busses

#ifdef IicBus1
   if (InitBus(IIC_1)!= OK)
      Result = FAIL;
#endif
#ifdef IicBus2
   if (InitBus(IIC_2)!= OK)
      Result = FAIL;
#endif
#ifdef IicBus3
   if (InitBus(IIC_3)!= OK)
      Result = FAIL;
#endif
#ifdef IicBus4
   if (InitBus(IIC_4)!= OK)
      Result = FAIL;
#endif
#ifdef IicBus5
   if (InitBus(IIC_5)!= OK)
      Result = FAIL;
#endif
#ifdef IicBus6
   if (InitBus(IIC_6)!= OK)
      Result = FAIL;
#endif

   return(Result);
}   









int TransmitIIC(UC Address ,UC num ,UC *data ,UC bus )
{
UC i;
int status=EventNotInitialised;
	if (BusInitialised[bus] == OK) 
	{
      Res[bus]->Lock(Res[bus]);
    	if( status = Start_IIC(bus) )
    	{
       	status = TxByte(Address & ~0x01,bus);

         if (status == EventIICNoAck )
            status = EventIICNoAdrAck;
     
       	for (i=0;i<num;i++)
       	{
         if (status == OK)
         	 status = TxByte(data[i],bus);
         else
             TxByte(data[i],bus);
       	}
       	Stop_IIC(bus);
    	}
      else
      {
//Rev1_1 removed eventlogging from lowest level
//          LogEvent(status,NULL);
      }
      Res[bus]->Unlock(Res[bus]);
	}
   return(status);
}


int ReceiveIIC(UC Address ,UC num ,UC *data ,UC bus )
{
UC i;
	if (BusInitialised[bus] == OK) 
	{
      Res[bus]->Lock(Res[bus]);
   	Start_IIC(bus);
   	TxByte(Address | 0x01,bus);
   	for (i=0;i<num-1;i++)
   	{
   		data[i] = RxByte(FALSE,bus);
   	}
   	data[i] = RxByte(TRUE,bus);
   	Stop_IIC(bus);
      Res[bus]->Unlock(Res[bus]);
	}
   return(BusInitialised[bus]);
}

//  If uninterrupted writing / reading is required on the IIC bus
//  then the following functions could be implemented.
//  Normally this is done by the devicedriver by protecting each
//  device with the required resources.
//   TxAndRx(UC Address,UC rxnum, UC *rxdat,UC txnum, UC *txdat, UC bus)
//   RxAndTx(UC Address,UC rxnum, UC *rxdat,UC txnum, UC *txdat, UC bus)
