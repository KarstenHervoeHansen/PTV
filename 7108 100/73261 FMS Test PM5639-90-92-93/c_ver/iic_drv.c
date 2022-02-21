 /*
 ; FILE NAME        iic_drv.c
 ; PURPOSE          Module containing IIC routines to control IIC
 ;                  link in 5639 calibration hardware setup (light box)
 ; SPEC             Peter Frederiksen
 ; DESIGN           Peter Frederiksen
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST
 ; LAST UPDT:       26.02.2004
 ;
 ; NOTES:           Functions in this module are directly translated 
 ;                  from Pascal source UNIT_IIC.PAS
 ;                  Timeout procedures are added to all wait routines.
 ;
*/
#include <dos.h>
#include "iic_drv.h"

#define DEL                 20
#define IIC_TIMEOUT (long)  500

typedef struct
   {
   long delay;           /* time to wait */
   struct timeb start;   /* start time   */
   } chrono_t;

int  end_chrono (chrono_t);
void start_chrono (chrono_t *, long);

static long iic_timeout;

/**********************************************************************
 * NAME       int init_iic(int addr)
 * PURPOSE    To initialize PC IIC card.
 *            
 * ARGUMENTS  int addr - address of PCD8584 (U1) parallel to IIC IC
 *                          on IIC card.
 *
 * RETURNS    0 if timeout on writing to PCD8584,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     See drawing "Light Box Control for PM5639" and 
 *            "IIC interface for PC"
 **********************************************************************/
int init_iic(int addr)
  {
  UC stat;
  chrono_t timeout;

  if ((addr < 0x300) | (addr > 0x31C))
    return (0);

  outportb(addr + 1, 0x00);
  delay(DEL);
  outportb(addr,     0x01);
  delay(DEL);
  outportb(addr + 1, 0x20);
  delay(DEL);
  outportb(addr,     0x1A);
  delay(DEL);
  outportb(addr + 1, 0x40);

  iic_timeout = IIC_TIMEOUT;

  start_chrono (&timeout, iic_timeout);
  stat = inportb(addr + 1);
  while ((stat & 0x01) == 0)    /* vent til BB = 1 */
    {
    delay(DEL);
    stat = inportb(addr + 1);
    if (end_chrono(timeout))
      return (0);
    }
  return (trm8574(addr, 64, 0xFF)); /* lamp in light box off */
  }

/**********************************************************************
 * NAME       int trmDA8591(int io_addr, UC addr, UC dat)
 * PURPOSE    To send data to PCF8591 DA converter in light box, which
 *            controls light intensity.
 *            
 * ARGUMENTS  int io_addr - address of PCD8584 (U1) parallel to IIC IC
 *                          on IIC card.
 *            UC addr     - address of PCF8591 DA converter in light box
 *            UC dat      - data to send
 *
 * RETURNS    0 if timeout on writing to PCD8584,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     See drawing "Light Box Control for PM5639" and 
 *            "IIC interface for PC"
 **********************************************************************/

int trmDA8591(int io_addr, UC addr, UC dat)
  {
  UC stat;
  chrono_t timeout;

  outportb(io_addr + 1, 0x40);
  delay(DEL);
  outportb(io_addr + 1, 0x41);
  delay(DEL);
  outportb(io_addr,     addr);
  delay(DEL);
  outportb(io_addr + 1, 0x45);

  start_chrono (&timeout, iic_timeout);
  do
    {
    delay(DEL);
    stat = inportb(io_addr + 1);
    if (end_chrono(timeout))
      return (0);
    } while ((stat & 0x80) == 0x80);    /* vent til PIN = 0 */

  outportb(io_addr, 0x40);         /* enable output fra D/A PCF8591 */

  start_chrono (&timeout, iic_timeout);
  do
    {
    delay(DEL);
    stat = inportb(io_addr + 1);
    if (end_chrono(timeout))
      return (0);
    } while ((stat & 0x80) == 0x80);    /* vent til PIN = 0 */

  outportb(io_addr, dat);          /* output data */
  start_chrono (&timeout, iic_timeout);
  do
    {
    delay(DEL);
    stat = inportb(io_addr + 1);
    if (end_chrono(timeout))
      return (0);
    } while ((stat & 0x80) == 0x80);    /* vent til PIN = 0 */
  
  delay(DEL);
  outportb(io_addr + 1, 0x43);     /* STOP: ES0=1  STA=0  STO=1  ACK=1 */
  delay(DEL);
  outportb(io_addr, 0x00);     /* dummy SDA og SCL = high */
  delay(DEL);
  return (1);
  }


