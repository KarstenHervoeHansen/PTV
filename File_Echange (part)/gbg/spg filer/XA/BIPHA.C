#include <stdio.h>

#define UC unsigned char

void bipha(UC a)
{ UC m,b;
 b= 0;
 for (m=0;m<3;m++)
 { if (a & 1)  b |= 2; else b |= 1;
   b<<=2; a>>=1;
 }
 if (a & 1)  b |= 2; else b |= 1;
 a>>=1;
// XBYTE[sendptr++]= b;
 b= 0;
 for (m=0;m<3;m++)
 { if (a & 1)  b |= 2; else b |= 1;
   b<<=2; a>>=1;
 }
 if (a & 1)  b |= 2; else b |= 1;
 a>>=1;
// XBYTE[sendptr++]= b;
}

main()
{
 bipha(1);
}
