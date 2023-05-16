#include <reg52.h>
/***************************************************************************/
/*    Serial_Init                                                         */
/*                                                                        */
/* Written by:        Sten A. Andersen                                    */
/*    Date:        950523                                                 */
/*    Revised:    950905                                                  */
/*    Rev.        0.02                                                    */
/*    Status accepted                                                     */
/* Module:        RS232ini.C                                              */
/***************************************************************************/
void Serial_Init( void)  {
      T2CON = 0x34;       /* 00110100 */
      RCAP2H = 65497 / 256;                /* Reload value = 65497 */
      RCAP2L = 65497 ;
      SCON = 0x90;        /* 01010000   Serial port mode 1 for 9615 kbit/sec */
      PCON = 0x00 ;       /* SMOD = 0; Not Bit-addressable */
      ES = 1;             /* Enables serial interrupt */
      PS = 1;             /* High prio. on serial interrupt  */

}
