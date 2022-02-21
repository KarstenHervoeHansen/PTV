#define COM1 0
#define COM2 1
#define COM3 2
#define COM4 3

/* STRUCTURE DEFINITIONS */

typedef struct {
	char Company[12];						/* e.g. "PHILIPS" */
	char KuNo[9];							/* e.g. "KU123456" */
	char nc[13];							/* e.g. "4008xxxxxxxx" */
	char SoftWareRev[5];					/* e.g. "02.0" */
	int  SensorType;						/* MUST be > 31 && < 48 */
} IDINFO;

typedef struct {
	int IOerror;							/* Copy of return value i.e. IOerror */
	float X, Y, Z;       	     		/* Measurements from Sensor */
	float whr[6];							/* x, y, u', v', u, v */
} MEASINFO;

extern char* PM39IOErrorTxt[14];

int Command_To_COM( int COM_Port, char* str);

int Open_COM( int COM_Port);
int Close_COM( int COM_Port);

int Open_TIME_TICK( void);
int Close_TIME_TICK( void);

int Start_Measuring( int COM_Port);
int Stop_Measuring( int COM_Port);

int Set_Integration_Time( int COM_Port, int time);
float Get_Integration_Time( int COM_Port);

int Get_ID( int COM_Port, IDINFO* SensorID);
int Get_Measurement( int COM_Port, MEASINFO* Meas);

