//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

unsigned char IsLeapYear(unsigned int year);
unsigned int DaysFrom1900(unsigned int year, unsigned char month, day);
unsigned long SecsFromMidnight(char hour, min, sec);
unsigned long SecsFrom1900(unsigned int year, unsigned char month, day, hour, min, sec, leapseconds);