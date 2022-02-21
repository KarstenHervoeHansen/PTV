/* rtxcbug.h - RTXCbug user configuration file */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "sysopts.h"

#define CONBUG /* #define if using CONsole as debug port */
#undef  COMBUG /* #define if using COM as debug port */

#ifdef CONBUG

#define CBUGOQ   CONOQ
#define CBUGIQ   CONIQ
#define CBUGRES  DOSRES
#define CBUGODRV CONODRV
#define CBUGIDRV CONIDRV

#endif /* CONBUG */

#ifdef COMBUG

#define CBUGOQ   COMOQ
#define CBUGIQ   COMIQ
#define CBUGRES  0           /* 0 resource implies no resource protection */
#define CBUGODRV COMODRV
#define CBUGIDRV COMIDRV

#endif /* COMBUG */

/* End of file - rtxcbug.h */
