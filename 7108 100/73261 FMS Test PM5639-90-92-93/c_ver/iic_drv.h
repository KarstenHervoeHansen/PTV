/* iic_drv.h */

#include <sys/types.h> 
#include <sys/timeb.h>
#include <time.h>

#define UC unsigned char


int trmDA8591(int, UC, UC);
int read8574(int, UC, UC *);
int trmAD8591(int, int, UC);
int read8591(int, int, UC *);
int trm8574(int, UC, UC);
int init_iic(int);
