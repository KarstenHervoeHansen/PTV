/*
********************************************************************
* Project	: PT5210  Black Burst
* Filename	: BBU.H
* Version	: 1.2
* Purpose	: Variables definitions
* Org.date	: 960515
* Author	: PRC
********************************************************************
HISTORY:
970120 Changed name from VARS.H
970114 RELEASED 1.1
*/

bit Acknowledge;

// PORT ADDRESSES **
volatile UC typeport1 @	0xfff;	// port for linietype (and high of line, corse)
volatile UC hfasecport1 @ 0x1000; // port for line type
volatile UC bulowport1 @ 0x2000; // port for burst phase, low
volatile UC buhiport1 @	0x3000; // port for burst phase, high
volatile UC hfasefport1 @ 0x4000; // port for line phase, fine


volatile UC typeport2 @	0x8000;	// port for linietype (and high of line, corse)
volatile UC hfasecport2 @ 0x9000; // port for line type
volatile UC bulowport2 @ 0xA000; // port for burst phase, low
volatile UC buhiport2 @	0xB000; // port for burst phase, high
volatile UC hfasefport2 @ 0xC000; // port for line phase, fine


// VARIABLES 24BIT **



// PORT BITS **
sbit fieldpuls1 @ 0x39c;
sbit fieldpuls2 @ 0x399;
sbit P3_0 @ 0x398;
sbit P3_1 @ 0x399;
sbit P3_2 @ 0x39a;
sbit P3_5 @ 0x39d;

// tables
extern code UC typeg[];
code UC *tablephg= typeg;
extern code UC typem[];
code UC *tablephm= typem;
extern code UI sfaseg[];
extern code UI sfasem[];