/**********************************************************************
 * NAME       int trm8574(int ioaddr, UC addr, UC dat)
 * PURPOSE    To send data to PCF8574 port in light box, which
 *            controls lamp ON/OFF.
 *            
 * ARGUMENTS  int io_addr - address of PCD8584 (U1) parallel to IIC IC
 *                          on IIC card.
 *            UC addr     - address of PCF8574 port in light box
 *            UC dat      - data to send.
 *
 * RETURNS    0 if timeout on writing to PCD8584,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     See drawing "Light Box Control for PM5639" and 
 *            "IIC interface for PC"
 **********************************************************************/
int trm8574(int ioaddr, UC addr, UC dat)
  {
  UC stat;
  chrono_t timeout;

  outportb(ioaddr + 1, 0x40);   /* IDLE: ES0=1  STA=0  STO=0  ACK=0 */

  start_chrono (&timeout, iic_timeout);
  stat = inportb(ioaddr + 1);
  while ((stat & 0x01) == 0)    /* vent til BB = 1 */
    {
    delay(DEL);
    stat = inportb(ioaddr + 1);
    if (end_chrono(timeout))
      return (0);
    }
  delay(DEL);
  outportb(ioaddr + 1, 0x41);  /* START: ES0=1  STA=0  STO=0  ACK=1 */
  delay(DEL);
  outportb(ioaddr, addr);
  delay(DEL);
  outportb(ioaddr + 1, 0x45);  /* START: ES0=1  STA=1  STO=0  ACK=1 */
  delay(DEL);

  stat = inportb(ioaddr + 1);
  start_chrono (&timeout, iic_timeout);
  do
    {
    delay(DEL);
    stat = inportb(ioaddr + 1);
    if (end_chrono(timeout))
      return (0);
    } while ((stat & 0x80) == 0x80);    /* vent til PIN = 0 */

  delay(DEL);
  outportb(ioaddr, dat);     /* send data */
  delay(DEL);

  stat = inportb(ioaddr + 1);
  start_chrono (&timeout, iic_timeout);
  do
    {
    delay(DEL);
    stat = inportb(ioaddr + 1);
    if (end_chrono(timeout))
      return (0);
    } while ((stat & 0x80) == 0x80);    /* vent til PIN = 0 */

  delay(DEL);
  outportb(ioaddr + 1, 0x43);   /* STOP: ES0=1  STA=0  STO=1  ACK=1 */
  delay(DEL);
  outportb(ioaddr, 0x00);       /* dummy SDA og SCL = high */
  delay(DEL);
  delay(DEL);
  return (1);
  }

/*******   Routines below are NOT tested!    ********************/

