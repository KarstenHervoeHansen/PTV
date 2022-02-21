/***************************************************************************/
/*  Copyright DK-Audio A/S, 2004                                           */
/*  Project:  PT8641 Trilevel HDTV generator (PT8612),                     */
/*            using 8051F320 Cygnal processor.                             */
/*  Module:   eeprom.c                                                     */
/*  Author:   Jnausz Kuzminski                                             */
/*  Date:     14.04.2004                                                   */
/*  Purpose:  This module implements functions to read and write onboard   */
/*            EEPROM memory of 24C08 type.                                 */
/*  Notes:                                                                 */
/*            Based on                                                     */
/*            "Interfacing SLx 24Cxx I 2 C-Bus Serial EEPROMs              */
/*             to 8051 Controller Family": ap083701.pdf                    */
/*                                                                         */
/*            This module implemets read and write functions described in  */
/*            Atmels                                                       */
/*                   AT24C02.pdf                                           */
/*                   "2-wire Serial EEPROM"      (1)                       */
/*            datasheet:                                                   */
/*            byte write, page write, acknowledge polling, current address */
/*            read, random read and sequential read.                       */
/*            Timing is done entirely in software, using NOPs.             */
/*            For 8051F231 running at 16.6 MHz the clock SCL is approx.    */
/*            200 kHz, for 16 NOPs in Delay16.  Acknowledge polling        */
/*            has a software timeout (WAIT_CYCLE), so functions will not   */
/*            hang in case of hardware failure.                            */
/*                                                                         */
/*   In case there is a fault on the bus, for instance, the SDA line is    */
/*   shorted to ground or pulled down to LOW by the slave device, this     */
/*   module will generate clock pulses until the line is released with the */
/*   time-out of 10 ms before returing a "HIGH" value from the I2cInit or  */
/*   I2cStop. For SCL line, it will monitor until the line is released     */
/*   with the time-out of 10 ms.                                           */
/*                                                                         */
/*   To use all 1 K of 24C08, outside program must control page bits P0    */
/*   and P1 in the "command" argument to functions in this module.         */
/*   See Figure 1. Device Address of (1)                                   */
/***************************************************************************/
#pragma WARNINGLEVEL (1) /* Lists only those warnings which may generate   */
                         /* incorrect code. Default is 2.                  */
/***************************************************************************/

#include <C8051F320.h>
#include "define.h"
#include <intrins.h>
#include "eeprom.h"

#include "io.c"

#define NOP _nop_();
#define Delay1 NOP
#define Delay16 { NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP}

#define PERIOD     2900   /* 2900 approximately 10 ms time out for bus faulty */

#define WAIT_CYCLE  100   /* 100 org  */

UI time_out;

void CheckClock(void);
UC   Check_SCL(void);

#if 0
/******************************************************************************/
/* Function:    ByteWriteE2prom                                               */
/*                                                                            */
/* Description: Write data byte to E2PROM                                     */        
/*                                                                            */
/*              Data format for writing into memory:                          */
/*              |ST|  CSW  |As|  EEA  |As|  DB  |As|SP|                       */
/*                                                                            */
/*              ST=START, CSW= command byte write,                            */
/*              As=acknowledge from slave, EEA=EEPROM address                 */
/*              DB=data byte to be sent, SP=stop                              */         
/*                                                                            */
/* Args :       (command) = command byte                                      */
/*              (address) = EEPROM address                                    */
/*              (*buffer) = location of data byte to be sent                  */
/*                                                                            */
/* Returns:     "0" - the programming is OK                                   */
/*              "1" - the data transfer has no acknowledge from slave         */                       
/*                                                                            */
/* Notes:       This function is illustrated on Figure 2. Byte Write of (1)   */
/******************************************************************************/
UC ByteWriteE2prom(UC command, UC address, UC *buffer)
  {
  I2cStart();                     /* START condition  */
  if (I2cMasterWrite(command))    /* command byte CSW (device address) */
    {
    I2cStop();
    return(1);        
    }
  if (I2cMasterWrite(address))    /* EEPROM address */        
    {
    I2cStop();
    return(1);
    }
  if (I2cMasterWrite(*buffer))    /* Data byte to be send */   
    {
    I2cStop();          
    return(1);
    }
  I2cStop();                      /* STOP condition */        
  return(0);
  }                
