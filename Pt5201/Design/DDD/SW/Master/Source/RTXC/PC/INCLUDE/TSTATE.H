/* tstate.h - RTXC task status definitions */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _TSTATE_H
#define _TSTATE_H

#define INACTIVE       0x100 /* task not yet executed, or terminated */
#define QUEUE_WAIT     0x080 /* queue */
#define SEMAPHORE_WAIT 0x040 /* semaphore */
#define MSG_WAIT       0x020 /* message */
#define BLOCK_WAIT     0x010 /* block */
#define RESOURCE_WAIT  0x008 /* resource */
#define DELAY_WAIT     0x004 /* delay wait */
#define PARTITION_WAIT 0x002 /* partition */
#define SUSPFLG        0x001 /* suspended */

#define READY          0x000 /* task is runnable */

#endif

/* End of file - tstate.h */
