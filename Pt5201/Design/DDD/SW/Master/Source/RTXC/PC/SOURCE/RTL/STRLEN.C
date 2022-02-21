/* strlen.c - RTXC compute string length function */

/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "runtm.h"
#include "utilprot.h"

/* return the length of a '\0'-terminated string, not including
   the ending '\0' */
int strlen(char const *ch)
{
    int length;

    for (length = 0; *ch != '\0'; ch++)
        length++;
    return length;
}

/* end of strlen.c */
