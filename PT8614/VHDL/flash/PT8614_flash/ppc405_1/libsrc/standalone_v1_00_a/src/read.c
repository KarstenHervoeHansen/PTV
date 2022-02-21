////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2004 Xilinx, Inc.  All rights reserved. 
// 
// Xilinx, Inc. 
// XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
// COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS 
// ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
// STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
// IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
// FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION. 
// XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
// THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
// ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
// FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
// AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Id: read.c,v 1.3 2005/09/08 23:02:48 vasanth Exp $
////////////////////////////////////////////////////////////////////////////////

/* read.c -- read bytes from a input device.
 */

extern char inbyte(void);

/*
 * read  -- read bytes from the serial port. Ignore fd, since
 *          we only have stdin.
 */
int
read (int fd, char* buf, int nbytes)
{
  int i = 0;

  for (i = 0; i < nbytes; i++) {
    *(buf + i) = inbyte();
    if ((*(buf + i) == '\n') || (*(buf + i) == '\r')) {
      (*(buf + i + 1)) = '\r';
      (*(buf + i + 2)) = 0;
      break;
    }
  }
  return (i + 2);
}