#endif

#if 1
/******************************************************************************/
/* Function:     PageWriteE2prom                                              */
/*                                                                            */
/* Description:  Write number of data bytes to E2PROM                         */        
/*                                                                            */
/*               Data format for writing into memory:                         */
/*               |ST|  CSW  |As|  EEA  |As| DB |As| DB |As|...|SP|            */
/*                                                                            */
/*               ST=START, CSW= command byte write,                           */
/*               As=acknowledge from slave, EEA=EEPROM address                */
/*               DB=data byte to be sent, SP=stop                             */         
/*                                                                            */
/* Args :        (command) = command byte                                     */
/*               (address) = EEPROM address                                   */
/*               (*buffer) = location of data byte to be sent                 */
/*               (count)   = number of byte to be send                        */
/*                                                                            */
/* Returns:      "0" - the programming is OK                                  */
/*               "1" - the data transfer has no acknowledge from slave        */                       
/*                                                                            */
/* Notes:        This function is illustrated on Figure 3. Page Write of (1)  */
/*               Up to 16 bytes can be written for 24C08                      */
/******************************************************************************/
UC PageWriteE2prom(UC command, UC address, UC *buffer, UC count)
  {
  UC i;
  
  I2cStart();                    /* START condition  */
  if (I2cMasterWrite(command))   /* command byte CSW  (device address) */
    {
    I2cStop();
    return(1);        
    }
  if (I2cMasterWrite(address))   /* EEPROM address */        
    {
    I2cStop();
    return(1);
    }
  for (i = 0; i < count; i++)    /* bytes of data to be sent */
    {
    if (I2cMasterWrite(*buffer))           
      {
      I2cStop();  
      return(1);
      }
    buffer++;
    }
  I2cStop();                     /* STOP condition */
  Ack_pol_r(command);            /* check for end of programming to E2PROM */
  return(0);
  }                
#endif
/******************************************************************************/
/* Function:     Ack_pol_r                                                    */
/*                                                                            */
/* Description:  Check for the completion of programming after the            */
/*               memory write with a read sequence. If the programming        */
/*               is completed, the acknowledge bit will be "0".               */        
/*                                                                            */
/*               Data format for writing into memory:                         */
/*                |ST|  CSR  |ST|  CSR  |......|As|DA|SP|                     */
/*                                                                            */
/*               ST=START, CSW= command byte write, As=acknowledge            */
/*               from slave, DA=data byte to be read, SP=stop                 */
/*                                                                            */ 
/*                                                                            */
/* Args :        (command) = command byte                                     */
/*                                                                            */
/* Return:        "0" - the programming is finished                           */
/*                "1" - the programming is not finished, an error             */
/*                      occured                                               */                       
/*                                                                            */
/******************************************************************************/
UC Ack_pol_r(UC command)
  {
  UC wait_cycle;        
  
  wait_cycle = WAIT_CYCLE;

  while (wait_cycle --)        
    {
    I2cStart();
    if (!I2cMasterWrite(command + 1)) /* wait if SCL is pulled down to LOW by */
      {                               /* slave device */
      I2cMasterRead(1); 
      I2cStop();
      return (0);
      }
    }
  return (1);    /* ERROR: SCL line is stuck to low */
  } 

/******************************************************************************/
/* Function:     Ack_pol_w                                                    */
/*                                                                            */
/* Description:  Check for the completion of programming after the            */
/*               memory write with a write sequence. If the programming       */
/*               is completed, the acknowledge bit will be "0".               */        
/*                                                                            */
/*               Data format for writing into memory:                         */
/*               |ST|  CSW  |ST|  CSW  |......|As|SP|                         */
/*                                                                            */
/*               ST=START, CSW= command byte write,                           */
/*               As=acknowledge from slave, SP=stop                           */
/*                                                                            */
/* Args :        (command) = command byte                                     */
/*                                                                            */
/* Return:       "0" - the programming is finished                            */
/*               "1" - the programming is not finished, an error              */
/*                     occured                                                */                       
/*                                                                            */
/******************************************************************************/
#if 0
UC Ack_pol_w(UC command)
  {
  UC wait_cycle;
  
  wait_cycle = WAIT_CYCLE;  /* wait_cycle at least 10mS */

  while (wait_cycle --)        
    {
    I2cStart();
    if (!I2cMasterWrite(command)) /* wait if SCL is pulled down to LOW by slave device */
      { 
      I2cStop();
      return (0);
      }
    }
  return (1);  /* ERROR: SCL line is stuck to low */
  } 
