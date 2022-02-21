/* cmbox.c - RTXC Mailbox definitions - Default             12/28/99 08:34 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cmbox.h"

const MBOX nmboxes = NMBOXES;

MHEADER ks_tblx mheader[1+NMBOXES];

#define NAMMAX 8

const char mboxname[1+NMBOXES][NAMMAX+1] =
{
   ""
};
