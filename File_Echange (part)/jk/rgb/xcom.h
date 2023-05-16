#include <sys/types.h> 
#include <sys/timeb.h>
#include <time.h>

#define uchar unsigned char
#define uint  unsigned int

typedef struct
   {
   long delay;           /* time to wait */
   struct timeb start;   /* start time   */
   } chrono_t;

int   Xcom_init(int, long, int, int, int, long);
void  Xcom_Close(int);
void  Xcom_TxData(unsigned char, int);
void  Xcom_Flush(int);
int   Xcom_DataReady(int);
uchar Xcom_RxData(int);
int   Xcom_get_data(uchar *, int);
int   Xcom_get_data_wtimeout(uchar *, int, long);
void  start_chrono (chrono_t *, long);
int   end_chrono (chrono_t);