#endif

#if 0
/******************************************************************************/
/* Function:     RandomReadE2prom                                             */
/*                                                                            */        
/* Description:  Read number of data bytes from E2PROM                        */        
/*                                                                            */
/*               Data format for reading from the memory:                     */
/*               |ST| CSW |As| EEA |As|ST| CSR |As| DA |NAm|SP|               */
/*                                                                            */
/*               ST=START, CSW= command byte write,                           */
/*               As=acknowledge from slave, EEA=EEPROM address                */
/*               CSR=command byte read, NAm=no aknowledge from master         */
/*               DA=data byte to be read, SP=stop                             */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/* Args :        (command) = command byte                                     */
/*               (address) = EEPROM address                                   */
/*               (*buffer) = location of data to be stored                    */
/*                                                                            */
/*                                                                            */
/* Returns:      "0" - the reading of data is OK                              */
/*               "1" - the data transfer has no acknowledge from slave        */                       
/*                                                                            */
/* Notes:        This function is illustrated on Figure 5. Random Read of (1) */
/******************************************************************************/
UC RandomReadE2prom(UC command, UC  address, UC *buffer)
  {
  I2cStart();                    /* START condition  */
  
  if (I2cMasterWrite(command))   /* command byte CSW  (device address) */
    {
    I2cStop();
    return(1);
    }
  if (I2cMasterWrite(address))   /* EEPROM address */        
    {                            
    I2cStop();
    return(1);
    }
  I2cStart();                       /* START condition  */
  if (I2cMasterWrite(command + 1))  /* command byte CSR */
    {                                       
    I2cStop();
    return(1);
    }
  *buffer = I2cMasterRead(1);     /* byte read with "HIGH"/ No acknowlege */
  I2cStop();                      /* STOP condition */
  return(0);
  }
#endif

#if 0
/******************************************************************************/
/* Function:     CurrentReadE2prom                                            */
/*                                                                            */        
/* Description:  Read data byte from E2PROM without setting                   */
/*               the address counter                                          */
/*                                                                            */
/*                                                                            */
/*               Data format for reading from the memory:                     */
/*               |ST| CSR |As| DA |NAm|SP|                                    */
/*                                                                            */
/*               ST=START, As=acknowledge from slave,                         */
/*               CSR= command byte read, NAm=no aknowledge from master        */
/*               DA=data byte to be read, SP=stop                             */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/* Args :        command) = command byte                                      */
/*               (*buffer) = location of data to be stored                    */
/*                                                                            */
/* Returns:     "0" - the reading of data is OK                               */
/*              "1" - the data transfer has no acknowledge from slave         */                       
/*                                                                            */
/* Notes:       This function is illustrated on Figure 4. Current Address     */
/*              Read of (1)                                                   */
/******************************************************************************/
UC CurrentReadE2prom(UC command, UC *buffer)
  {
  I2cStart();                       /* START condition  */
  if (I2cMasterWrite(command + 1))   /* command byte CSW  (device address) */
    {                                       
    I2cStop();
    return(1);
    }
  *buffer = I2cMasterRead(1);     /*  byte read with "HIGH"/No acknowlege */
                               
  I2cStop();                      /* STOP condition */
  return(0);
  }
