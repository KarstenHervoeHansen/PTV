#include "time_utils.h"

// Daycount in month 1 --> 12. (Leap year is handeled in calculations.)
unsigned char code DayTable[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
unsigned char code DayTableLeap[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// Days between 1/1/0001 and 1/1/1900
unsigned int code DateDelta = 693596;

unsigned char IsLeapYear(int year)
{
	if ( (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0)) )
	{ return(1); } else { return(0); } 
}

unsigned int DaysFrom1900(int year, char month, day)
{
	char xdata c;

	int xdata i;

    for (c = 0; c < month - 1; c++)
	{
		if (IsLeapYear(year) == 1) day = day + DayTableLeap[c];	
		else day = day + DayTable[c];	
	}

    year = year - 1;

    i = (year * 365) + (year / 4) - (year / 100) + (year / 400) + day - DateDelta;
	
    return (i);
}

unsigned long SecsFromMidnight(char hour, min, sec)
{
	unsigned long xdata iRes, iHour, iMin, iSec;
	
	iHour = hour; // Hour can not be used directly in the calculation, since the value will be changed during calculation and an overflow can occour.
	iMin = min;
	iSec = sec;
		
	iRes = (iHour * 3600) + (iMin * 60) + iSec;

    return (iRes);
}

unsigned long SecsFrom1900(int year, char month, day, hour, min, sec, gps_leapseconds)
{
	unsigned long xdata i;

	i = DaysFrom1900(year, month, day) * 86400L;

	i += SecsFromMidnight(hour, min, sec);
	
	i++; // <----------------------------TEST
	return(i);
}


