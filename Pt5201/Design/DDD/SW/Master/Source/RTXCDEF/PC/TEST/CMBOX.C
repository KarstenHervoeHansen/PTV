/* cmbox.c - RTXC Mailbox definitions - pt5765 rev.1         03/25/99 23:05 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cmbox.h"

extern const MBOX near nmboxes = NMBOXES;

MHEADER near mheader[1+NMBOXES];

#define NAMMAX 8

extern const char mboxname[1+NMBOXES][NAMMAX+1] =
{
   "",
   "COM_MBOX"
};

/* End of file - cmbox.c */