#endif 
/******************************************************************************/
/* Function:     SeqReadE2prom                                                */
/*                                                                            */        
/* Description:  Read number of data bytes from E2PROM                        */        
/*                                                                            */
/*                                                                            */
/*               Data format for reading from the memory:                     */
/*               |ST| CSW |As| EEA |As|ST| CSR |As| DA |Am|...| DA |NAm|SP|   */
/*                                                                            */
/*               ST=START, CSW= command byte write ,                          */
/*               As=acknowledge from slave, EEA=EEPROM address                */
/*               CSR= command byte read , Am=aknowledge from master,          */
/*               NAm=no aknowledge from master, DA=data byte to be read       */
/*               SP=stop                                                      */
/* Args :        (command) = command byte                                     */
/*               (address) = EEPROM address                                   */
/*               (*buffer) = location of data to be stored                    */
/*               (count)   = number of byte to be received                    */
/*                                                                            */
/* Return:       "0" - the reading of data is OK                              */
/*               "1" - the data transfer has no acknowledge from slave        */                       
/*                                                                            */
/* Notes:        This function is illustrated on Figure 6. Seq. Read of (1)   */
/******************************************************************************/
UC SeqReadE2prom(UC command, UC  address, UC *buffer, UC count)
  {
  UC i;
  
  I2cStart();                    /* START condition  */
  if (I2cMasterWrite(command))   /* command byte CSW  (device address) */
    {
    I2cStop();
    return(1);
    }
  if (I2cMasterWrite(address))         /* EEPROM address */
    {                            
    I2cStop();
    return(1);
    }
  I2cStart();                          /* START condition  */
    if (I2cMasterWrite(command + 1))   /* command byte CSR (read) */
    {                                       
    I2cStop();
    return(1);
    }

  for (i = 0; i < count - 1; i++) /* number of bytes to be read */
    {
    *buffer = I2cMasterRead(0);
    buffer++;
    }
  *buffer = I2cMasterRead(1);     /* last byte read with "HIGH"/No acknowlege */
  I2cStop();                      /* STOP condition */
  return(0);
  }

/******************************************************************************/
/*                         Low Level Functions                                */
/******************************************************************************/

/******************************************************************************/
/* Function:      CheckClock                                                  */
/*                                                                            */
/* Description:   Send HIGH and read the SCL line. It will wait until         */
/*                the line has been released from slave device for            */
/*                every bit of data to be sent or received.                   */
/*                                                                            */
/*      Args :    None                                                        */
/*                                                                            */
/*      Return:   None                                                        */
/*                                                                            */
/******************************************************************************/
#if 0
void CheckClock(void)
  {
  while (!SCL)  /* check for wait state before sending or receiving any data. */
    SCL = 1;
  }
#endif
/******************************************************************************/
/* Function:      Check_SCL                                                   */
/*                                                                            */
/* Description:   Send HIGH and read the SCL line. It will wait until         */
/*                the line has been released from slave device with the       */
/*                time out of approximately 10 ms.                            */        
/*                                                                            */
/* Args :         None                                                        */
/*                                                                            */
/* Return:        "0" - SCL line is OK                                        */
/*                "1" - SCL line is faulty                                    */
/*                                                                            */
/******************************************************************************/
UC Check_SCL(void)
  {
  time_out = PERIOD;    

  while (time_out--)        
    {
    if (!SCL)           /* wait if SCL is pulled down to LOW by slave device */
      {
      SCL = 1;          /* set clock to high */
      return (0);
      }
    }
  return (1);           /* ERROR: SCL line is stuck to low */
  }

/******************************************************************************/
/* Function:        I2cInit                                                   */
/*                                                                            */
/* Description:     Initialize the I2C bus                                    */        
/*                                                                            */
/* Args :           None                                                      */
/*                                                                            */
/* Return:          "0" - bus line is OK                                      */
/*                  "1" - bus line is faulty                                  */
/*                                                                            */
/******************************************************************************/
UC I2cInit(void)
  {
  return (0); /*********************************************/

  if (!SDA)          /* if lines are low, set them to high */
    if (I2cStop())
      return (1);

  if (!SCL) 
    if (I2cStop())
      return (1);
  
  return (0);
  }
                

/******************************************************************************/
/* Function:       I2cStart                                                   */
/*                                                                            */
/* Description:    Generate a START condition on I2C bus                      */        
/*                                                                            */
/* Args :          None                                                       */
/*                                                                            */
/* Return:         None                                                       */
/*                                                                            */
/******************************************************************************/
void I2cStart(void)
  {
  SDA = 1;        /* to make sure the SDA and SCL are both high */
  SCL = 1;
  Delay16;

  SDA = 0;        /* SDA line go LOW first */
  Delay16;
  SCL = 0;        /* then followed by SCL line with time delay */
  }

