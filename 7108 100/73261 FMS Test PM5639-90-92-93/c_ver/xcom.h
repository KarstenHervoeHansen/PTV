/* File: xcom.h  */

#define uchar unsigned char
#define uint  unsigned int

extern int CCFlag; /* Control-Break flag; if 1 then CtrlBrk pressed */

int   Xcom_Init(int, long, int, int, int, long);
void  Xcom_Close(int);
int   Xcom_Putchar(uchar, int);
int   Xcom_Putdata(uchar *, int, int);
void  Xcom_Flush(int);
int   Xcom_DataReady(int);
int   Xcom_Getchar(uchar *, int);
int   Xcom_Getchar_wtimeout(uchar *, int, long);

void  INstallNewCC(int);
void  INstallOldCC(void);
