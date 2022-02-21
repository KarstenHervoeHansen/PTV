#include <string.h>
#include <stdlib.h>
#include "inouts.h"
#include "NavSync_prot.h"
#include "UART.h"

//struct time_info GPS_time;
long xdata timing_info[16];
char xdata in_string[115];
char gps_valid;

void get_info()
{
	char index, offset, string_count=0;
	long temp_val=0;
	char temp_string[8]="00000000";
	char set[3]={'.',',',0x0D};

	//hent buffer fra UART
	read_buf(&in_string);
		
	//hvis headeren er POLYT (returnere 0 hvis sandt!)
	if(strncmp(in_string, "POLYT", 5)==0)
	{
		//index efter "$polyt,"
		index=6; string_count=0;

		while (string_count!=15 && index<100)
		{
			//reset midlertidig streng
			memset(temp_string, '\0', sizeof(temp_string));
			//søg efter næste komma, punktum eller end-line
			offset = strcspn(&in_string[index], set);
			//kopier fra index og længden af næste streng
			memcpy(temp_string, &in_string[index], offset);
			//opdater index
			index=index+offset+1;
			
			timing_info[string_count++]=atol(temp_string);
		}
	}

	//hvis headeren er GPGLL
	if (strncmp(in_string, "GPGLL", 5)==0)
	{
		//'A' på plads 46 i strengen betyder valid GPS lock
		if(in_string[46]=='A')
			gps_valid=1;
		else
			gps_valid=0;
	}
}

void setup_msg(char *str)
{
	int n=0;

	for(n; n<strlen(str); n++)
		putch(str[n]);

	putch(0x0D);
	putch(0x0A);
}

