//Rev 1_1 added InitBusses : inits all iic busses (InitBus(x) becomes obsolete
//		  added support for 1+6 busses
int InitBus(UC bus);

int InitBusses(void);

int TransmitIIC(UC Address ,UC num ,UC *data ,UC bus );
int ReceiveIIC(UC Address ,UC num ,UC *data ,UC bus );

#define IIC_PTV 0
#define IIC_1	1
#define IIC_2   2
#define IIC_3   3
#define IIC_4   4
#define IIC_5   5
#define IIC_6   6