int read8574(int io_addr, UC addr, UC *dat)
  {
  UC stat;

  io_addr++;
  delay(DEL);
  outportb(io_addr + 1, 0x41);
  delay(DEL);
  outportb(io_addr, addr);
  delay(DEL);
  outportb(io_addr + 1, 0x45);
  delay(DEL);

  do
    {
    delay(DEL);
    stat = inportb(io_addr + 1);
    } while(!(stat & 0x80));  /* vent til PIN = 0 */

  *dat = inportb(io_addr);     /* start reading */
  do
    {
    delay(DEL);
    stat = inportb(io_addr + 1);
    } while(!(stat & 0x80));  /* vent til PIN = 0 */
  delay(DEL);
  outportb(io_addr + 1, 0x40);/* IDLE:  ES0=1 STA=0 STO=0 ACK=0 */
  delay(DEL);
  *dat = inportb(io_addr);    /* read valid data */
  do
    {
    delay(DEL);
    stat = inportb(io_addr + 1);
    } while(!(stat & 0x80));  /* vent til PIN = 0 */
  delay(DEL);
  outportb(io_addr + 1, 0x42);  /* STOP:  ES0=1,  STA=0  STO=1  ACK=0 */
  delay(DEL);
  return ((int)stat);
  }


int trmAD8591(int ioaddr, int addr, UC dat)
  {
  UC stat;
  chrono_t timeout;

  outportb(ioaddr + 1, 0x40);
  delay(DEL);
  outportb(ioaddr + 1, 0x41);
  delay(DEL);
  outportb(ioaddr, addr);
  delay(DEL);
  outportb(ioaddr + 1, 0x45);

  start_chrono (&timeout, iic_timeout);
  do
    {
    delay(DEL);
    stat = inportb(ioaddr + 1);
    if (end_chrono(timeout))
      return (0);
    } while (!(stat & 0x80)); /* vent til PIN = 0 */

  delay(DEL);
  outportb(ioaddr, dat); /* Control byte to PCF8591 */

  start_chrono (&timeout, iic_timeout);
  do
    {
    delay(DEL);
    stat = inportb(ioaddr + 1);
    if (end_chrono(timeout))
      return (0);
    } while (!(stat & 0x80)); /* vent til PIN = 0 */
  delay(DEL);
  outportb(ioaddr + 1, 0x43);
  delay(DEL);
  outportb(ioaddr, 0x00);
  delay(DEL);
  return (1);
  }

int read8591(int ioaddr, int addr, UC *dat)
  {
  UC stat;

  addr++;
  delay(DEL);
  outportb(ioaddr + 1, 0x41);
  delay(DEL);
  outportb(ioaddr, addr);
  delay(DEL);
  outportb(ioaddr + 1, 0x45);
  do
    {
    delay(DEL);
    stat = inportb(ioaddr + 1);
    } while (!(stat & 0x80)); /* vent til PIN = 0 */
  *dat = inportb(ioaddr);     /* start reading */
  do
    {
    delay(DEL);
    stat = inportb(ioaddr + 1);
    } while (!(stat & 0x80)); /* vent til PIN = 0 */
  delay(DEL);
  outportb(ioaddr + 1, 0x40);
  delay(DEL);
  *dat = inportb(ioaddr); /* Read last data */
  do
    {
    delay(DEL);
    stat = inportb(ioaddr + 1);
    } while (!(stat & 0x80)); /* vent til PIN = 0 */
  delay(DEL);
  *dat = inportb(ioaddr);  /* Read valid data */
  do
    {
    delay(DEL);
    stat = inportb(ioaddr + 1);
    } while (!(stat & 0x80)); /* vent til PIN = 0 */
  delay(DEL);
  outportb(ioaddr + 1, 0x42);  /* STOP:  ES0=1,  STA=0  STO=1  ACK=0 */
  delay(DEL);
  outportb(ioaddr, 0x00); /* dummy SCL of SDA = high */
  delay(DEL);
  return ((int)stat);
  }




/* initialize chronometer */
static void start_chrono (chrono_t *c, long t)
  {
  ftime (&(c->start));
  c->delay = t;
  }

/* return TRUE if time has expired */
static int end_chrono (chrono_t c)
  {
  struct timeb temp;
  long t;

  ftime (&temp);
  /* convert difference to milliseconds */ 
  t = (temp.time - c.start.time) * 1000 + ((long)temp.millitm - (long)c.start.millitm);

  return (t >= c.delay);
  }
