/***************************************************************************/
/*  Copyright DK-Audio A/S, 2004                                           */
/*  Project:  PT8641 Trilevel HDTV generator (PT8612),                     */
/*            using 8051F320 Cygnal processor.                             */
/*  File:     io.c                                                         */
/*  Author:   Jnausz Kuzminski                                             */
/*  Date:     23.02.2006                                                   */
/*  Purpose:  To define IO pins for all modules.                           */
/*  Notes:    LEVOK added on 19.04.2006                                    */
/***************************************************************************/
sbit SCK   = P0^0; /* Clock for the SPI comms interface to the FPGA   */
sbit MISO  = P0^1; /* MISO  for the SPI comms interface to the FPGA   */
sbit MOSI  = P0^6; /* MOSI  for the SPI comms interface to the FPGA   */
sbit LEVOK = P0^7; /* OK2 signal for level error signal to Master     */
sbit CS    = P1^7; /* CS    for the SPI comms interface to the FPGA   */

sbit SDA   = P1^0; /* Data  for IIC comms interface to serial EEPROM  */
sbit SCL   = P1^1; /* Clock for IIC comms interface to serial EEPROM  */

#if 1
sbit LED   = P1^2;  /* Test LED on 8612 */
#else
sbit LED    = P2^2; /* Test LED on KIT  */
#endif