/******************************************************************************/
/* Subroutine:   I2cMasterWrite                                               */
/*                                                                            */
/* Description:  Output one byte of data to slave device. Check for           */
/*               WAIT condition before every bit is sent.                     */ 
/*                                                                            */
/* Args :        one byte of data to be sent to slave device.                 */
/*                                                                            */
/* Return:       acknowledgement from slave:                                  */
/*               0 = acknowledge is received                                  */
/*               1 = no acknowledge is received                               */                       
/*                                                                            */
/******************************************************************************/
UC I2cMasterWrite(UC input_byte)
  {
  UC mask, i;

  mask = 0x80;                    

  for (i = 0; i < 8; i++)     /* send one byte of data */
    {
    if (mask & input_byte)    /* send bit according to data */
      SDA = 1;
    else
      SDA = 0;
    mask = mask >> 1;         /* shift right for the next bit */
    Delay1;
    Check_SCL();             /* check SCL line */
    Delay16;        
    SCL = 0;                  /* clock is low */
    Delay16;
    }
        
  SDA = 1;                /* release SDA line*/
  Delay1;
  SCL = 1;                /* generate 9th clock pulse */
  Delay16;
  mask = SDA;             /* read acknowledge */
  SCL  = 0;               /* clock is low */
  Delay16;                /* to avoid short pulse transition on SDA line */
  return (mask);          /* return acknowledge bit */
  }        

/******************************************************************************/
/* Function:      I2cMasterRead                                               */
/*                                                                            */
/* Description:   Read one byte of data from the slave device. Check          */
/*                for WAIT condition before every bit is received.            */        
/*                                                                            */
/* Args :         Acknowledge require:                                        */
/*                0 - generate LOW  output after a byte is received           */
/*                1 - generate HIGH output after a byte is received           */
/*                                                                            */
/* Return:        received one byte of data from slave device                 */
/*                                                                            */                       
/*                                                                            */
/******************************************************************************/
UC I2cMasterRead(UC ack)
  {
  UC mask, i, rec_data;
  
  rec_data = 0;        
  mask     = 0x80;
  
  for (i = 0; i < 8; i++)
    {           
    Check_SCL();               /* clock is high */
    if (SDA)                    /* read data while clock is high */
      rec_data |= mask;
    mask = mask >> 1;
    SCL = 0;                    /* clock is low */
    Delay16;                                                                                      
    } 
  if (ack)                      /* set SDA data first before port direction */        
     SDA = 1;                   /* send acknowledge */
  else
     SDA = 0;
  Delay1;
  SCL = 1;              /* clock is high */
  Delay16;
  SCL = 0;              /* clock is low */
  SDA = 1;
  Delay16;              /* to avoid short pulse transition on SDA line */
  return (rec_data);
  }

/******************************************************************************/
/* Function:     I2cStop                                                      */
/*                                                                            */
/* Description:  generate stop condition on the I2C bus                       */        
/*                                                                            */
/* Args :        none                                                         */
/*                                                                            */
/* Return:       "0" - the bus line is OK                                     */
/*               "1" - the bus line has been pulled down to low               */                       
/*                                                                            */
/*                                                                            */
/******************************************************************************/
UC I2cStop(void)
  {
  time_out = PERIOD;      
  
  while (time_out--)
    {
    if (!SDA)                 /* check SDA line */
      {
      SCL = 1;                /* generate a clock pulse if SDA is pull */
      Delay16;                /* down to low */
      SCL = 0;
      Delay16;
      }
    else                      /* check SCL line  */
      {
      SDA  = 0;
      Delay1;
      if (Check_SCL())        /* to generate STOP condition */
        return (1);           /* ERROR: SCL line is stuck to low */
      Delay16;
      SDA = 1;
      Delay16;
      return (0);
      }
    }
  return (1);                 /* ERROR: SDA line is stuck to low */
  }
            
/* end of eeprom.c */
